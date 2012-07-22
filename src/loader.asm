; Copyright (C) 2012 LeZiZi Studio
; 
;   Licensed under the Apache License, Version 2.0 (the "License");
;   you may not use this file except in compliance with the License.
;   You may obtain a copy of the License at
;
;       http://www.apache.org/licenses/LICENSE-2.0
;
;   Unless required by applicable law or agreed to in writing, software
;   distributed under the License is distributed on an "AS IS" BASIS,
;   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;   See the License for the specific language governing permissions and
;   limitations under the License.

; ==========================================
;          Loader of AF-IA32 kernel
; ==========================================

; ------------------------------------------
;             Memory assignment
;	0x0000	Boot
;	0x1000	Loader
;	0x2000	Goodbye Screen
;	0x3000	Font
;	0x3800	Title Bar
;	0x4000	Cursors and bitmaps
;	0x5000	登陆图 1
;	0x6000	登陆图 2
;	0x7000	应用程序图标
;	0x8000	系统数据及堆栈区
;	0x9000	binary of kernel
;	全局变量不在程序中分配空间，只分配它的指针
; ------------------------------------------

[BITS 16]           
[ORG 0x0000]
	jmp					main
	BOOT_DRIVER:		db  0
	VESA:				times 512 db 0		;assigned for VESA information

; ------------------------------------------
;         Temporary GDT Descriptor
; ------------------------------------------
	;;总共定义三个段，一个空段由 intel 保留，一个代码段，一个数据段
	gdt_descriptor:
		dw				0xffff				;GDT 表的大小
		dw				gdt					;GDT 表的位置
		dw				0x0001				;此处的 1 是由于 SETUP_SEG 段基址为 0x1000
	gdt:
		gdt_null:
			dw			0x0000
			dw			0x0000
			dw			0x0000
			dw			0x0000
		gdt_kernel_code:					;内核代码段,把整个内存空间做为一个整段
			dw			0xffff
			dw			0x0000
			dw			0x9a00				;非一致代码段
			dw			0x00cf
		gdt_system_data:
			dw			0xffff
			dw			0x0000
			dw			0x9200				;非一致数据段
			dw			0x00cf

; ------------------------------------------
;   取得启动驱动器号,此号由 boot 程序保留
; ------------------------------------------
get_boot_driver:														
	push				fs
	
	mov					ax , 0x8000
	mov					fs , ax
	mov					al , [ fs:0 ]
	mov					[ BOOT_DRIVER ] ,al

	pop					fs
	ret

; ------------------------------------------
;            Configure VESA mode
; ------------------------------------------
set_vesa_model:
	push				es
	push				fs
	
	; set resolution to 800*600
	mov					ax , 0x4f02
	mov					bx , 0x4114			;;800 * 600 ( 5:6:5 )

	int					0x10

	; get the linear address of video memory, and save it to [ es:VESA + 40 ]
	mov					bx , 0x1000
	mov					es , bx  
	mov					bx , 0x8000
	mov					fs , bx
	mov					di , VESA
	mov					ax , 0x4f01
	mov					cx , 0x114
	int					0x10
	mov					eax , [ es:VESA + 40 ]
	mov					[ fs:1 ] , eax
	
	pop					es
	pop					fs
	
	ret

; ------------------------------------------
;				   Read Kernel
; ------------------------------------------
read_kernel:
	push				es
	.read_kernel_1:
		mov				ax , 0x9000
		mov				es , ax
		mov				bx , 0
		mov				ah , 2
		mov				dl , [ BOOT_DRIVER ] 
		mov				dh , 0 
		mov				ch , 0 
		mov				cl , 4
		mov				al , 15

		int				0x13
		jc				 .read_kernel_1

		add				bx , 15 * 512
	.read_kernel_2:
		mov				dh , 1
		mov				dl , [ BOOT_DRIVER ]
		mov				ch , 0
		mov				cl , 1
		mov				ah , 2
		mov				al , 18
		int				0x13
		jc				 .read_kernel_2
		pop				es
		ret

; ------------------------------------------
;     Universal Floppy Reader (less 64k)
;;存入的段起始位置放在 ax 中
;;欲读入的磁道总数放在 di 中
;;起始面放入 dh 中
;;起始磁道放入 ch 中
; ------------------------------------------
read_disk_length_less_64K:
	push				es
	mov					es , ax
	mov					bx , 0
	.read_disk_length_less_64K_1:
		mov				ah , 2					;功能号 2 表示读磁盘扇区
		mov				dl , [ BOOT_DRIVER ]	;驱动器号
		mov				cl , 1					;启始扇区1扇区
		mov				al , 18					;读入扇区数
		int				0x13  
		jc				 .read_disk_length_less_64K_1
		
		dec				di
		cmp				di , 0
		je				 .read_disk_length_less_64K_end
		
		add				bx , 18 * 512
		xor				dh , 1
		cmp				dh , 0
		jne				.read_disk_length_less_64K_1
		inc				ch
		jmp				.read_disk_length_less_64K_1
		
	.read_disk_length_less_64K_end:
		pop				es
		ret

; ------------------------------------------
;				Read Resources 
; ------------------------------------------
read_logout_bmp:  
	mov					ax , 0x2000				;0x2000:0000
	mov					di , 6					;1.13 , 0.14 , 1.14 , 0.15 , 1.15 , 0.16
	mov					dh , 1
	mov					ch , 13
	call				read_disk_length_less_64K  
	ret
read_font_lib:
	mov					ax , 0x3000
	mov					di , 3					;0.20 , 1.20 , 0.21
	mov					dh , 0
	mov					ch , 20
	call				read_disk_length_less_64K
	ret
read_top_bmp:
	mov					ax , 0x3800
	mov					di , 2										    
	mov					dh , 1
	mov					ch , 16
	call				read_disk_length_less_64K
	ret
read_cursor:
	mov					ax , 0x4000				;0x4000:0000
	mov					di , 4					;0.9 , 1.9 , 0.10 , 1.10
	mov					dh , 0
	mov					ch , 9
	call				read_disk_length_less_64K  
	ret
read_login_bmp1:
	mov					ax , 0x5000				;0x5000:0000
	mov					di , 7					;0.2 , 1.2 , 0.3 , 1.3 , 0.4 , 1.4 , 0.5
	mov					dh , 0
	mov					ch , 2
	call				read_disk_length_less_64K 
	ret
read_login_bmp2:
	mov					ax , 0x6000				;0x6000:0000
	mov					di , 7					;1.5 , 0.6 , 1.6 , 0.7 , 1.7 , 0.8 , 1.8
	mov					dh , 1
	mov					ch , 5
	call				read_disk_length_less_64K
	ret
read_application_bmp:
	mov					ax , 0x7000				;0x7000:0000
	mov					di , 4					;1.11 , 0.12 , 1.12 , 0.13
	mov					dh , 1
	mov					ch , 11
	call				read_disk_length_less_64K 
	ret

; ------------------------------------------
;				    Loader Main
; ------------------------------------------
main:  
	;初始化寄存器，因为 Bios 中断及 call 会用到堆栈或 ss 寄存器
	;在 CPU 启动或复位时是由 BIOS 初始化的，而现在进行了段转移，需要我们重新设置
	mov					ax , 0x1000
	mov					ds , ax  
	mov					ax , 0x8000
	mov					ss , ax
	mov					sp , 0xffff
	; get boot driver
	call				get_boot_driver
	; configure VESA mode
	call				set_vesa_model
	; read bitmaps
	call				read_login_bmp1
	call				read_login_bmp2
	call				read_cursor
	call				read_application_bmp
	call				read_logout_bmp
	call				read_top_bmp
	; read fonts
	call				read_font_lib
	; read kernel binary
	call				read_kernel
	; load GDT discriptor  														          
	lgdt				[ gdt_descriptor ]
	; close interrupt
	cli
	; change to protected mode
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax
	; jump to 32-bits code section
	jmp dword         0x8:0x90000
	
times 1024-($-$$) db 0