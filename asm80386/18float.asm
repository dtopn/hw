;Turbo Debugger跟踪时，
;点菜单View->Numeric Processor查看小数堆栈
data segment
abc dd 3.14
xyz dd 2.0
result dd 0
data ends
code segment
assume cs:code, ds:data
main:
   mov ax, data
   mov ds, ax
   fld [abc]; 把3.14载入到小数堆栈
   fld [xyz]; 把2.0载入到小数堆栈
   fmul st, st(1); 两数相乘
   fstp [result]; 保存结果到result，并弹出
   fstp st      ; 弹出小数堆栈中残余的值
   mov ah, 4Ch
   int 21h
code ends
end main
