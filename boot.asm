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

; ------------------------------------------
;           BOOT of AF-IA32 kernel
; ------------------------------------------

org	07c00h						;Boot starts from 7c00

mov	bp, Message				; ES:BP = the address of the string
mov	cx, 58						; CX = the length of the string
mov	ax, 01301h				; AH = 13,  AL = 01h
mov	bx, 000ch					; Page is 0(BH = 0) Color is red,highlight (BL = 0Ch)
mov	dl, 0
int	10h								; BIOS 10h interrupt.

jmp	$									; Keep the word on the screen.

Message: db "A Framework IA32 Kernel [Copyright (C) 2012 LeZiZi Studio]"

times 510-($-$$) db	0		; Fill the extra space of binary. Make it as long as 510 bytes.
dw 0xaa55								; The last two bytes (0xaa55) make BIOS recognize the BOOT file.