/* Linker script for ARM Cortex-M3 */

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Stack size */
_Min_Stack_Size = 0x400;

/* Heap size */
_Min_Heap_Size = 0x200;

SECTIONS
{
    /* Vector table and code */
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } >FLASH

    /* Initialized data */
    .data :
    {
        _sdata = .;
        *(.data*)
        _edata = .;
    } >SRAM AT>FLASH

    /* Zero-initialized data */
    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } >SRAM

    /* Stack and heap */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >SRAM
} 