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

%include	"macros.inc"
org	07c00h
jmp	LOADER_BEGIN
[SECTION .gdt]
; ------------------------------------------
;                    GDT
; ------------------------------------------
LABEL_GDT:			Descriptor		0,				0,						0			; empty
LABEL_DESC_CODE32:	Descriptor		0,				SegCode32Len - 1,		DA_C + DA_32; Code Section
LABEL_DESC_VIDEO:	Descriptor		0B8000h,		0ffffh,					DA_DRW		; Video Adapter Graphics Memory

GdtLen	equ	$ - LABEL_GDT	; length of GDT
GdtPtr	dw	GdtLen - 1		; GDT pointer
dd	0						; end of GDT

; ------------------------------------------
;               GDT Selectors
; ------------------------------------------
SelectorCode32		equ	LABEL_DESC_CODE32	- LABEL_GDT; relative address of  LABEL_DESC_CODE32
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT; relative address of  LABEL_DESC_VIDEO
; END of [SECTION .gdt]

; ------------------------------------------
;              16-bits jumper
; ------------------------------------------
[SECTION .s16]
[BITS 16]
LOADER_BEGIN:
; initail 32-bits Code Section Descriptors in GDT
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, LABEL_SEG_CODE32
	mov	word [LABEL_DESC_CODE32 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_CODE32 + 4], al
	mov	byte [LABEL_DESC_CODE32 + 7], ah

; prepare GdtPtr for loading GDT

	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_GDT ; [GdtPtr + 2] <- base address of GDT
	mov	dword [GdtPtr + 2], eax	

; load GDT
	lgdt	[GdtPtr]

; close int
	cli

; open A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al

; change to protected mode
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

; jump to 32-bits code section
	jmp	dword SelectorCode32:0

SegCode16Len	equ	$ - LOADER_BEGIN
; END of [SECTION .s16]

; ------------------------------------------
;              32-bits loader
; ------------------------------------------
[SECTION .s32]
[BITS	32]
LABEL_SEG_CODE32:
	mov	ax, SelectorVideo
	mov	gs, ax

	mov	edi, (80 * 11 + 79) * 2		; Line 11 , column 79 on the screen 
	mov	ah, 0Ch						; background BLACK, foreground RED
	mov	al, 'L'
	mov	[gs:edi], ax
	jmp	$

SegCode32Len	equ	$ - LABEL_SEG_CODE32
; END of [SECTION .s32]

times (512 - SegCode16Len - SegCode32Len -GdtLen -16) db	0		; Fill the extra space of binary. Make it as long as 510 bytes.
dw 0xaa55