code segment
assume cs:code
main:
   mov ax, 0B800h
   mov es, ax
   mov di, 0; ES:DI
   mov cx, 80*25
again:   
   mov byte ptr es:[di], "A"
   mov byte ptr es:[di+1], 17h
   mov bx, 800h
wait2:
   mov dx, 0FFFFh
wait1:
   sub dx, 1
   jnz wait1
   sub bx, 1
   jnz wait2
   mov byte ptr es:[di], ' '
   mov byte ptr es:[di+1], 00h
   add di, 2
   sub cx, 1
   jnz again
   mov ah, 1
   int 21h
   mov ah, 4Ch
   int 21h
code ends
end main
