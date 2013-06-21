data segment
x dw 0FFFFh
s db 5 dup (0Dh),0Dh,0Ah,"$"
data ends
code segment
assume cs:code, ds:data
main:
   mov ax, data
   mov ds, ax
   mov bx, 0; 下标
   mov bp, 0; 数位统计
   mov ax, [x]
next:
   mov dx, 0; 保证被除数的高16位为0
   mov cx, 10
   div cx; (DX:AX)/CX=AX..DX
   add dl, '0'
   push dx
   inc bp
   cmp ax, 0
   jne next
store:
   pop dx
   mov s[bx], dl
   inc bx
   dec bp
   jnz store
   mov ah, 9
   mov dx, offset s
   int 21h
   mov ah, 4Ch
   int 21h
code ends
end main
