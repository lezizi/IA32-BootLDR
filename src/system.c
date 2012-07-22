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
                Kernel HCL
 ==========================================

*/
#include "interrupt.h"
#include "mouse.h"
#include "keyboard.h"

/* 声明 gdt 表初始化函数 */
static void system_init_gdt() ;


/* GDT表项结构 */
struct system_gdt_item_struct{
	unsigned short seg_length_limit_0_15 ;		// 段限0~15位
	unsigned short seg_base_addr_0_15 ;			// 基址的 0~15　位
	unsigned char  seg_base_addr_16_23 ;		// 基址的16~23位
	unsigned char  type    : 4 ;				// type位
	unsigned char  s       : 1 ;				// S位
	unsigned char  dpl     : 2 ;				// 特权位
	unsigned char  p       : 1 ;				// P位
	unsigned char  seg_length_limit_16_19 : 4 ;	// 段限的16~19位
	unsigned char  avl     : 1 ;				// AVL位
	unsigned char  saved_0 : 1 ;				// 保留位，必须为0
	unsigned char  d_or_b  : 1 ;				// D/B位
	unsigned char  g       : 1 ;				// G位
	unsigned char  seg_base_addr_24_31 ;		// 基址的24~31位
} ;

/* GDT表结构 */
struct system_gdt_struct{ 
	struct system_gdt_item_struct null_seg ;		// 空段，Intel保留
	struct system_gdt_item_struct system_code_seg ;	// 系统代码段
	struct system_gdt_item_struct system_date_seg ;	// 系统数据段  
} ;

/* GDT 描述符结构 */
struct system_gdt_descriptor_struct{ 
	unsigned short gdt_length_limit ;
	struct system_gdt_struct *gdt_addr ;
} ;

/* 定义 gdt 表 */
static struct system_gdt_struct system_gdt ;

/* 定义 gdt 描述符 */
static struct system_gdt_descriptor_struct system_gdt_descriptor ;

/* 系统初始化 */
void system_init()  
{
	// 由于以后可能向 gdt 表中增添项目,因此,此处应重新初始化 gdt 表,以保存 gdt 表位置
	system_init_gdt() ;
	
	// 初始化中断
	interrupt_init() ;
	
	// 初始化键盘
	keyboard_init() ;
	
	// 初始化 mouse
	mouse_init() ;
}

/* 改写 gdt 表 */
static void system_init_gdt()
{  
	// 初始化系统代码段
	struct system_gdt_item_struct *p = &system_gdt.system_code_seg ;
	p->avl = 0 ;
	p->d_or_b = 1 ;
	p->dpl = 0 ;
	p->g = 1 ;
	p->p = 1 ;
	p->s = 1 ;
	p->saved_0 = 0 ;
	p->seg_base_addr_0_15 = 0 ;
	p->seg_base_addr_16_23 = 0 ;
	p->seg_base_addr_24_31 = 0 ;
	p->seg_length_limit_0_15 = 0xffff ;
	p->seg_length_limit_16_19 = 0xff ;
	p->type = 0x9a ;
	
	// 初始化系统数据段
	system_gdt.system_date_seg = system_gdt.system_code_seg ;
	system_gdt.system_date_seg.type = 0x92 ;
	
	// 初始化 gdt 表描述符
	system_gdt_descriptor.gdt_addr = &system_gdt ;
	system_gdt_descriptor.gdt_length_limit = 0xffff ;
	
	// 内嵌汇编载入 gdt 表描述符
	__asm__( "lgdt %0" : "=m"( system_gdt_descriptor ) ) ;
}