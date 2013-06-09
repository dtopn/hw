 code segment
 assume cs:code
 main:
    mov bl, 0A5h
    mov cx, 8
 again:
    shl bl, 1
    jc bit_one
 bit_zero:
    mov dl, '0'
    jmp output
 bit_one:
    mov dl, '1'
 output:
    mov ah, 2
    int 21h
    dec cx
    jnz again
    mov ah, 4Ch
    int 21h
 code ends
 end main



 #include <dos.h>
 #include <stdio.h>
 main()
 {
    unsigned char s[]=
    {
       0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,
       0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00
    };
    int i, j;
    char far *p = (char far *)0xA0000000;
    p += 320L*100;
    _AX = 0x0013;       /* mov ax, 0013h */
    geninterrupt(0x10); /* int 10h */

    for(i=0; i<16; i++)
    {
       for(j=7; j>=0; j--)
       {
   if((s[i] & (1<<j)) != 0)
      *(p+i*320+(7-j)) = 4;
       }
    }
    getchar();
 }

 data segment
 s db 00h,000h,038h,06Ch,0C6h,0C6h,0C6h,0FEh
   db 0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h
 data ends
 code segment
 assume cs:code, ds:data
 main:
    mov ax, data
    mov ds, ax
    mov ax, 0A000h
    mov es, ax
    mov ax, 0013h
    int 10h
    mov bx, 320*100
    mov di, bx; es:di->第100行首列
    mov dx, 16; 控制行循环
    mov si, 0; 数组s的下标
 next_row:
    mov al, s[si]; 取出点阵中的8位
    mov cx, 8; 控制列循环
 next_dot:
    shl al, 1
    jnc skip
    mov byte ptr es:[di], 4; 画红点
 skip:
    inc di
    dec cx
    jnz next_dot
    add bx, 320
    mov di, bx; es:di->下一行的行首
    inc si    ; ds:si->点阵中的下个元素
    dec dx
    jnz next_row
    mov ah, 1
    int 21h
    mov ax, 0003h
    int 10h
    mov ah, 4Ch
    int 21h
 code ends
 end main