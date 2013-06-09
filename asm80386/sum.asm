.386
.model flat, stdcall
option casemap :none


include include\windows.inc
include include\kernel32.inc
include include\user32.inc

includelib lib\kernel32.lib
includelib lib\user32.lib

.data

result db 100 dup(0)
format db "%d",0
prompt db "The result",0

.code
main:
    mov eax, 0
    mov ebx, 1
again:
    add eax, ebx
    add ebx, 1
    cmp ebx, 100
    jbe again
    invoke wsprintf, addr result, addr format, eax
    invoke MessageBox, NULL, addr result, addr prompt, MB_OK
    ret
   
end main
