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
            Kernel Message Queue
 ==========================================

*/
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

enum message_message_type_enum{
	MESSAGE_MOUSE_MESSAGE , 
	MESSAGE_SHUTDOWN_COMPUTER ,
	MESSAGE_KEYBOARD_MESSAGE
} ;
struct message_message_struct{
	enum message_message_type_enum message_type ;
	union {
		unsigned char dose_left_button_down ;
		unsigned char key ;
	} ;
	union {
		unsigned char dose_right_button_down ;
	} ;
	union {
		unsigned int x_position ;
	} ;
	union {
		unsigned int y_position ;
	} ;
} ;
struct message_message_queue_struct{
	int head ;
	int tail ;
	struct message_message_struct message_buffer[ 8 ] ;
} ;
int message_get_message( struct message_message_queue_struct *message_queue , struct message_message_struct *message ) ;
void message_put_message( struct message_message_queue_struct *message_queue , struct message_message_struct message ) ;
void message_init_message_queue( struct message_message_queue_struct *message_queue ) ; 
#endif
