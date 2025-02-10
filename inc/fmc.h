#ifndef __FMC_H
#define __FMC_H

#define FMC_BACKUP_START_ADDR       0X0801E000
#define FMC_BACKUP_PAGE_SIZE        32
#define FMC_PAGE_SIZE               0X800

#define USER_UPDATE_FLAG_ADDR            0X0802D000
#define USER_UPDATE_VAL                 0xAAAAAAAA
#include "stdint.h"
typedef struct 
{
    uint32_t pack_addr;
    uint16_t pack_idx;
    uint16_t pack_crc;
    uint16_t pack_length;
    uint32_t file_recv_length;
    uint32_t file_length;
    uint16_t file_crc;
    
}fmc_blk_t;

extern fmc_blk_t fmc_blk;


void fmc_erase_backup_pages(void);
void fmc_program_pack(uint8_t *data, uint16_t length, uint32_t address);
void fmc_init(void);
void fmc_program_update_success(void);
void fmc_erase_update_flag(uint32_t addr);
#endif

