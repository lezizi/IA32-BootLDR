/*
Copyright (C) 2012 LeZiZi Studio
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 ==========================================
       PS/2 Keyboard Interrupt Handler
 ==========================================

*/

#include "interrupt.h"
#include "io.h"
#include "message.h"
#include "kernel.h"

/****************************************
*  declaration                          *
*****************************************/

void keyboard_asm_handle_for_keyboard_interrupt() ;

void keyboard_handle_for_keyboard_interrupt() ;


/****************************************
*  defination                           *
*****************************************/

static const int KEYBOARD_KEYBOARD_INTERRUPT_NUMBER = 0x21 ;
static const int KEYBOARD_KEYBOARD_INTERRUPT_IRQ_NUMBER = 1 ;

static int keyboard_is_extention_key_scan_code ;

/****************************************
*  implementation                       *
*****************************************/

void keyboard_init()
{
	keyboard_is_extention_key_scan_code = 0 ;

	// 安装中断处理函数
	interrupt_install_handle_for_interrupt( KEYBOARD_KEYBOARD_INTERRUPT_NUMBER , keyboard_asm_handle_for_keyboard_interrupt ) ;
	// 设置中断屏蔽字
	interrupt_set_interrupt_mask_word( KEYBOARD_KEYBOARD_INTERRUPT_IRQ_NUMBER , 1 ) ;
}

// 键盘中断处理函数
void keyboard_handle_for_keyboard_interrupt()
{
	unsigned char ch = io_read_from_io_port( 0x60 ) ;
	// 先过滤掉特殊字节，扩展码，及弹起键

	if( ch == 0x0 || ch > 0x80){
		// 0x0 表示按键产生错误 > 0x80 表示弹起键（包括 0xe0 的扩展键）
		return ;
	}
	else{
		// 翻译键盘消息，并构造一个消息，发送给内核
		switch( ch ){
			case 0x1:
				ch = 27 ; // esc
				break;
			case 0x02:
				ch = '1' ;
				break;
			case 0x03:
				ch = '2' ;
				break;
			case 0x04:
				ch = '3' ;
				break;
			case 0x05:
				ch = '4' ;
				break;
			case 0x06:
				ch = '5' ;
				break;
			case 0x07:
				ch = '6' ;
				break;
			case 0x08:
				ch = '7' ;
				break;
			case 0x09:
				ch = '8' ;
				break;
			case 0x0A:
				ch = '9' ;
				break;
			case 0x0B:
				ch = '0' ;
				break;
			case 0x0C:
				ch = '-' ;
				break;
			case 0x0D:
				ch = '=' ;
				break;
			case 0x0E:
				ch = 8 ; // backspace 
				break;
			case 0x10:
				ch = 'q' ;
				break;
			case 0x11:
				ch = 'w' ;
				break;
			case 0x12:
				ch = 'e' ;
				break;
			case 0x13:
				ch = 'r' ;
				break;
			case 0x14: 
				ch = 't' ;
				break;
			case 0x15:
				ch = 'y' ;
				break;
			case 0x16:
				ch = 'u' ;
				break;
			case 0x17:
				ch = 'i' ;
				break;
			case 0x18:
				ch = 'o' ;
				break;
			case 0x19:
				ch = 'p' ;
				break;
			case 0x1A:
				ch = '[' ;
				break;
			case 0x1B:
				ch = ']' ;
				break;
			case 0x1C:
				ch = 13 ;
				break;
			case 0x1D:
				ch = 0 ; //向上键
				break;
			case 0x1E:
				ch = 'a' ;
				break;
			case 0x1F:
				ch = 's' ;
				break;
			case 0x20:
				ch = 'd' ;
				break;
			case 0x21:
				ch = 'f' ;
				break;
			case 0x22:
				ch = 'g' ;
				break;
			case 0x23:
				ch = 'h' ;
				break;
			case 0x24:
				ch = 'j' ;
				break;
			case 0x25:
				ch = 'k' ;
				break;
			case 0x26:
				ch = 'l' ;
				break;
			case 0x27:
				ch = ';' ;
				break;
			case 0x28:
				ch = 39 ; // '
				break;
			case 0x29:
				ch = '`' ;
				break;
			case 0x2A:
				ch = 200 ; //shift
				break;
			case 0x2B:
				ch = 92 ;// \
				break;
			case 0x2C:
				ch = 'z' ;
				break;
			case 0x2D:
				ch = 'x' ;
				break;
			case 0x2E:
				ch = 'c' ;
				break;
			case 0x2F:
				ch = 'v' ;
				break;
			case 0x30:
				ch = 'b' ;
				break;
			case 0x31:
				ch = 'n' ;
				break;
			case 0x32:
				ch = 'm' ;
				break;
			case 0x33:
				ch = ',' ;
				break;
			case 0x34:
				ch = '.' ;
				break;
			case 0x35:
				ch = '/' ;
				break;
			case 0x37:
				ch = 200 ; // right shift = shift
				break;
			case 0x38:
				ch = 201 ; // left alt = alt
				break;
			case 0x39:
				ch = ' ' ; // left alt = alt
				break;
			case 0x3A:
				ch = 202 ; // capslock
				break;
			case 0x9C:
				ch = 13 ; // capslock
				break;
			default :
				ch = 0 ;
		}
		if( ch ){
			struct message_message_struct message;
			message.message_type = MESSAGE_KEYBOARD_MESSAGE ;
			message.key = ch ;
			message_put_message( &kernel_message_queue , message ) ;
		}
	}
}