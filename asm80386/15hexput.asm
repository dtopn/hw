 data segment
 t db "0123456789ABCDEF"
 x dw 1111000010100110B
 data ends
 code segment
 assume cs:code, ds:data
 main:
    mov ax, data
    mov ds, ax
    mov bx, 4; loop times
    mov cl, 4; shift left 4 digits
 next_4bit:
    rol x, cl; x shift left 4 digits
    mov si, x
    and si, 000Fh
    mov dl, t[si]
    mov ah, 2
    int 21h; output chars in dl
    dec bx
    jnz next_4bit
    mov ah, 4Ch
    int 21h
 code ends
 end main


