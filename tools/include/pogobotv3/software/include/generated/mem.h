//--------------------------------------------------------------------------------
// Auto-generated by LiteX (3ab7eaa5) on 2023-01-05 08:25:40
//--------------------------------------------------------------------------------
#ifndef __GENERATED_MEM_H
#define __GENERATED_MEM_H

#ifndef SRAM_BASE
#define SRAM_BASE 0x00000000L
#define SRAM_SIZE 0x00020000
#endif

#ifndef SPIFLASH_BASE
#define SPIFLASH_BASE 0x00200000L
#define SPIFLASH_SIZE 0x00200000
#endif

#ifndef ROM_BASE
#define ROM_BASE 0x00260000L
#define ROM_SIZE 0x00010000
#endif

#ifndef CSR_BASE
#define CSR_BASE 0xf0000000L
#define CSR_SIZE 0x00010000
#endif

#ifndef MEM_REGIONS
#define MEM_REGIONS "SRAM      0x00000000 0x20000 \nSPIFLASH  0x00200000 0x200000 \nROM       0x00260000 0x10000 \nCSR       0xf0000000 0x10000 "
#endif
#endif