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
                 ATA Driver
 ==========================================

*/
#include "ata.h"
#include "io.h"
void ata_open ( unsigned short IOBase)
{
	io_write_to_io_port(IOBase + 7, 0x95);
}

void ata_read ( unsigned short IOBase, unsigned char blockNumber, unsigned short* buffer)
{
	io_write_to_io_port(IOBase + 6, 0xE0 | (blockNumber >> 24) & 0xf); // LBA, master,
	io_write_to_io_port(IOBase + 2, 1); // Read only one sector.
	io_write_to_io_port(IOBase + 3, blockNumber & 0xff); // bit 0 - 7 of the sector id
	io_write_to_io_port(IOBase + 4, (blockNumber >> 8) & 0xff); // bit 8 - 15 of the sector id
	io_write_to_io_port(IOBase + 5, (blockNumber >> 16) & 0xff); // bit 16 - 23 of the sector id__
	// wait until the controller accepts command
	while(!(io_read_from_io_port(IOBase + 7) & 0x40));  
	
	while((io_read_from_io_port(IOBase + 7) == 88)); //等待扇面缓冲正常
	// send a read command
	io_write_to_io_port(IOBase + 7, 0x20);  
	for(int i = 0; i < 512; i++)  
	{  
		((short *)buffer)[i] = io_read_from_io_port(IOBase + 0);  
		//((short *)buffer)[i] = io_read_from_io_port(IOBase +1);
	}
	
}

void ata_write ( unsigned short IOBase, unsigned char blockNumber, unsigned short* buffer)
{
	io_write_to_io_port(IOBase + 6, 0xE0 | (blockNumber >> 24) & 0xf); // LBA, master,
	io_write_to_io_port(IOBase + 2, 1); // Read only one sector.
	io_write_to_io_port(IOBase + 3, blockNumber & 0xff); // bit 0 - 7 of the sector id
	io_write_to_io_port(IOBase + 4, (blockNumber >> 8) & 0xff); // bit 8 - 15 of the sector id
	io_write_to_io_port(IOBase + 5, (blockNumber >> 16) & 0xff); // bit 16 - 23 of the sector id__
	// wait until the controller accepts command
	while(!(io_read_from_io_port(IOBase + 7) & 0x40));  
	
	while((io_read_from_io_port(IOBase + 7) == 88)); //等待扇面缓冲正常
	// send a read command
	io_write_to_io_port(IOBase + 7, 0x30);  
	for(int i = 0; i < 512; i++)  
	{  
		io_write_to_io_port(IOBase + 0,((short *)buffer)[i]);  
		//((short *)buffer)[i] = io_read_from_io_port(IOBase +1);
	}
	
}

