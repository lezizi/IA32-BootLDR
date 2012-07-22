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
        PS/2 Mouse Interrupt Handler
 ==========================================

*/

#include "interrupt.h"
#include "vesa.h"
#include "io.h"
#include "message.h"
#include "kernel.h"
#include "mouse.h"

/****************************************
*  declaration                          *
*****************************************/

void mouse_asm_handle_for_mouse_interrupt() ;

void mouse_handle_for_mouse_interrupt() ;

void mouse_enable_mouse() ;

static void *mouse_get_mouse_picture_addr( enum mouse_type_enum mouse_type ) ;

/****************************************
*  defination                           *
*****************************************/

static const int MOUSE_MOUSE_INTERRUPT_NUMBER = 0x2c ;
static const int MOUSE_MOUSE_INTERRUPT_IRQ_NUMBER = 12 ;

static const unsigned short mouse_old_picture[ 32 * 32 ] ;

int ttmp = 0 ;

/****************************************
*  implementation                       *
*****************************************/

void* mouse_get_mouse_picture_addr( enum mouse_type_enum mouse_type )
{
	switch( mouse_type ){
		case MOUSE_NORMAL_MOUSE:
			return (void*)0x40000 ;
		case MOUSE_OVER_MOUSE :
			return (void*)0x41000 ;
		case MOUSE_DOWN_MOUSE :
			return (void*)0x42000 ;
	}
}

void mouse_init()
{
	// enable mouse
	mouse_enable_mouse() ;
	// install interrupt handler
	interrupt_install_handle_for_interrupt( MOUSE_MOUSE_INTERRUPT_NUMBER , mouse_asm_handle_for_mouse_interrupt ) ;
	// 设置中断屏幕字
	interrupt_set_interrupt_mask_word( MOUSE_MOUSE_INTERRUPT_IRQ_NUMBER , 1 ) ;
}

void mouse_handle_for_mouse_interrupt()
{
	static int x_position = 400 ;
	static int y_position = 300 ;
	static int count = 1; 
	static int x_sign = 0 ;
	static int y_sign = 0 ;
	
	static struct message_message_struct message ;
	char ch = io_read_from_io_port( 0x60 ) ;
	switch( ++count ){
		case 1 :
			// get key press information
			message.dose_left_button_down = ch & 0x1 ;
			message.dose_right_button_down = ch & 0x2 ;

			// get the sign of the movement number, 1 for - and 0 for +.
			x_sign = ch & 0x10 ? 0xffffff00 : 0 ;
			y_sign = ch & 0x20 ? 0xffffff00 : 0 ;
			
			if ((message.dose_right_button_down>0)&&(message.dose_left_button_down>0)){
				// reset count=0 when receiving ACK
				// ACK looks as if the left key and the right key were pressed at the same time
				count=0;
			};
			
			// send message to kernel
			message.message_type = MESSAGE_MOUSE_MESSAGE ;
			message_put_message( &kernel_message_queue , message );
			break;
		case 2 :
			x_position += ( x_sign | ch ) ;
			// avoid moving mouse out of the screen, which may cause overflow
			if (x_position<0) x_position=0;
			if (x_position>800-10) x_position=800-10;
			message.x_position = x_position ;
			break ;
		case 3 :
			y_position += -( y_sign | ch ) ;
			count = 0 ;
			// avoid moving mouse out of the screen, which may cause overflow
			if (y_position<0) y_position=0;
			if (y_position>600-20) y_position=600-20;
			message.y_position = y_position ;
			break ;

	}

}

void mouse_enable_mouse()
{
	// 对 8042 键盘控制芯片进行编程，允许 mouse 接口
	io_write_to_io_port( 0x64 , 0xa8 ) ;
	// 通知 8042 下个字节的发向 0x60 的数据将发给 mouse
	io_write_to_io_port( 0x64 , 0xd4 ) ;
	// 允许 mouse 发数据
	io_write_to_io_port( 0x60 , 0xf4 ) ;
	// 通知 8042,下个字节的发向 0x60 的数据应放向 8042 的命令寄存器
	io_write_to_io_port( 0x64 , 0x60 ) ;
	// 许可键盘及 mouse 接口及中断
	io_write_to_io_port( 0x60 , 0x47 ) ;
}

void mouse_show_mouse( int x , int y , enum mouse_type_enum mouse_type )
{
	// 先保存 mouse 显示区的那副图
	mouse_save_picture( x , y ) ;
	void *mouse_picture_addr = mouse_get_mouse_picture_addr( mouse_type ) ;
	// 再画出 mouse
	vesa_show_bmp_picture( x , y , mouse_picture_addr , ( unsigned short )0x00000000 , 1 ) ;
}

void mouse_move_mouse( int x1 , int y1 , int x2 , int y2 , enum mouse_type_enum mouse_type )
{
	// 先恢复以前 mouse 所在的显示区的那副图
	mouse_restore_picture( x1 , y1 ) ;
	// 再显示 mouse
	mouse_show_mouse( x2 , y2 , mouse_type ) ;
}

void mouse_save_picture( int x , int y )
{
	// mouse 的大小固定为 32 * 32
	vesa_copy_picture_from_screen( x , y , ( unsigned short * )mouse_old_picture , 32 , 32 ) ;
}

void mouse_restore_picture( int x , int y )
{
	vesa_copy_picture_to_screen( x , y , ( unsigned short * )mouse_old_picture , 32 , 32 ) ;
}
