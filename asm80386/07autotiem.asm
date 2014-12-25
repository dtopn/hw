  
comment #
    ��������ǰд��һ��DOSפ������������Ļ�����Ͻ���ʾ��ǰ��ʱ�䡣
��DOS����������autotime/u���԰������ڴ����Ƶ���
    Ҫ��DOS�±�TSR��һ��Ҫ�˽�һЩ�й��ж�(interrupt)��֪ʶ���ر���
��ʱ���йص��ж���int 8h��int 1Ch,��������ж�int 9hҲ��TSR�Ƚ�ϲ��
�ӹܵ��жϣ��������п��ȼ���hotkey�������TSR��Ҫ�õ�����
    �Ҿ��ò���COM��ʽдTSR�ȽϷ��㣬��Ϊ��ֻ��һ���Σ��Ҹ����μĴ���
��ֵ����ȡ���TSR�Ļ����������£�
    1. �ж��Լ��Ƿ��Ѿ�פ�������Ѿ�פ����ֱ�ӷ���DOS�������ظ�פ����
       һ����õķ����Ƕ�ȡ�ӹܵ��ж�������ƫ�Ƶ�ַ�Ƿ����Լ���ͬ��Ҳ
       �����жϸ�������ָ���ĳ��ָ���ĳ����־�Ƿ����Լ���ͬ��
    2. ��ȡ������Ҫ�ӹܵ��ж�������int 21h/AH=35h����
    3. �޸�Ҫ�ӹܵ��ж�������int 21h/AH=25h����
    4. ����פ�����ȡ��������int 21h/AH=31h����פ����DXӦ�õ���
       �ڳ���paragraph����1paragraph=16bytes)��һ��������¹�ʽ���㣺
       DX=������PSP��ʼ��פ���������һ���ֽ�֮������ֽ�����+000Fh��/16
       ע��PSP(program segment prefix)��Ϊ�����ǰ׺�����Ķ�ַ���ڳ���
       �ձ�װ��ʱDS��ES��ֵ��
    5. פ����һ�����int 21h/AH=31h, COM��ʽ����Ҳ�ɲ���int 27h��DX����
       ����16��������һ��פ�����жϷ�����̣�Interrupt Service Routine)
       ��ʱ���ἤ�һ����˵����дISRӦע���������⣺
      ��1��ISRӦ����ڴ��������еļĴ������ڷ���ǰ�ٻָ���
       (2) ���ISRҪ�õ������һЩ���������������趨DS��ES�ȶμĴ�����
           ���������Լ���DS��ES����פ��ʱ��ֵ������COM��ʽ�ĳ���ֻҪ
           �򵥵ز������µ�ָ��Ϳ����ˣ�
           push cs
           pop  ds
           push cs
           pop  es
      ��3��ISRӦ�������DOS�����������DOS�����루reentrance����
           �Ӷ����ϵͳ����
end of comment #


;==============Դ����ʼ========================
;������: autotime.asm
;����:   Black White
;MASM5.1 ���벽��:
;   MASM autotime;
;   LINK autotime;
;   EXE2BIN autotime autotime.com
;TASM3.1 ���벽��:
;   TASM autotime
;   TLINK/t autotime
;
code segment
assume cs:code,ds:code
org 100h
main:
    jmp initialize
;--------------Int_1ch---------------------------
int_1ch:
    inc cs:count	;when int occurs, only CS is determined
    cmp cs:count,18
    jb  goto_old_1ch
    mov cs:count,0
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push ds
    push es
    ;
    push cs
    pop  ds; DS=CS
    push cs
    pop  es; ES=CS
    mov al,4			;want to get value from 4th unit of CMOS
    out 70h,al; index hour  ;using IO to communicate with CMOS
    in al,71h ; AL=hour(e.g. 08h means 8 am., 15h means 3 pm.) 
    call convert
    mov word ptr current_time[0],ax  ;if want to modify its value add ; mov al, xx out 71h, al
    mov al,2
    out 70h,al; index minute	;using BCD code ,eg,al=19h means decimal 19,binary coded decimal
    in  al,71h; AL=minute	;if using 13h represent 19,you have to use division
    call convert
    mov word ptr current_time[3],ax
    mov al,0  ; index second
    out 70h,al
    in  al,71h; AL=second
    call convert
    mov word ptr current_time[6],ax
    call disp_time
    pop es
    pop ds
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
goto_old_1ch:
    jmp dword ptr cs:old_1ch
old_1ch dw 0,0; old vector of int_1ch
;---------Disp_time--------------
;Output:display current time
;       at (X0,Y0)
X0 = 80-current_time_str_len
Y0 = 0
disp_time proc near
    push ax
    push cx
    push dx
    push si
    push di
    push ds
    push es
    mov ax,0B800h
    mov es,ax; ES=video buf seg
    mov ax,Y0
    mov cx,80*2
    mul cx   ; DX:AX=Y0*(80*2)
    mov dx,X0
    add dx,dx; DX=X0*2
    add ax,dx; AX=Y0*(80*2)+(X0*2)
    mov di,ax; ES:DI--->video buffer
    push cs
    pop  ds
    mov  si,offset current_time; DS:SI--->current_time
    mov  cx,current_time_str_len
    cld
    mov ah,17h; color=blue/white
disp_next_char:
    lodsb
    stosw
    loop disp_next_char
    pop es
    pop ds
    pop di
    pop si
    pop dx
    pop cx
    pop ax
    ret
disp_time endp
;---------End of Disp_time-------

;---------Convert----------------
;Input:AL=hour or minute or second
;      format:e.g. hour   15h means 3 pm.
;                  second 56h means 56s
;Output: (e.g. AL=56h)
;     AL='5'
;     AH='6'
convert proc near
    push cx
    mov ah,al ; e.g. assume AL=56h
    and ah,0Fh; AH=06h
    mov cl,4
    shr al,cl ; AL=05h
    add ax,'00' ;ax, 3030h
    pop cx
    ret
convert endp
;---------End of Convert---------
current_time db '00:00:00'
current_time_str_len = $-offset current_time ; 8 bytes
                                             ; $ is a macro which
                                             ; means current offset
count db 0   ; increment it on every interrupt
             ; When it reaches 18(about 1 second elapsed),
             ; it's time to display the time.
flag  db 'BW'; resident flag
;-------------------End of Int_1ch---------------------------------

initialize:
    cmp byte ptr ds:[80h],2; command line parameter's len >= 2 ?
    jae check_command_line
    jmp no_parameter
check_command_line:
    mov ax,ds:[81h]; AX=first two bytes of command line
    xchg ah,al
    and al,0DFh; 'u'--->'U'
    cmp ax,'/U'
    jne no_parameter
uninstall:
    mov ax,351ch
    int 21h
    cmp word ptr es:[flag],'WB'
    jne not_resident
    mov ah,49h
    int 21h   ; free memory kept for AUTOTIME
    lds dx,dword ptr es:[old_1ch]; DS:DX=old vector of int_1ch
    mov ax,251ch
    int 21h   ; recover vector of int_1ch
    push cs
    pop  ds
    mov ah,9
    mov dx,offset uninstalled_msg
    int 21h
    jmp exit
uninstalled_msg db 'AUTOTIME is uninstalled!',0dh,0ah,'$'
not_resident:
    mov ah,9
    mov dx,offset not_resident_msg
    int 21h
    jmp exit
not_resident_msg db 'Not resident!',7,0dh,0ah,'$'
no_parameter:
    mov ax,351ch
    int 21h     ; get vector of int_1ch--->ES:BX
    cmp word ptr es:[flag],'WB'
    jne not_installed
    jmp already_resident
not_installed:
    mov old_1ch[0],bx
    mov old_1ch[2],es; save old vector of int_1ch
    mov es,ds:[002Ch]; ES=environment block seg
    mov ah,49h
    int 21h   ; free environment block
    mov ax,251ch
    mov dx,offset int_1ch
    int 21h   ; set vector of int_1ch
install:
    mov ah,9
    mov dx,offset install_msg
    int 21h
    mov dx,offset initialize
    add dx,0Fh; include remnant bytes ;let it be divisible by 16(10h)
    mov cl,4
    shr dx,cl ; DX=program's paragraph size to keep resident
    mov ah,31h;it only need total length/16 (ah) 1300
    int 21h   ; keep resident
install_msg db 'AUTOTIME version 1.0',0dh,0ah
            db 'Copyright Black White. Nov 18,1997',0dh,0ah,'$'
already_resident:
    mov ah,9
    mov dx,offset already_resident_msg
    int 21h
    jmp exit
already_resident_msg db 'Already resident!',0dh,0ah
                     db 'Use parameter /u to uninstall!',7,0dh,0ah,'$'
exit:
    mov ah,4ch
    int 21h
code ends
end main
;==============Դ�������========================

