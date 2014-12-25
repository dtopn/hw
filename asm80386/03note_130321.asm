.386
;32bit assembly
:

.data
result db 100 dup(0) 	;100 ge 0 dup(1,2,3,4)
			;static result[100]
format db "%d", 0
prompt db "The result", 0

.code
main:
	mov eax, 0	;extended ax 16bit(H) + 16bit(L)(ax)
			;if you want to assign a value to 16bit(H)
			;then use this
			;mov ebx, 12340000h
			;and eax, 0000FFFFh
			;or eax, ebx
	mov ebx, 1
again:
	add eax, ebx	;eax = 0 + 1 + 2 + 3
	add ebx, 1	;ebx = 4
	cmp ebx, 100	;compare
	jbe again	;jump if below or equal
invoke wsprintf, offset result, offset format
	;equals 
	;offset format, eax
	;push eax
	;push offset format
	;push offset result
invoke MessageBox, 0, offset result, offset prompt
	ret
end main

char [buf]100;
int x = 5050
sprintf 

