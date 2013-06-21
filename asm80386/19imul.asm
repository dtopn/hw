.386
data segment use16;use16表示偏移地址采用16位
s db "12345678", 0
x dd 0
data ends
code segment use16
assume cs:code, ds:data
main:
   mov ax, data
   mov ds, ax
   mov bx, 0
   mov eax, 0
   mov edx, 0
next:
   mov dl, s[bx]
   cmp dl, 0
   je done
   imul eax, eax, 10
   sub dl, '0'
   add eax, edx
   inc bx
   jmp next
done:
   mov [x], eax
   mov ah, 4Ch
   int 21h
code ends
end main
