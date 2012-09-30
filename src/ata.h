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

#ifndef _ATA_H_
#define _ATA_H_
void ata_read ( unsigned short IOBase, unsigned char blockNumber, unsigned short* buffer);
void ata_write ( unsigned short IOBase, unsigned char blockNumber, unsigned short* buffer);
void ata_open ( unsigned short IOBase);
#endif
