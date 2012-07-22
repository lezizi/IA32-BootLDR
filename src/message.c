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
#include "message.h"
#include "interrupt.h"

/****************************************
*  implementation                       *
*****************************************/

void message_init_message_queue( struct message_message_queue_struct *message_queue )
{
	message_queue->head = 0 ;
	message_queue->tail = 0 ;
}

int message_get_message( struct message_message_queue_struct *message_queue , struct message_message_struct *message )
{
	// 先关中断，使得在取消息的同时，中断函数不得操作消息队列，互斥。
	//interrupt_close_interrupt() ;
	//__asm__( "cli" ) ;

	if( message_queue->head == message_queue->tail ){
		
		return 0 ; // return 0 when fail
	}
	
	message_queue->head = ++message_queue->head % 256 ;
	*message = message_queue->message_buffer[ message_queue->head ] ;
	
	return 1 ; // return 1 when success

	//__asm__( "sti" ) ;
	//interrupt_open_interrupt() ; 
}

void message_put_message( struct message_message_queue_struct *message_queue , struct message_message_struct message )
{
	int tail = ( message_queue->tail + 1 ) % 256 ;
	if( tail == message_queue->head ){
		// return when message queue is full
		return ;
	}
	message_queue->tail = tail ;
	message_queue->message_buffer[ message_queue->tail ] = message ;
}
