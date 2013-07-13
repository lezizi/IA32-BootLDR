/*
Copyright (C) 2013 LeZiZi Studio
 
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
                   Kernel
 ==========================================

*/

#include "system.h"
#include "vesa.h"
#include "mouse.h"
#include "interrupt.h"
#include "io.h"
#include "message.h"
#include "graphic.h"
#include "ata.h"

struct message_message_queue_struct kernel_message_queue ;

enum kernel_kernel_state_enum{ KERNEL_WAIT_USER_LOGIN , KERNEL_USER_LOGED_IN , KERNEL_window } ;
static enum kernel_kernel_state_enum kernel_kernel_state = KERNEL_WAIT_USER_LOGIN ;  
static enum mouse_type_enum kernel_mouse_type = MOUSE_NORMAL_MOUSE ;

static int kernel_mouse_x_position = 444 ;
static int kernel_mouse_y_position = 300 ;

static int application_picture_x = 30;
static int application_picture_y = 30;

static int window_x = 200;
static int window_y = 50;

static unsigned short *old_picture ;
static unsigned short *window_background ;
static unsigned short *window_buffer ;
static unsigned short *ata_buffer ;  //磁盘缓冲

static int old_picture_width ;
static int old_picture_height ;

static int input_pos=0;
static int input_offset_x=0;
static int input_offset_y=0;

const int window_border = 25;
const int window_width = 360;
const int window_height = 380;

// function forward declarations
void kernel_draw_login_form();
void input_handle( char ch );
void kernel_login();
int kernel_does_point_in_rect( int x , int y , int x1 , int y1 , int x2 , int y2 );
void kernel_new_window();
void kernel_close_window();
void kernel_logout();
void notepad_print_ata_buffer(unsigned short color);

/****************************************
*  implementation                       *
*****************************************/

void kernel_main()
{
	// 操作系统初始化
	system_init() ;  //0x903ae

	old_picture = ( unsigned short * )0x100000 ;		// 越过前面的只读内存区
	window_background = ( unsigned short * )0x200000 ;	// 越过前面的只读内存区
	window_buffer = ( unsigned short * )0x300000 ;		// 越过前面的只读内存区
	ata_buffer= ( unsigned short * )0x400000 ;		// 越过前面的只读内存区

	kernel_mouse_x_position = 444 ;
	kernel_mouse_y_position = 300 ;

	application_picture_x = 30;
	application_picture_y = 30;

	window_x = 200;
	window_y = 50;

	// 画登陆界面 
	kernel_draw_login_form() ; // 0x903b3

	// 清空键盘缓冲区
	io_read_from_io_port( 0x60 ) ;

	// 初始化消息队列
	message_init_message_queue( &kernel_message_queue ) ;

	
	struct message_message_struct message ;

	kernel_kernel_state = KERNEL_WAIT_USER_LOGIN ;
	
	// 进入内核主消息循环
	for( ;; ){
		if( !message_get_message( &kernel_message_queue , &message ) ){
			continue ;
		}
		if( message.message_type == MESSAGE_SHUTDOWN_COMPUTER ){
			break ;
		}
		switch( message.message_type ){
			case MESSAGE_KEYBOARD_MESSAGE : // 键盘消息
				// 检测是不是正在窗口，否则不处理
				if( kernel_kernel_state == KERNEL_window ){
					// 调用窗口处理函数进行处理
					input_handle( message.key ) ;
				}
				break ;
			case MESSAGE_MOUSE_MESSAGE : // mouse 消息
				if( kernel_kernel_state == KERNEL_WAIT_USER_LOGIN ){
					// 检测是否需要登陆
					if( message.dose_left_button_down && kernel_mouse_type == MOUSE_OVER_MOUSE ){
						kernel_login() ;
					}
					else if( kernel_mouse_x_position != message.x_position || kernel_mouse_y_position != message.y_position ){
						// 检测是否需要改变 mouse 形状
						if( kernel_does_point_in_rect( message.x_position , message.y_position , 150 , 271 , 431 , 375 ) ){
							kernel_mouse_type = MOUSE_OVER_MOUSE ;
						}
						else{
							kernel_mouse_type = MOUSE_NORMAL_MOUSE ;
						}
					}
				}
				else if( kernel_kernel_state == KERNEL_USER_LOGED_IN || kernel_kernel_state == KERNEL_window ){
					// 检测是否要启动窗口，如果窗口已启动，则不再启动，但如果在窗口状态下，则应用程序图标失效
					int ibool2 ;
					if( kernel_kernel_state == KERNEL_window ){
						// 在窗口状态下，应用程序图标失效
						ibool2 = 0 ;
					}
					else{
						int x_temp = message.x_position ;
						if( kernel_mouse_type == MOUSE_OVER_MOUSE ){
							x_temp += 8 ;
						}
						ibool2 = kernel_does_point_in_rect( x_temp , message.y_position , application_picture_x , application_picture_y , application_picture_x + 128 , application_picture_y + 128 ) ;
					}

					if( ibool2 && message.dose_left_button_down && kernel_mouse_type == MOUSE_OVER_MOUSE ){
						kernel_new_window();
					}

					// 检测是否要关闭窗口，如果不是窗口状态，失效
					int ibool3 ;
					if( kernel_kernel_state == KERNEL_window ){   
						int x_temp = message.x_position ;
						if( kernel_mouse_type == MOUSE_OVER_MOUSE ){
							x_temp += 8 ;
						}
						ibool3 = kernel_does_point_in_rect( x_temp , message.y_position , window_x + 338 , window_y + 2 , window_x + 354 , window_y + 16 ) ;
					}
					else{
						ibool3 = 0 ;
					}
					if( ibool3 && message.dose_left_button_down && kernel_mouse_type == MOUSE_OVER_MOUSE ){
						// 关闭窗口，重画桌面
						kernel_close_window() ;
						break ;
					}
					
					// 检测是否需要移动
					// 获得 x , y  的移动量
					int imove_x = kernel_mouse_x_position - message.x_position ;
					int imove_y = kernel_mouse_y_position - message.y_position ;
					
					// 检测是否需要移动应用程序图标
					if( ibool2 && message.dose_right_button_down && kernel_mouse_type == MOUSE_DOWN_MOUSE && ( imove_x || imove_y ) ){
						// 表示要移动图标先恢复原图片
						vesa_copy_picture_to_screen( application_picture_x , application_picture_y , old_picture , 128 + 32 , 128 + 32 ) ;
						// 计算新位置
						application_picture_x -= imove_x ;
						application_picture_y -= imove_y ;
						// 再保存目标地址图片
						vesa_copy_picture_from_screen( application_picture_x  , application_picture_y , old_picture , 128 + 32 , 128 + 32 ) ;
						// 再在新位置画出应用程序图片
						unsigned short color = vesa_compound_rgb( 255 , 255 , 255 ) ;
						vesa_show_bmp_picture( application_picture_x , application_picture_y , ( void * )0x70000 , color , 1 ) ;
						// mouse 由下面画，这里先保存 mouse 原位置的图
						mouse_save_picture( kernel_mouse_x_position , kernel_mouse_y_position ) ;
					}
					
					// 检测是否需要移动窗口，如果不是在窗口状态下，则失效，如果 ibool3 有效，则也失效，因为它同 ibool3 互不相容
					int ibool4 ;
					if( kernel_kernel_state != KERNEL_window || ibool3 ){
						ibool4 = 0 ;
					}
					else{
						int x_temp = message.x_position ;
						if( kernel_mouse_type == MOUSE_OVER_MOUSE ){
							x_temp += 8 ;
						}
						ibool4 = kernel_does_point_in_rect( x_temp , message.y_position , window_x , window_y , window_x + 356  , window_y + 18 ) ;
					}
					if( ibool4 && kernel_mouse_type == MOUSE_DOWN_MOUSE && ( imove_x || imove_y ) ){
						// 恢复窗口原位置的图片
						vesa_copy_picture_to_screen( window_x , window_y , window_background ,window_width, window_height) ;
						
						// 更新窗口位置
						window_x -= imove_x ;
						window_y -= imove_y ;
						// avoid overflow
						if (window_x<0) window_x=0;
						if (window_y<0) window_y=0;
						if (window_x+window_width>800) window_x=800-(window_width);
						if (window_y+window_height>600) window_y=600-(window_height);

						// 对应用程序的当前背景进行快照
						vesa_copy_picture_from_screen( window_x  , window_y , window_background , window_width ,window_height) ;

						// draw buffer to screen with mask DISABLED
						vesa_draw_buffer(window_buffer,window_width,window_height,window_x,window_y ,0 , 0 );
					}
					
					// 检测是否需要退出
					// 由于这个是任务栏上的，因此，当其余有效时，其不可能有效
					int ibool1 ;
					if( ibool2 || ibool3 || ibool4 ){
						ibool1 = 0 ;
					}
					else{
						ibool1 = kernel_does_point_in_rect( message.x_position , message.y_position , 4 , 570 , 38 , 593 ) ;
					}
					
					if( ibool1 && message.dose_left_button_down && kernel_mouse_type == MOUSE_OVER_MOUSE ){
						kernel_logout() ;
						break ;
					}
					
					// 检测是否需要改变 mouse 形状
					// 有上可知 ibool1 , ibool2 , ibool3 , ibool4 在同一时刻，只可能有一个有效
					if( ibool1 ){
						kernel_mouse_type = MOUSE_OVER_MOUSE ;
					}
					else if( ibool2 ){
						if( message.dose_right_button_down ){
							kernel_mouse_type = MOUSE_DOWN_MOUSE ;
						}
						else{
							kernel_mouse_type = MOUSE_OVER_MOUSE ;
						}
					}
					else if( ibool3 ){
						kernel_mouse_type = MOUSE_OVER_MOUSE ;
					}
					else if( ibool4 ){
						if( message.dose_right_button_down ){
							kernel_mouse_type = MOUSE_DOWN_MOUSE ;
						}
						else{
							kernel_mouse_type = MOUSE_OVER_MOUSE ;
						}
					}
					else{
						kernel_mouse_type = MOUSE_NORMAL_MOUSE ;
					}
					}
					// 重画 mouse
					if ((kernel_mouse_x_position != message.x_position)||(kernel_mouse_y_position != message.y_position)) {
						mouse_move_mouse( kernel_mouse_x_position , kernel_mouse_y_position , message.x_position , message.y_position , kernel_mouse_type ) ;
						kernel_mouse_x_position = message.x_position ;
						kernel_mouse_y_position = message.y_position ;
					}
					break ;
				}
		}
}


int kernel_does_point_in_rect( int x , int y , int x1 , int y1 , int x2 , int y2 )
{
	if( x >= x1 && x <= x2 ){
		if( y >= y1 && y <= y2 ){
			return 1;
		}
	}
	return 0;
}

void kernel_login() 
{
	// 用白色清屏
	unsigned short color = vesa_compound_rgb( 255 , 255 , 255 ) ;
	vesa_clean_screen( color ) ;
	
	// 保存应用程序地区原图片，供移动使用
	vesa_copy_picture_from_screen( application_picture_x , application_picture_y , old_picture , 128 + 32 , 128 + 32 ) ; // 为原图长度加 mouse 长度
	
	// 显示应用程序图标
	// TODO:建议改用 buffer
	vesa_show_bmp_picture( application_picture_x , application_picture_y , ( void * )0x70000 , color , 1 ) ;

	// 显示英文
	color = vesa_compound_rgb( 0 , 0 , 255 ) ;
	const char string[]  = "A Framework IA32 [Copyright (C) 2013 LeZiZi Studio]"; 
	// when using this, -fno-stack-protector must be included in the Makefile
	for( int i = 0 ; i < 52 ; ++i ){
		vesa_print_english(383+i*8 , 552 , string[i] , color ) ;
	}
	
	// 画底边
	color = vesa_compound_rgb( 187 , 250 , 131 ) ;
	for( int i = 0 ; i < 30 ; i += 2 ){
		vesa_draw_x_line( 570 + i , 0 , 799 , color ) ;
	}

	// 先显示"关机"
	color = vesa_compound_rgb( 255 , 0 , 255 ) ;
	for( int i = 0 ; i < 2 ; ++i ){
		vesa_print_chinese( 4 + i * 18 , 577 , i , color ) ;
	}
	// 再显示"乐滋滋工作室" 
	color = vesa_compound_rgb( 0 , 0 , 255 ) ;
	for( int i = 2 ; i < 8 ; ++i ){
		vesa_print_chinese( 680 + ( i - 2 )* 18 , 577 , i , color ) ;
	}
	
	// 显示 mouse
	mouse_show_mouse( kernel_mouse_x_position , kernel_mouse_y_position , MOUSE_NORMAL_MOUSE ) ;
	
	// 更改 mouse 状态
	kernel_mouse_type = MOUSE_NORMAL_MOUSE ;
	
	// 更改系统状态
	kernel_kernel_state = KERNEL_USER_LOGED_IN ;
}

// 完成用户退出
void kernel_logout() 
{
	// 用白色清屏
	unsigned short color = vesa_compound_rgb( 255 , 255 , 255 ) ;
	vesa_clean_screen( color ) ;  
	
	// 显示退出图片( 246 * 103 )
	vesa_show_bmp_picture( 277 , 248 , ( void * )0x20000 , 0 , 0 ) ;

	// 关中断
	interrupt_close_interrupt() ;

	// 清空消息队列
	message_init_message_queue( &kernel_message_queue ) ;

	// 加入退出消息
	struct message_message_struct message ;
	message.message_type = MESSAGE_SHUTDOWN_COMPUTER ;
	message_put_message( &kernel_message_queue , message ) ;
	
	io_write_to_io_port(0x64, 0xFE);
}

void kernel_new_window()
{

	// 画窗口
	// 先恢复 mouse 中保存的图
	mouse_restore_picture( kernel_mouse_x_position , kernel_mouse_y_position ) ;

	// 对应用程序的当前背景进行快照
	vesa_copy_picture_from_screen( window_x  , window_y , window_background , window_width ,window_height) ;

	kernel_kernel_state = KERNEL_window ;

	// initail the buffer
	unsigned short color = vesa_compound_rgb(255,255,255 ) ;
		
	buffer_clean(window_buffer,window_width,window_height,color);
		
	// draw white background
	color = vesa_compound_rgb( 255 , 255 , 255 ) ;
	buffer_draw_rect(window_buffer,window_width,window_height, 1, 1 , window_width  , window_height, color , 1 ) ;

	// draw title bar
	buffer_load_bitmap_16(window_buffer,window_width,window_height,1,3,( void * )0x38000,color,1);

	// draw the rectangle
	color = vesa_compound_rgb( 0 , 0 , 255 ) ;
	buffer_draw_rect(window_buffer,window_width,window_height, 1 , 1 , window_width-1 , window_height-1 , color , 0 ) ;
	buffer_draw_rect(window_buffer,window_width,window_height, 2 , 2 , window_width-2 , window_height-2 , color , 0 ) ;
	
	// reset keboard input offset
	input_offset_x=0;
	input_offset_y=0;

	// read form hard disk
	notepad_print_ata_buffer(color);

	// draw buffer to screen with mask DISABLED
	vesa_draw_buffer(window_buffer,window_width,window_height,window_x,window_y ,color , 0 );
	
	// 显示 mouse , 因为可能在画棋盘时盖住了 mouse
	mouse_show_mouse( kernel_mouse_x_position , kernel_mouse_y_position , kernel_mouse_type ) ;

}

void kernel_close_window()
{
	//回写数据
	ata_buffer[input_pos]=0;// set the last char to 0, indicating EOF
	ata_open(0x1f0);
	ata_write( 0x1f0,  0x100, ata_buffer);
	//重画桌面
	kernel_login() ;
}

void notepad_print_ata_buffer(unsigned short color)
{
	ata_open(0x1f0);
	ata_read(0x1f0,  0x100, ata_buffer);
	int i = 0 ;
	while(ata_buffer[i]!=0){
		if (ata_buffer[i] == 13){ // enter
			input_offset_x = 0;
			input_offset_y += 18;
		}
		else
		{
			buffer_print_english(window_buffer,window_width,window_height,input_offset_x+window_border ,input_offset_y+window_border , ata_buffer[i] , color ) ;
			input_offset_x+=8;
		};
		i++;
	};
	input_pos = i;
}

void input_handle( char ch )
{
	if( ch == 27 ){ // esc 
		kernel_close_window() ;
		return ;
	}
	else if (ch == 8){ // backspace
		// fill it with a 8*16 white rectangle
		input_offset_x -= 8;
		if (input_offset_x<0) {
			input_offset_x = 296;
			input_offset_y -= 18;
		};

		unsigned short color = vesa_compound_rgb( 255 , 255 , 255 ) ;
		buffer_draw_rect(window_buffer,window_width,window_height,input_offset_x+window_border ,input_offset_y+window_border ,input_offset_x +window_border+ 8,window_y+ input_offset_y+window_border + 16, color,1 ) ;

		// move the input offset

		input_pos--;
		if (input_pos<0) {
			input_pos=0;//prevent overflow
			input_offset_x = 0;
			input_offset_y = 0;
		};
	}
	else if (ch == 13){ // enter
		int input_pos_old = input_pos;
		input_pos+=(296-input_offset_x)/8;
		for (int filli=input_pos_old;filli<input_pos;filli++)
		{
			ata_buffer[filli]='.';
		};
		input_offset_x = 0;
		input_offset_y += 18;
		ata_buffer[input_pos]=ch;
		input_pos++;
	}
	else {
		unsigned short color = vesa_compound_rgb( 0 , 0 , 255 ) ;
		buffer_print_english(window_buffer,window_width,window_height,input_offset_x+window_border ,input_offset_y+window_border , ch , color ) ;
		input_offset_x += 8;
		if (input_offset_x>296) {
			input_offset_x = 0;
			input_offset_y += 18;
			ata_buffer[input_pos]=13;
			input_pos++;
		};
		//写到缓冲区
		ata_buffer[input_pos]=ch;
		input_pos++;
	}
	// draw buffer to screen with mask DISABLED
	vesa_draw_buffer(window_buffer,window_width,window_height,window_x,window_y,0,0);
}

// 画登陆界面
void kernel_draw_login_form() //0x90020
{
	//ata_open(0x10e0);//10e0
	// 用白色清屏
	unsigned short color = vesa_compound_rgb( 255 , 255 , 255 ) ;
	vesa_clean_screen( color ) ;
	// 显示第一幅图，所占矩形 ( 150 , 271 )-( 431 , 375 )
	vesa_show_bmp_picture( 150 , 271 , ( void * )0x50000 , 0 , 0 ) ;
	// 显示第二幅图
	color = vesa_compound_rgb( 0 , 255 , 0 ) ;
	vesa_show_bmp_picture( 457 , 225 , ( void * )0x60000 , color , 1 ) ;
	// 显示 mouse
	mouse_show_mouse( kernel_mouse_x_position , kernel_mouse_y_position , kernel_mouse_type ) ;  //0x900a3
}
