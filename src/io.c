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
               Hardware IO
 ==========================================

*/

unsigned char io_read_from_io_port( unsigned short port_number )
{
  unsigned char result ;
  __asm__( "in %%dx , %%al" : "=a"( result ) : "d"( port_number ) ) ;
  return result ;
}

void io_write_to_io_port( unsigned short port_number , unsigned char data )
{
  __asm__( "out %%al , %%dx" : : "a"( data ) , "d"( port_number ) ) ;
}
