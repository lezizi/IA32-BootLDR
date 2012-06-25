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
;       Protected Mode MACROs & CONSTs
; ==========================================

; ------------------------------------------
;               CONST 'Selector'
; usage: SA_(Selector Attribute)
; ------------------------------------------
SA_RPL0		EQU	0	; Requested Privilege Level 0
SA_RPL1		EQU	1	; Requested Privilege Level 1
SA_RPL2		EQU	2	; Requested Privilege Level 2
SA_RPL3		EQU	3	; Requested Privilege Level 3

SA_TIG		EQU	0	; GDT Indicator (to read the Descriptor From GDT)
SA_TIL		EQU	4	; LDT Indicator (to read the Descriptor From LDT)

; ------------------------------------------
;              MACRO 'Descriptor'
;
; usage: Descriptor Base, Limit, Attr
;        Base:  dd
;        Limit: dd (low 20 bits available)
;        Attr:  dw (lower 4 bits of higher byte are always 0)
; ------------------------------------------

%macro Descriptor 3
	dw	%2 & 0FFFFh
	dw	%1 & 0FFFFh
	db	(%1 >> 16) & 0FFh
	dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)	
	db	(%1 >> 24) & 0FFh
%endmacro ;

; ------------------------------------------
;              MACRO 'Gate'
;
; usage: Gate Selector, Offset, DCount, Attr
;        Selector:  dw
;        Offset:    dd
;        DCount:    db
;        Attr:      db
; ------------------------------------------

%macro Gate 4
	dw	(%2 & 0FFFFh)
	dw	%1
	dw	(%3 & 1Fh) | ((%4 << 8) & 0FF00h)
	dw	((%2 >> 16) & 0FFFFh)
%endmacro ; 

; ------------------------------------------
;        Storage Section Descriptors
; ------------------------------------------

DA_DR		EQU	90h	; read
DA_DRW		EQU	92h	; read/write
DA_DRWA		EQU	93h	; read/write/access
DA_C		EQU	98h	; execute
DA_CR		EQU	9Ah	; execute/read
DA_CCO		EQU	9Ch	; execute (coordinate)
DA_CCOR		EQU	9Eh	; execute/read (coordinate)

; ------------------------------------------
;        System Section Descriptors
; ------------------------------------------
DA_LDT		EQU	  82h	; LDT
DA_TaskGate	EQU	  85h	; TASK gate
DA_386TSS	EQU	  89h	; 386 Task states
DA_386CGate	EQU	  8Ch	; 386 CALL gate
DA_386IGate	EQU	  8Eh	; 386 INT gate
DA_386TGate	EQU	  8Fh	; 386 TRAP gate

; ------------------------------------------
;              other CONSTs
; ------------------------------------------

DA_32		EQU	4000h	; 32 bits section
DA_DPL0		EQU	  00h	; DPL = 0
DA_DPL1		EQU	  20h	; DPL = 1
DA_DPL2		EQU	  40h	; DPL = 2
DA_DPL3		EQU	  60h	; DPL = 3
