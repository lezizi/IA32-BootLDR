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
             VESA Graphic Driver
 ==========================================

*/
#ifndef _VESA_H_
#define _VESA_H_

extern const int VESA_SCREEN_X_MAX ;
extern const int VESA_SCREEN_Y_MAX ;

unsigned short vesa_compound_rgb( unsigned char r , unsigned char g , unsigned char b ) ;
unsigned short vesa_change_color_form_555_to_565( unsigned short color_form_555 ) ;
void vesa_draw_point( unsigned int x , unsigned int y , unsigned short color ) ;
void vesa_fill_rect( unsigned int x1 , unsigned int y1 , unsigned int x2 , unsigned int y2 , unsigned int color ) ;
void vesa_clean_screen( unsigned short color ) ;
void vesa_draw_x_line( unsigned int y , unsigned int x1 , unsigned int x2 , unsigned short color ) ;
void vesa_draw_y_line( unsigned int x , unsigned int y1 , unsigned int y2 , unsigned short color ) ;
void vesa_draw_rect( unsigned int x1 , unsigned int y1 , unsigned int x2 , unsigned y2 , unsigned short color , int dose_fill_it ) ; 
void vesa_print_english( unsigned int x , unsigned int y , unsigned int pos_in_font , unsigned short color ) ;
void vesa_print_chinese( unsigned int x , unsigned int y , unsigned int pos_in_font , unsigned short color ) ;
void vesa_show_bmp_picture( unsigned int x , unsigned int y , void *bmp_addr , unsigned short mask_color , int dose_use_mask_color ) ;
void vesa_copy_picture_from_screen( unsigned int x , unsigned int y , unsigned short *object_picture_addr , unsigned int picture_width , unsigned int picture_height ) ;
void vesa_copy_picture_to_screen( unsigned int x , unsigned int y , unsigned short *source_picture_addr , unsigned int picture_width , unsigned int pictrue_height ) ;
unsigned short vesa_get_point_color( unsigned int x , unsigned int y ) ;

void vesa_print_chinese( unsigned int x , unsigned int y , unsigned int pos_in_font , unsigned short color );
void vesa_print_english( unsigned int x , unsigned int y , unsigned int pos_in_font , unsigned short color );
int vesa_print_integer( unsigned int x , unsigned int y , unsigned int in_integer , unsigned short color );

// --------------------我是分割线---------------------
// 上面是直接绘图，而下面是缓冲区绘图，推荐使用下面的 vesa_draw_buffer

void vesa_draw_buffer ( unsigned short *buffer_ADDR, unsigned int buffer_width , unsigned int buffer_height, unsigned int x , unsigned int y ,  unsigned short mask_color , int use_mask_color );
#endif
