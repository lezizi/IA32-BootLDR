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
                Interrupt
 ==========================================

*/
#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_
void interrupt_init() ;
void interrupt_open_interrupt() ;
void interrupt_close_interrupt() ;
void interrupt_install_handle_for_interrupt( unsigned char interrupt_number , void *handle_for_interrupt ) ;
void interrupt_set_interrupt_mask_word( unsigned int irq_number , int dose_enable ) ;
#endif
