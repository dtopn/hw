data segment
	other db "other", 0dh, 0ah, "$"
	lower db "lowcase", 0dh, 0ah, "$"
	number db "number", 0dh 0ah, "$"
	upper db "uppercase", 0dh, oah, "$"
	other db "other", 0dh, oah, "$"
	char db "AZaz09"
	addr dw offset upper, offset lower	;define word
		dw offset number, offset other	;type addr 0 2 4 6 to cite them
data ends

code segment
	assume cs:code, ds:data
main:
	mov ax, data
	mov ds, ax
	mov si, 0	;si as char numberset indicator
	mov di, 0 	;di as addr numberset indicator
	mov cx, 3	;loop 3times 
	mov ah, 01h
	int 21h
		;cmp al, 'a'
		;jb not_lowcase	;jump if below
		;cmp al, 'z'
		;ja not_lowcase	;jump if above
		;		;sub al, 20h
		;jmp is_lower
 	again:
		cmp al, char[si]
		
not_lowcase:
mov ah,2
mov dl, al
int 21h
mov ah, 4ch
int 21h
output:
	mov ah, 9
	int 21h

exit:
	mov ah, 4ch
	int 21h
code ends
end main
