data segment
	a dw 0
	b dw 0
	str1 db "Num1: $"
	str2 db "Num2: $"

	num_buff db 5 dup(.), '$'
	char_buff db 5,?,5 dup(.)
data ends

code segment
assume cs:code, ds:data

main:
	mov ax, data
	mov ds, ax

	mov dx, offset a
	;call cout
	mov ah, 9h
	int 21h
	lea ax, a
	call read_char2int_2_ax
	
read_char2int_2_ax proc
	
