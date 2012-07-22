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
#ifndef _MOUSE_H_
	#define _MOUSE_H_

	#define MOUSE_WIDTH 10
	#define MOUSE_HEIGHT 20

	enum mouse_type_enum{ MOUSE_NORMAL_MOUSE , MOUSE_OVER_MOUSE , MOUSE_DOWN_MOUSE } ;
	void mouse_init() ;
	void mouse_show_mouse( int x , int y , enum mouse_type_enum mouse_type ) ;
	void mouse_move_mouse( int x1 , int y1 , int x2 , int y2 , enum mouse_type_enum mouse_type ) ;
	void mouse_save_picture( int x , int y ) ;
	void mouse_restore_picture( int x , int y ) ;
#endif