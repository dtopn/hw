data segment
upper db "uppercase$"
lower db "lowercase$"
number db "number$"
other db "other$"
char db "AZaz09"
addr dw offset upper, offset lower
     dw offset number, offset other
     ; dw:define 
	;type addr 0 2 4 6 to cite them
data ends
code segment
assume cs:code, ds:data
main:
   mov ax, data
   mov ds, ax
   mov si, 0
   mov di, 0 ; si di shuzu xia biao
   mov cx, 3 ; loop 3 times
   mov ah, 1
   int 21h
again:
   cmp al, char[si]
   jb next
   cmp al, char[si+1]
   ja next
   mov dx, addr[di]
   jmp output
next:
   add si, 2	
   add di, 2
   sub cx, 1
   jnz again
   mov dx, addr[di]
output:
   mov ah, 9
   int 21h
exit:
   mov ah, 4Ch
   int 21h
code ends
end main
