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
;   PS/2 Mouse Interrupt Handler (wrapper)
; ==========================================

[BITS 32]
[GLOBAL mouse_asm_handle_for_mouse_interrupt]
[EXTERN mouse_handle_for_mouse_interrupt]
[SECTION .text]
mouse_asm_handle_for_mouse_interrupt:
	pushad
	pushf
	call				mouse_handle_for_mouse_interrupt
	;send EOI message to the hardware, finishing interrupt handling
	mov					al,0x20
	out					0x20,al
	out					0xa0,al
	;popad, because the value of 'al' was changed while sending EOI message
	popf
	popad
	iret