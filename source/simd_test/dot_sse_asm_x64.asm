; float __fastcall dot_sse_asm(const float *a, const float *b, size_t len);
;
public dot_sse_asm

.code

dot_sse_asm PROC
	; x64 calling convension:
	; - rcx ... a   (1st argument)
	; - rdx ... b   (2nd argument)
	; - r8d ... len (3rd argument)
	; - xmm0 ... return value (for float type)

	push	rbx						; secure RBX address
	sub		rsp, 16					; allocate `float accum[4]` on stack [*]
	mov		rbx, rsp				; RBX= accum

	xorps	xmm1, xmm1				; nullify xmm1 register
	mov		rax, 0					; using RAX as offset

multiplication:
	movaps	xmm0, XMMWORD PTR [rcx+rax]	; xmm0= (a[0] a[1] a[2] a[3])
	mulps	xmm0, XMMWORD PTR [rdx+rax]	; xmm0*=(b[0] b[1] b[2] b[3])
	addps	xmm1, xmm0					; xmm0+=xmm0
	add		rax, 16						; a+=4; b+=4
	sub		r8d, 4						; len-=4
	jne		multiplication				; if (len != 0) goto multiplication

	movaps	XMMWORD PTR [rbx], xmm1		; store result to the local float[4]
	movss	xmm0, DWORD PTR [rbx]		; xmm0[0:1]= accum[0]
	addss	xmm0, DWORD PTR [rbx+4]		; xmm0[0:1]+=accum[1]
	addss	xmm0, DWORD PTR [rbx+8]		; xmm0[0:1]+=accum[2]
	addss	xmm0, DWORD PTR [rbx+12]	; xmm0[0:1]+=accum[3]

	add		rsp, 16
	pop		rbx
	ret		0
	; [*] "call" of this function and the "push" of RBX moves subtracts RSP
	;     8 bytes per each so it is 16-byte aligned there.

dot_sse_asm endp

end
