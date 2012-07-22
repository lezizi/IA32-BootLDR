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
              Graphic Buffer
 ==========================================

*/

#include "graphic.h"
#include "bmp.h"

int buffer_is_point_in_rect( int x , int y , int x1 , int y1 , int x2 , int y2 )
{
	if( x >= x1 && x <= x2 ){
		if( y >= y1 && y <= y2 ){
			return 1;
		}
	}
	return 0;
}

unsigned short buffer_compound_rgb( unsigned char r , unsigned char g , unsigned char b )
{
	union{
		unsigned int color ;
		struct{
			unsigned char b : 5 ;
			unsigned char g : 6 ;
			unsigned char r : 5 ;

		};
	}u ;
	u.r = r % 32 ;
	u.g = g % 64 ;
	u.b = b % 32 ;
	return u.color ;
}


void buffer_draw_point(unsigned short *buffer_ADDR , unsigned int buffer_width , unsigned int buffer_height ,unsigned int x , unsigned int y , unsigned short color )
{
	// avoid overflow
	if (( x > buffer_width )||( x <0)) return ;
	if ((y > buffer_height )||(y<0)) return ;
	
	*( buffer_ADDR + y * buffer_width + x ) = color;
}

unsigned short buffer_get_point_color (unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y )
{
	return *( buffer_ADDR + y * buffer_width + x );
}


void buffer_clean (unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height ,unsigned short color )
{
	buffer_fill_rect(buffer_ADDR,buffer_width,buffer_height, 0 , 0 , buffer_width  , buffer_height  , color ) ;
}

void buffer_draw_x_line(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int y , unsigned int x1 , unsigned int x2 , unsigned short color )
{
	for( int x = x1 ; x < x2 + 1 ; ++x ){
		buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  x , y , color ) ;
	}
}

void buffer_draw_y_line(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y1 , unsigned int y2 , unsigned short color )
{
	for( int y = y1 ; y < y2 + 1 ; ++y ){
		buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  x , y , color ) ;
	}
}

void buffer_draw_rect(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x1 , unsigned int y1 , unsigned int x2 , unsigned y2 , unsigned short color , int dose_fill_it )
{
	buffer_draw_x_line(buffer_ADDR, buffer_width , buffer_height,  y1 , x1 , x2 , color ) ;
	buffer_draw_x_line(buffer_ADDR, buffer_width , buffer_height,  y2 , x1 , x2 , color ) ;
	buffer_draw_y_line(buffer_ADDR, buffer_width , buffer_height,  x1 , y1 , y2 , color ) ;
	buffer_draw_y_line(buffer_ADDR, buffer_width , buffer_height,  x2 , y1 , y2 , color ) ;
	if( dose_fill_it ){
		buffer_fill_rect( buffer_ADDR , buffer_width, buffer_height ,  x1 , y1 , x2 , y2 , color ) ;
	}
}

void buffer_fill_rect(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x1 , unsigned int y1 , unsigned int x2 , unsigned int y2 , unsigned short color )
{
	for( int x = x1 ; x < x2 + 1 ; ++x ){
		for( int y = y1 ; y < y2 + 1 ; ++y ){
			buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  x , y , color ) ;
		}
	}
}

void buffer_print_english(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y , unsigned int pos_in_font , unsigned short color )
{
	unsigned char *english_font_addr = ( unsigned char * )0x30200 + pos_in_font * 16 ; // 一个英文 16 字节
	union{
		unsigned char ch ;
		struct{
			unsigned char ch0 : 1 ;
			unsigned char ch1 : 1 ;
			unsigned char ch2 : 1 ;
			unsigned char ch3 : 1 ;
			unsigned char ch4 : 1 ;
			unsigned char ch5 : 1 ;
			unsigned char ch6 : 1 ;
			unsigned char ch7 : 1 ;
		} ;
	} u ;
	
	for( int j = 0 ; j < 16 ; ++j ){ // 16 lines for an English character
		u.ch = english_font_addr[ j ] ;
		int offset = x ;
		if( u.ch7 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset , y , color ) ;
		if( u.ch6 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 1  , y , color ) ;
		if( u.ch5 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 2 , y , color ) ;
		if( u.ch4 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 3 , y , color ) ;
		if( u.ch3 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 4 , y , color ) ;
		if( u.ch2 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 5 , y , color ) ;
		if( u.ch1 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 6 , y , color ) ;
		if( u.ch0 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 7 , y , color ) ;
		//english_font_addr[ j ]=0;
		y++ ;// move y ON THE SCREEN to the next line
	}
}


void buffer_print_chinese(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y , unsigned int pos_in_font , unsigned short color )
{
	unsigned char *chinese_font_addr = ( unsigned char * )0x30000 + pos_in_font * 32 ; // 一个汉字 32 字节
	
	union{
		unsigned char ch ;
		struct{
			unsigned char ch0 : 1 ;
			unsigned char ch1 : 1 ;
			unsigned char ch2 : 1 ;
			unsigned char ch3 : 1 ;
			unsigned char ch4 : 1 ;
			unsigned char ch5 : 1 ;
			unsigned char ch6 : 1 ;
			unsigned char ch7 : 1 ;
		} ;
	} u ;
	
	for( int j = 0 ; j < 16 ; ++j ){ // 一个汉字 16 行
		for( int i = 0 ; i < 2 ; ++i ){ // 每行 2 个字节 ( 16 位 )
			u.ch = chinese_font_addr[ j * 2 + i ] ;
			int offset = x + i * 8 ; // 一个字节 8 个像素
			if( u.ch7 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset , y , color ) ;
			if( u.ch6 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 1  , y , color ) ;
			if( u.ch5 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 2 , y , color ) ;
			if( u.ch4 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 3 , y , color ) ;
			if( u.ch3 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 4 , y , color ) ;
			if( u.ch2 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 5 , y , color ) ;
			if( u.ch1 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 6 , y , color ) ;
			if( u.ch0 )	buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  offset + 7 , y , color ) ;
		}
		y++ ; // move y ON THE SCREEN to the next line
	}
}

unsigned short buffer_change_color_form_555_to_565( unsigned short color_form_555 )
{
	union{
		unsigned int color ;
		struct{
			unsigned char b : 5 ;
			unsigned char g : 6 ;
			unsigned char r : 5 ;
		} ;
	} u1 ;
	
	union{
		unsigned int color ;
		struct{
			unsigned char b : 5 ;
			unsigned char g : 5 ;
			unsigned char r : 5 ;
			unsigned char alpha : 1 ;  // alpha 通道，但这里不用
		} ;
	} u2 ;
	
	u2.color = color_form_555 ;
	u1.r = u2.r ;
	if( u2.g ){
		u1.g = ( int )u2.g * 2 + 1 ; // 把 555 中间的色彩 * 2，为了掩色处理方便，如果不是 0，则末位恒置 1
	}
	else{
		u1.g = 0 ;
	}
	u1.b = u2.b ;
	return u1.color ;

}
void buffer_load_bitmap_16(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y , void *bmp_addr , unsigned short mask_color , int use_mask_color )
{ 
	// Only windows 16-bit bmp (1:5:5:5) is supported
	struct bmp_bmp_head_struct *bmp_head = ( struct bmp_bmp_head_struct * )bmp_addr;
	int width = bmp_head->info_head.width ;
	int height = bmp_head->info_head.height ;
	
	// 下面记算存储每个点的色彩的信息所在的位置
	unsigned short *color = ( unsigned short * )( ( unsigned int )bmp_addr + bmp_head->offset ) ;

	// 由于一行的字节数比须是 4 的倍数，因此，这里先计算每行需要的填充数，除 2 是因为每个像素两个字节
	int fill_length = width * 2 % 4 / 2 ; 
	
	// bmp 的存放顺序是从下到上，从左到右
	for( int i = height - 1 ; i >= 0 ; --i ){
		for( int j = 0 ; j < width ; ++j ){
			unsigned short temp_color = buffer_change_color_form_555_to_565( *color ) ;
			if( !use_mask_color || temp_color != mask_color ){
				// draw every point
				buffer_draw_point(buffer_ADDR, buffer_width , buffer_height,  x + j , y + i , temp_color ) ;
			}
			++color ;
		}
		color += fill_length ;
	}
}

void buffer_rotate(unsigned short *buffer_ADDR, unsigned int buffer_width,unsigned int buffer_height ,int upside_down, int left_right_reverse)
{
	unsigned short temp_color;
	if (upside_down>0) {
		for( int j = 0 ; j < buffer_height/2 ; ++j ){
			for( int i = 0 ; i < buffer_width ; ++i ){
				temp_color = *( buffer_ADDR + (buffer_height-j) * buffer_width + i );
				*( buffer_ADDR + (buffer_height-j) * buffer_width + i ) = *( buffer_ADDR + j * buffer_width + i ); 
				*( buffer_ADDR + j * buffer_width + i ) = temp_color;
			}
		}
	};
	if (left_right_reverse>0) {
		for( int j = 0 ; j < buffer_height ; ++j ){
			for( int i = 0 ; i < buffer_width/2 ; ++i ){
				temp_color = *( buffer_ADDR + j * buffer_width + (buffer_width-i) );
				*( buffer_ADDR + j * buffer_width + (buffer_width-i) ) = *( buffer_ADDR + j * buffer_width + i );
				*( buffer_ADDR + j * buffer_width + i ) = temp_color;
			}
		}
	};
}