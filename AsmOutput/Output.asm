.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/StaticLibrary.lib
ExitProcess PROTO :DWORD

EXTRN COMBINE: proc
EXTRN MEASURE: proc
EXTRN BREAKL: proc
EXTRN RANDOMIZE: proc
EXTRN EXPONENT: proc
EXTRN OutputInt: proc
EXTRN OutputStr: proc

.stack 4096

.const
	L1 SDWORD 0
	L2 SDWORD 2
	L3 BYTE "Counting to length: ", 0
	L4 SDWORD 1
	L5 BYTE " ", 0
	L6 SDWORD 1
	L7 SDWORD 0
	L8 BYTE "Hey ", 0
	L9 BYTE "POIT!", 0
	L10 BYTE "Is length of str > 5?", 0
	L11 SDWORD 5
	L12 BYTE "Yeah! ", 0
	L13 BYTE "It is less", 0
	L14 SDWORD 3
	L15 SDWORD 4
	L16 BYTE "Is average of 4 and exponent [3,3] = 0?", 0
	L17 BYTE "It is zero", 0
	L18 BYTE "It is not zero: ", 0
	L19 BYTE "Remainer is: ", 0
	L20 SDWORD 57
	L21 BYTE "Number of the day is: ", 0
	L22 BYTE "Bye!", 0

.data
	buffer BYTE 256 dup(0)
	averresult SDWORD 0
	countni SDWORD 0
	iszerobuf SDWORD 0
	mainstring DWORD ?
	mainlength SDWORD 0
	mainha SDWORD 0
	mainresult SDWORD 0
	mainfl SDWORD 0
	mainhey SDWORD 0
	mainheh SDWORD 0

.code

aver PROC avern : SDWORD, averm : SDWORD
	push L1
	pop averresult
	push avern
	push averm
	pop eax
	pop ebx
	add eax, ebx
	push eax
	pop averresult
	push averresult
	push L2
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop averresult
	push averresult
	jmp local0
local0:
	pop eax
	ret
aver ENDP

countn PROC countnn : SDWORD
	push L1
	pop countni
	push offset L3
	call OutputStr
	push countnn
	call OutputInt
	call BREAKL
	mov eax, countni
	cmp eax, countnn
	jl cycle0
	jmp cyclenext0
cycle0:
	push countni
	push L4
	pop eax
	pop ebx
	add eax, ebx
	push eax
	pop countni
	push countni
	call OutputInt
	push offset L5
	call OutputStr
	mov eax, countni
	cmp eax, countnn
	jl cycle0
cyclenext0:
	ret
countn ENDP

iszero PROC iszeron : SDWORD
	mov eax, iszeron
	cmp eax, L1
	jz m0
	jnz m1
	je m1
m0:
	push L6
	pop iszerobuf
	jmp e0
m1:
	push L7
	pop iszerobuf
e0:
	push iszerobuf
	jmp local1
local1:
	pop eax
	ret
iszero ENDP

main PROC
	push offset L8
	push offset L9
	pop edx
	pop edx
	push offset L9
	push offset L8
	push offset buffer
	call COMBINE
	push eax
	pop mainstring
	push mainstring
	call OutputStr
	call BREAKL
	push mainstring
	pop edx
	push mainstring
	call MEASURE;
	push eax
	pop mainlength
	push offset L10
	call OutputStr
	call BREAKL
	mov eax, mainlength
	cmp eax, L11
	jg m2
	jl m3
	je m3
m2:
	push offset L12
	call OutputStr
	push mainlength
	call countn
	call BREAKL
	jmp e1
m3:
	push offset L13
	call OutputStr
	call BREAKL
e1:
	push L14
	push L14
	pop edx
	pop edx
	push L14
	push L14
	call EXPONENT;
	push eax
	pop mainha
	push aver
	push L15
	push mainha
	pop edx
	pop edx
	push mainha
	push L15
	call aver
	push eax
	pop mainresult
	push offset L16
	call OutputStr
	call BREAKL
	push iszero
	push mainresult
	pop edx
	push mainresult
	call iszero
	push eax
	pop mainfl
	mov eax, mainfl
	cmp eax, 1
	jz m4
	jnz m5
	je m5
m4:
	push offset L17
	call OutputStr
	call BREAKL
	jmp e2
m5:
	push offset L18
	call OutputStr
	push mainresult
	call OutputInt
	call BREAKL
	push offset L19
	call OutputStr
	push mainha
	push L15
	pop eax
	pop ebx
	add eax, ebx
	push eax
	push L2
	pop ebx
	pop eax
	cdq
	idiv ebx
	push edx
	pop mainhey
	push mainhey
	call OutputInt
	call BREAKL
e2:
	push L2
	push L20
	pop edx
	pop edx
	push L20
	push L2
	call RANDOMIZE;
	push eax
	pop mainheh
	push offset L21
	call OutputStr
	push mainheh
	call OutputInt
	call BREAKL
	push offset L22
	call OutputStr
	call ExitProcess
main ENDP
end main