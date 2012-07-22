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
#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

unsigned short buffer_compound_rgb( unsigned char r , unsigned char g , unsigned char b ) ;
unsigned short buffer_change_color_form_555_to_565( unsigned short color_form_555 ) ;
int buffer_is_point_in_rect( int x , int y , int x1 , int y1 , int x2 , int y2 );
void buffer_draw_point(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y , unsigned short color ) ;
void buffer_fill_rect(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x1 , unsigned int y1 , unsigned int x2 , unsigned int y2 , unsigned short color );
void buffer_clean(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned short color ) ;
void buffer_draw_x_line(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int y , unsigned int x1 , unsigned int x2 , unsigned short color ) ;
void buffer_draw_y_line(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y1 , unsigned int y2 , unsigned short color ) ;
void buffer_draw_rect(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x1 , unsigned int y1 , unsigned int x2 , unsigned y2 , unsigned short color , int dose_fill_it ) ; 
void buffer_print_english(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y , unsigned int pos_in_font , unsigned short color ) ;
void buffer_print_chinese(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y , unsigned int pos_in_font , unsigned short color ) ;

void buffer_load_bitmap_16(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y , void *bmp_addr , unsigned short mask_color , int use_mask_color ) ;

unsigned short buffer_get_point_color(unsigned short *buffer_ADDR , unsigned int buffer_width,unsigned int buffer_height , unsigned int x , unsigned int y ) ;
void buffer_rotate(unsigned short *buffer_ADDR, unsigned int buffer_width,unsigned int buffer_height ,int upside_down, int left_right_reverse);
#endif