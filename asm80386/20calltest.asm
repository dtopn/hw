data segment
s db 10 dup(0)
t db 10 dup(0)
data ends
code segment
assume cs:code, ds:data
main:
   mov ax, data
   mov ds, ax
   mov di, offset s
   call input
   mov di, offset t
   call input
   mov ah, 4Ch
   int 21
input:
   push di
   push cx
   mov cx, 0
input_next:
   mov ah, 1
   int 21h
   cmp al, 0Dh
   je input_done
   inc cx
   mov [di], al
   inc di
   jmp input_next
input_done:
   mov byte ptr [di], 0
   mov ax, cx; AX用作函数的返回值
   pop cx
   pop di
   ret
code ends
end main
