data segment
x dw 0FFFFh
s db "00000",0Dh,0Ah,"$"
data ends
code segment
assume cs:code, ds:data
main:
   mov ax, data
   mov ds, ax
   mov bx, 4; 下标
   mov ax, [x]
next:
   mov dx, 0; 保证被除数的高16位为0
   mov cx, 10
   div cx; (DX:AX)/CX=AX..DX
   add dl, '0'
   mov s[bx], dl
   cmp ax, 0
   je  done
   dec bx
   jmp next
done:
   mov ah, 9
   mov dx, offset s
   int 21h
   mov ah, 4Ch
   int 21h
code ends
end main
