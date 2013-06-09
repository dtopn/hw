.386
data segment use16
upleft    dw 320/2, 200/2; (upleft.x, upleft.y)
downright dw 320/2, 200/2; (downright.x, downright.y)
red   db 64 dup(0)
green db 64 dup(0)
blue  db 64 dup(0)
color db 1
seed  dd 0
data ends

code segment use16
assume cs:code, ds:data
main:
   mov ax, data
   mov ds, ax
   mov ax, 0013h
   int 10h; set video mode to 320*200
   mov ax, 0A000h
   mov es, ax
   call set_palette; at first, all 64 colors are set to black
   
   mov cx, 64
draw_next_rectangle:   
   call draw_rectangle
   dec upleft[0]
   dec upleft[2]
   inc downright[0]
   inc downright[2]
   inc color
   loop draw_next_rectangle

   call randomize
   mov bp, 64*2
moving:  
   call move_colors
   call set_palette
   call delay
   dec bp
   jnz check_key
   mov bp, 64*2
   call random_color
check_key:   
   mov ah, 1
   int 16h   ; check key press
   jz  moving; if no key is pressed => goto moving

   mov ax, 0003h
   int 10h   ; reset video mode to 80*25 text mode
   mov ah, 4Ch
   int 21h

;-----------Draw_rectangle-------------
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
   loop upline_next_dot
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
   loop down_line_next_dot
   
   mov si, bx
   mov cx, downright[2]
   sub cx, upleft[2]
   inc cx; CX=dots for left line
   push cx
   mov al, color
left_line_next_dot:
   mov byte ptr es:[si], al
   add si, 320
   loop left_line_next_dot
   pop cx
   
   mov si, bx
   mov ax, downright[0]
   sub ax, upleft[0]
   add si, ax; SI=video offset address for upright dot
   mov al, color
right_line_next_dot:
   mov byte ptr es:[si], al
   add si, 320
   loop right_line_next_dot
   pop cx
   ret
;--------End of Draw_rectangle----------

;--------Set_palette---------
set_palette:
   mov cx, 64
   mov si, 0
   mov bl, 1; color=1
   mov dx, 3C8h; DX=3C8h=color index register
next_palette:
   mov al, bl
   out dx, al; ready to set color BL
   inc dx    ; DX=3C9h=color ingredients register
   mov al, red[si]
   out dx, al; set red ingredients for color BL
   mov al, green[si]
   out dx, al; set green ingredients for color BL
   mov al, blue[si]
   out dx, al; set blue ingredients for color BL
   dec dx
   inc si
   inc bl; color++
   loop next_palette
   ret
;------End of Set_palette------------

;------Move_colors-------------------
move_colors:
   mov bx, 63
   mov cx, 63
move_colors_next:
   mov al, red[bx-1]
   mov red[bx], al
   mov al, green[bx-1]
   mov green[bx], al
   mov al, blue[bx-1]
   mov blue[bx], al
   dec bx
   loop move_colors_next
   inc blue[0]
   cmp blue[0], 63
   jbe move_colors_done
   mov blue[0], 0
   inc green[0]
   cmp green[0], 63
   jbe move_colors_done
   mov green[0], 0
   inc red[0]
   cmp red[0], 63
   jbe move_colors_done
   mov red[0], 0
move_colors_done:
   ret   
;--------End of Move_colors--------

;--------Delay---------------------
delay:
   mov cx, 100h
just_wait:
   push cx
   xor cx, cx
self:
   loop self
   pop cx
   loop just_wait
   ret
;-------End of Delay---------------   

;-------Randomize------------------
randomize:
   push es
   push bx
   xor bx, bx
   mov es, bx
   mov bx, 46Ch
   mov eax, es:[bx]; dword ptr 0000:[046C] contains time ticks
   mov seed, eax
   pop bx
   pop es
   ret
;-------End of Randomize-----------

;-------Random_color---------------
random_color:
   call rand
   and al, 3Fh; ensure AL <= 63
   mov red[0], al
   call rand
   and al, 3Fh
   mov green[0], al
   call rand
   and al, 3Fh
   mov blue[0], al
   ret      
;-------End of Random_color--------

;-------Rand--------------
rand:
   imul eax, seed, 015A4E35h ; imul eax, seed, 343FDh
   inc eax                   ; add eax, 269EC3h
   mov seed, eax             ; mov seed, eax
   shr eax, 10h              ; shr eax, 10h
   and ax, 7FFFh             ; and eax, 7FFFh
   ret
;-------End of Rand-------
code ends
end main
  
