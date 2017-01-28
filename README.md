# simd\_test
SIMD instruction benchmark

Created to learn how to use SIMD instructions in C with compiler intrinsics
and x64 assembly language (with Visual Studio for Windows program.)

This program executes some very basic linear algebra (vector) operations and
also contains logic to determine whether the CPU supports some SIMD instruction
set or not (the program isn't using it though). If you interested, just look at
the source files. Note that there are no x86 (Win32) version of .asm files.

## Example Results

An example output from the program built with Visual Studio 2015
(on a laptop with Core i7 6600U, Skylake architecture):

	Number of times each function ran in 0.5 secs:
	add_c               :  471470 times
	add_sse             :  496611 times
	add_avx             :  830946 times
	mul_c               :  486972 times
	mul_sse             :  497136 times
	mul_avx             :  813090 times
	div_c               :  498424 times
	div_sse             :  492239 times
	div_avx             :  595721 times
	mul_add_c           :  384463 times
	mul_add_sse         :  281115 times
	mul_add_avx         :  506863 times
	dot_c               :   96564 times
	dot_sse             :  377050 times
	dot_sse41_dp        :  254332 times
	dot_sse_asm         :  387838 times
	dot_avx             :  306822 times
	dot_avx_dp          :  437111 times
	dot_avx_asm         :  384199 times

An example output from the program built with Visual Studio 2008:

	Number of times each function ran in 0.5 secs:
	add_c               :  222883 times
	add_sse             :  642514 times
	mul_c               :  233686 times
	mul_sse             :  667243 times
	div_c               :  127245 times
	div_sse             :  508287 times
	mul_add_c           :  157523 times
	mul_add_sse         :  345454 times
	dot_c               :   96752 times
	dot_sse             :  380795 times
	dot_sse41_dp        :  251552 times
	dot_sse_asm         :  379998 times

## Notes

Some notes for who reads this readme:

- The results above are not a comparison of C, SSE, and AVX -- it's just a
  result of my implementation.
- Benchmark result is very unstable so that speed ranking changes almost
  every time I've executed -- test yourself.

Some notes I've learned by writing this:

- Since every x64 CPU supports SSE, SSE checking logic is not needed for 64-bit
  program.
- As naturally expected, VS2015 emits much optimized code than VS2008's one.
- As for the cost of implementation, compiler intrinsics is better than
  hand-written assembly -- significantly easier to write, safer because of
  type checking, no need to reimplement architecture by architecture, etc.

One important thing to note. Even though output from code using compiler
intrinsics is generally as fast as fine-tuned hand-written assembly, it CAN be
slower. Actually I couldn't write effective code using instrinsics first time.
Then I checked how my code was translated at disassembly window in Visual Studio,
learned which part is inefficient, and tried to change my code more compiler
friendly to make that part efficient. The problem in my case was that the
intrinsic function I chose was not optimal for the case. Since compiler
intrinsics directs the compiler to use the very specific instruction, choosing
unsuitable intrinsics forces the compiler to emit the unsuitable instruction,
and forces to make a detour. To avoid such pitfalls, and to shoot such
troubles, I felt that it's better to know basic assembly language to analyze
what's going on behind the scene.

Once I understood basics of x64 assembly, there seems to be no reason to choose
writing assembly code by hand any more.
