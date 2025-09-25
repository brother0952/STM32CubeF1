/* 链接脚本 for STM32F103C8Tx with 64K Flash and 20K RAM */

/* 内存定义 */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 64K
  SRAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 20K
}

/* 最高地址定义 */
_Min_Stack_Size = 0x400;      /* 最小栈空间 1K */
_Min_Heap_Size = 0x200;       /* 最小堆空间 512字节 */

/* 栈顶地址 */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

/* 段定义 */
SECTIONS
{
  /* 代码段 */
  .text :
  {
    KEEP(*(.isr_vector))    /* 中断向量表 */
    *(.text*)               /* 代码段 */
    *(.rodata*)             /* 只读数据段 */
    _etext = .;             /* 代码段结束地址 */
    _sidata = .;            /* 数据段初始化值的起始地址 */
  } >FLASH

  /* 已初始化数据段 */
  .data :
  {
    _sdata = .;             /* 数据段起始地址 */
    *(.data*)               /* 数据段 */
    _edata = .;             /* 数据段结束地址 */
  } >SRAM AT> FLASH

  /* 未初始化数据段 */
  .bss :
  {
    _sbss = .;              /* BSS段起始地址 */
    *(.bss*)                /* BSS段 */
    *(COMMON)
    _ebss = .;              /* BSS段结束地址 */
  } >SRAM

  /* 用户堆栈段 */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >SRAM

  /* 移除调试信息 */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  /* 定义堆栈指针 */
  .ARM.attributes 0 : { *(.ARM.attributes) }
} 