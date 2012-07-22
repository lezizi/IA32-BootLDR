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
;           BOOT of AF-IA32 kernel
; ==========================================

[BITS 16]
[ORG 0x7C00]
jmp						main

; ------------------------------------------
;                   CONSTs
; ------------------------------------------
BOOT_SEG				equ		0x0000
SETUP_SEG				equ		0x1000
SETUP_OFFSET			equ		0x0000
SETUP_SIZE				equ		1024
TEMP_DATA_SEG			equ		0x9000

BOOT_DRIVER				db 0

; ------------------------------------------
;                 copy Loader
; ------------------------------------------
copy_loader:
	push				es
	.read_setup_1
	mov					ax,SETUP_SEG
	mov					es,ax
	mov					bx,SETUP_OFFSET
	mov					ah,2
	mov					dl,[ BOOT_DRIVER ]
	mov					ch,0
	mov					cl,2
	mov					al,SETUP_SIZE / 512
	int					0x13
	jc					.read_setup_1
	
	pop					es
	ret

; ------------------------------------------
;                 Boot main
; ------------------------------------------
main:
	mov					ax,BOOT_SEG
	mov					ds,ax
	mov					ax,TEMP_DATA_SEG
	mov					ss,ax
	mov					sp,0xffff	

	; get the number of the booting driver
	mov					[ BOOT_DRIVER ] , dl


	; copy Loader into memory
	call				copy_loader

	; open A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al

	mov					ax , 0x2401
	int					0x15

	; jump to Loader
	jmp dword			SETUP_SEG:SETUP_OFFSET

times 510-($-$$) db 0
dw 0xAA55