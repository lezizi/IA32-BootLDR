/*
Copyright (C) 2013 LeZiZi Studio
 
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
           Memory Interrupt Handler
 ==========================================

*/

#include "interrupt.h"
#include "vesa.h"
#include "kernel.h"
#include "memory.h"

/****************************************
*  declaration                          *
*****************************************/

void memory_asm_handler_for_page_fault_interrupt();

void memory_handler_for_page_fault_interrupt() ;

/****************************************
*  defination                           *
*****************************************/

static const int MEMORY_PAGE_FAULT_INTERRUPT_NUMBER = 0x0E ;
unsigned int *page_directory = (unsigned int*) 0x900000; //暂时解决方案：远超过结束位置
unsigned int *first_page_table = (unsigned int*) 0x9C0000;

/****************************************
*  implementation                       *
*****************************************/


/* 声明 gdt 表初始化函数 
static void system_init_gdt() ;


/* GDT表项结构 
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

/* GDT表结构 
struct system_gdt_struct{ 
	struct system_gdt_item_struct null_seg ;		// 空段，Intel保留
	struct system_gdt_item_struct system_code_seg ;	// 系统代码段
	struct system_gdt_item_struct system_data_seg ;	// 系统数据段  
} ;

/* GDT 描述符结构 
struct system_gdt_descriptor_struct{ 
	unsigned short gdt_length_limit ;
	struct system_gdt_struct *gdt_addr ;
} ;

/* 定义 gdt 表 
static struct system_gdt_struct system_gdt ;

/* 定义 gdt 描述符 
static struct system_gdt_descriptor_struct system_gdt_descriptor ;

/* 改写 gdt 表 
static void system_init_gdt()
{  
 	// 由于以后可能向 gdt 表中增添项目,因此,此处应重新初始化 gdt 表,以保存 gdt 表位置
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
	system_gdt.system_data_seg = system_gdt.system_code_seg ;
	system_gdt.system_data_seg.type = 0x92 ;
	
	// 初始化 gdt 表描述符
	system_gdt_descriptor.gdt_addr = &system_gdt ;
	system_gdt_descriptor.gdt_length_limit = 0xffff ;
	
	// 内嵌汇编载入 gdt 表描述符
	__asm__( "lgdt %0" : "=m"( system_gdt_descriptor ) ) ;
}
*/

void memory_mmu_paging_init()
{
	// install interrupt handler
	interrupt_install_handle_for_interrupt( MEMORY_PAGE_FAULT_INTERRUPT_NUMBER , memory_asm_handler_for_page_fault_interrupt ) ;
	
	page_directory = (unsigned int*) 0xA000000; //暂时解决方案：远超过结束位置
	first_page_table = (unsigned int*) 0xAC0000;
	
	// holds the physical address where we want to start mapping these pages to.
	// 此时我们设置1024个页面覆盖4MB内存

	unsigned int address = 0; 
	for(unsigned int id_in_dir=0; id_in_dir<1024; id_in_dir++)//至少需要 4MB运行
	{
		first_page_table += 1024;
		for(unsigned int i = 0; i < 1024; i++)
		{
		    first_page_table[i] = address | 7; // 属性: supervisor level, read/write, present.
		    address = address + 4096; //advance the address to the next page boundary
		}
		page_directory[id_in_dir] = first_page_table; 
		page_directory[id_in_dir] |= 7;// attributes: supervisor level, read/write, present
	}
	//破坏性实验
	first_page_table =(unsigned int*)  0xAC0000;
	for(unsigned int id_in_dir=1024*2; id_in_dir<1024*800; id_in_dir++)
		first_page_table[id_in_dir] = 0 | 2;
	//破坏性实验

	// enable paging
	__asm__("mov %0, %%cr3":: "b"(page_directory));
	unsigned int cr0;
 	__asm__("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	cr0 &= 0xFFFEFFFF; //disable write protection
	__asm__("mov %0, %%cr0":: "b"(cr0));
	//__asm__("int $0x0e");// reserve for A-Framewrok action exchanging.
}

void memory_handler_for_page_fault_interrupt()
{
	unsigned int color = vesa_compound_rgb( 255 , 0 , 255  ) ;
	const char string[]  = "  (DEMO) ERROR OF PAGEFAULT HAS OCCURRED!          "; 
	//when using this, -fno-stack-protector must be included in the Makefile
	for( int i = 0 ; i < 52 ; ++i ){
		vesa_print_english(303+i*8 , 502 , string[i] , color ) ;
	}
	while(1);
	
}
