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
;              Kernel (wrapper)
; ==========================================

[BITS 32]
[GLOBAL main]
[EXTERN kernel_main]
main:
	mov				ax , 0x10
	mov				ds , ax
	mov				es , ax
	mov				fs , ax
	mov				gs , ax
	mov				ss , ax
	mov				esp , 0x8ffff
	call			kernel_main
	jmp				$
  