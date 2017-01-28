; float __fastcall dot_avx_asm(const float *a, const float *b, size_t len);
;
public dot_avx_asm

.code

dot_avx_asm PROC
	; x64 calling convension:
	; - rcx ... a   (1st argument)
	; - rdx ... b   (2nd argument)
	; - r8d ... len (3rd argument)
	; - xmm0 ... return value (for float type)

	mov		r10, rsp					; secure RSP to R10
	and		rsp, 18446744073709551584	; 0xFFFFFFFFFFFFFFE0... discarding lower 32 bits to align
	sub		rsp, 32						; allocate `float accum[8];`
	mov		r9, rsp						; R9= accum

	vxorps	ymm0, ymm0, ymm0			; nullify ymm0 register
	mov		rax, 0						; using RAX as offset

multiplication:
	vmovaps			ymm1, YMMWORD PTR [rcx+rax]				; ymm0[0:7]= a[0:7]
	vdpps			ymm1, ymm1, YMMWORD PTR [rdx+rax], 241	; (ymm0[0:1], ymm0[4:5])= dot(ymm0[0:7], b[0:7], 0xf1) <_mm_dp_ps>
	vextractf128	xmm2, ymm1, 1							; ymm2[0:3]= ymm1[4:7]
	addss			xmm0, xmm1								; ymm0[0:1]+=ymm1[0:1]
	addss			xmm0, xmm2								; ymm0[0:1]+=ymm2[0:1]
	add				rax, 32									; a+= 32; b+= 32
	sub				r8d, 8									; len-= 8
	jne				multiplication							; if (len != 0) goto multiplication

	mov				rsp, r10
	ret				0

dot_avx_asm endp

end
