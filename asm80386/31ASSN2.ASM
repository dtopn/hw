data segment
	a dw 0
	b dw 0
	sumh dw 0
	suml dw 0
	mulh dw 0
	mull dw 0
	decc db 10 dup(3),'$'
	crlf db 0dh, 0ah, '$'
	digi db "0123456789ABCDEF"
	str1 db "Numb1: $"
	str2 db "Numb2: $"
data ends


code segment
assume cs:code, ds:data

main:
	mov ax, data
	mov ds, ax

	mov dx, offset str1
	mov ah, 9h
	int 21h
	mov si, offset a
	call readin
	
	mov dx, offset str2
	mov ah, 9h
	int 21h
	mov si, offset b
	call readin
	
	mov dx, 0
	mov ax, a
	add ax, b
	adc dx, 0
	mov sumh, dx
	mov suml, ax
	
	mov ax, a
	mul b
	mov mulh, dx
	mov mull, ax	

	mov ax, a
	mov dx, 0
	call decout
	mov ah, 2
	mov dl, '+'
	int 21h
	mov ax, b
	mov dx, 0
	call decout
	mov ah, 2
	mov dl, '='
	int 21h
	call crlfout
	mov ax, suml
	mov dx, sumh
	call decout
	call crlfout

	mov ax, sumh
	call hexout
	mov ax, suml
	call hexout
	mov dl, 'h'
	mov ah, 2
	int 21h
	call crlfout
	mov ax, sumh
	call binout
	mov dl, ' '
	call charout
	mov ax, suml
	call binout
	mov dl, 'B'
	call charout
	call crlfout
	
	mov ax, a
	mov dx, 0
	call decout
	mov ah, 2
	mov dl, '*'
	int 21h
	mov ax, b
	mov dx, 0
	call decout
	mov ah, 2
	mov dl, '='
	int 21h
	call crlfout
	mov ax, suml
	mov dx, sumh
	call decout
	call crlfout
	
	mov ax, mull
	mov dx, mulh
	call decout
	call crlfout

	mov ax, mulh
	call hexout
	mov ax, mull
	call hexout
	mov dl, 'h'
	mov ah, 2
	int 21h
	call crlfout
	mov ax, mulh
	call binout
	mov dl, ' '
	call charout
	mov ax, mull
	call binout
	mov dl, 'B'
	call charout
	call crlfout
	
	;-----------ends
	mov ah, 4ch
	int 21h

binout:				;ax
	push bx
	push cx
	push dx
	push si
	mov cl, 1		;shl 1
	mov bx, 16		;repeat 16times
	mov si, 4
next_bin:
	cmp si, 0
	jnz no_space
	mov si, 4
	mov dl, ' '
	call charout
no_space:	
	shl ax, cl
	mov dl, 30h
	jnc not_inc
	inc dl
not_inc:
	call charout
	dec si
	sub bx, 1
	jnz next_bin

	
	pop si
	pop dx
	pop cx
	pop bx
	ret

charout:
	push ax
	mov ah, 2
	int 21h
	pop ax
	ret
	
hexout:				;ax is put out
	push bx
	push cx
	push dx
	push si

	mov bx, 4; loop times
	mov cl, 4; shift left 4 digits
next_4bit:
	
	rol ax, cl; x shift left 4 digits
	mov si, ax
	and si, 000Fh
	mov dl, digi[si]
	push ax
	mov ah, 2
	int 21h; output chars in dl
	pop ax
	dec bx
	jnz next_4bit
	
	pop si
	pop dx
	pop cx
	pop bx
	ret
	
decout:
	;; dexout dx:ax
	push bx
	push cx
	push di
	push bp

	mov bp, offset decc
	mov di, 9
	
next_dec:	
	call div10		;dx:ax/=10 bx reminder
	add bx, 30h
	mov ds:[bp+di], bl
	dec di
	push dx
	push ax
	or dx, ax
	pop ax
	pop dx
	jz end_dec
	jmp next_dec
end_dec:
	mov dx, di
	add dx, bp
	inc dx
	mov ah, 9
	int 21h

	pop bp
	pop di
	pop cx
	pop bx
	ret

div10:
	;; div dx:ax with 10, bx remider, dx:ax quotient
	push cx
	
	mov cx, ax		;high->low low->temp
	mov ax, dx
	mov dx, 0
	mov bx, 10
	div bx
	push ax			;quotient.high->temp1
	mov ax, cx		;temp->low
	div bx			;qutient.low->ax
	mov bx, dx
	pop dx			;quotient.high->dx
 
	pop cx
	;bx remider dx:ax quotient	
	ret

crlfout:
	push ax
	push dx
	mov dx, offset crlf
	mov ah, 9
	int 21h
	pop dx
	pop ax
	ret
	
readin:
	push ax
	push dx
	push bx
 nextchar:
	mov ah, 1
	int 21h
	cmp al, 0dh
	je over
	sub al, '0'
	mov bx, 0
	or bl, al
	mov ax, 10
	mul word ptr [si]
	mov word ptr [si], ax
	add word ptr [si], bx
	jmp nextchar
 over:
	pop bx
	pop dx
	pop ax
	ret
code ends
end main
