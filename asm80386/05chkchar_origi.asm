data segment
upper db "uppercase$"
lower db "lowercase$"
number db "number$"
other db "other$"
data ends
code segment
assume cs:code, ds:data
main:
   mov ax, data
   mov ds, ax
   mov ah, 1
   int 21h
   cmp al, 'A'
   jb not_upper
   cmp al, 'Z'
   ja not_upper
is_upper:
   mov dx, offset upper
   jmp output
not_upper:
   cmp al, 'a'
   jb not_lower
   cmp al, 'z'
   ja not_lower
is_lower:
   mov dx, offset lower
   jmp output
not_lower:
   cmp al, '0'
   jb not_number
   cmp al, '9'
   ja not_number
is_number:
   mov dx, offset number
   jmp output
not_number:
   mov dx, offset other
output:
   mov ah, 9
   int 21h
exit:
   mov ah, 4Ch
   int 21h
code ends
end main
