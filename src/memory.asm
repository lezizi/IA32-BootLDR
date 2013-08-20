; Copyright (C) 2013 LeZiZi Studio
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
;     Memory Interrupt Handler (wrapper)
; ==========================================

[BITS 32]
[GLOBAL memory_asm_handler_for_page_fault_interrupt]
[EXTERN memory_handler_for_page_fault_interrupt]
[SECTION .text]
memory_asm_handler_for_page_fault_interrupt:
	pop			ebx
	pushad
	pushf
	call				memory_handler_for_page_fault_interrupt
	popf
	popad
	iret
