data segment
upleft    dw 320/2, 200/2; (upleft.x, upleft.y)
downright dw 320/2, 200/2; (downright.x, downright.y)
color     db 1
data ends

code segment
assume cs:code, ds:data
main:
   mov ax, data
   mov ds, ax
   mov ax, 0013h
   int 10h; set video mode to 320*200
   mov ax, 0A000h
   mov es, ax
   
   mov cx, 64
draw_next_rectangle:   
   call draw_rectangle
   dec upleft[0]
   dec upleft[2]
   inc downright[0]
   inc downright[2]
   inc color
   dec cx
   jnz draw_next_rectangle
   mov ah,1
   int 21h
   mov ax, 0003h
   int 10h
   mov ah, 4Ch
   int 21h
   

draw_rectangle:
   push cx
   mov ax, 320
   mul upleft[2]; AX=320*upleft.y
   add ax, upleft[0]
   mov bx, ax   ; BX=video offset address for upleft dot

   mov cx, downright[0]
   sub cx, upleft[0]
   inc cx       ; CX=dots for upline or downline
   mov si, bx
   mov al, color
   push cx
upline_next_dot:
   mov es:[si], al
   inc si
   dec cx
   jnz upline_next_dot
   pop cx

   mov dx, downright[2]
   sub dx, upleft[2]; dx=dots between upline and downline
   mov ax, 320
   mul dx; AX=320*(downright.y-upleft.y)
   mov si, bx
   add si, ax; SI=video offset address for downleft dot
   mov al, color
down_line_next_dot:
   mov es:[si], al
   inc si
   dec cx
   jnz down_line_next_dot
   
   mov si, bx
   mov cx, downright[2]
   sub cx, upleft[2]
   inc cx; CX=dots for left line
   push cx
   mov al, color
left_line_next_dot:
   mov byte ptr es:[si], al
   add si, 320
   dec cx
   jnz left_line_next_dot
   pop cx
   
   mov si, bx
   mov ax, downright[0]
   sub ax, upleft[0]
   add si, ax; SI=video offset address for upright dot
   mov al, color
right_line_next_dot:
   mov byte ptr es:[si], al
   add si, 320
   dec cx
   jnz right_line_next_dot
   pop cx
   ret
code ends
end main

