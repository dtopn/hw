data segment
	digi db "0123456789ABCDEF"
data ends

code segment
assume cs:code, ds:data

main:
	mov ax, data
	mov ds, ax
	
	mov ax, 3
	int 10h

	mov ax, 0b800h
	mov es, ax
	mov di, 0
	mov cx, 0
	mov bp, 0
again1:
	mov es:[bp+di], cl
	mov byte ptr es:[bp+di+1], 4
	;--------------------out hexnum
	mov al, cl
	and ax, 0ffh
	mov bl, 10h
	div bl	;-------------------quotient al
	mov bx, offset digi
	xlat	;-------------------al <- digi high
	mov es:[bp+di+2], al
	mov byte ptr es:[bp+di+3], 2
	
	mov al, cl
	and ax, 0ffh
	mov bl, 10h
	div bl 
	mov al, ah;-------------------remider ah -> al
	mov bx, offset digi
	xlat
	mov es:[bp+di+4], al
	mov byte ptr es:[bp+di+5], 2
	;--------------------next line
	add di, 80 * 2
	inc cx
	cmp cx, 100h
	jz end1
	cmp di, 25 * 80 * 2
	jnz again1
	add bp, 14
	mov di, 0
	jmp again1
end1:
	mov ah, 1
	int 21h
	mov ax, 3
	int 10h
	mov ah, 4ch
	int 21h
code ends
end main