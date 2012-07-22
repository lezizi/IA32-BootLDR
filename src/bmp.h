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
                  Bitmap
 ==========================================

*/

#ifndef _BMP_H_
#define _BMP_H_
/****************************************
*  Structures for 16-bit BMP(5:6:5)     *
*****************************************/

struct bmp_bmp_info_head_struct{
	int the_struct_size ;
	int width ;
	int height ;
	// There should have been more metadata, however, AF handles only 16-bit BMP now.
} ;

struct bmp_bmp_head_struct{
	short type ;
	int size_file ;
	short reserved_0 ;
	short reserved_1 ;
	int offset ;
	struct bmp_bmp_info_head_struct info_head ;
} ;
#endif
