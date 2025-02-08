#include "config.h"

fmc_blk_t fmc_blk = {0};

void fmc_erase_update_flag(uint32_t addr)
{
    uint32_t EraseCounter;

    /* unlock the flash program/erase controller */
    fmc_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);

    fmc_page_erase(addr);
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);

    /* lock the main FMC after the erase operation */
    fmc_lock();
}

void fmc_program_update_success(void)
{
    /* unlock the flash program/erase controller */
    fmc_unlock();

    uint32_t address = USER_UPDATE_FLAG_ADDR;

    /* program flash */

        fmc_word_program(address, USER_UPDATE_VAL);

        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    

    /* lock the main FMC after the program operation */
    fmc_lock();
}

void fmc_erase_backup_pages(void)
{
    uint32_t EraseCounter;

    /* unlock the flash program/erase controller */
    fmc_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);

    /* erase the flash pages */
    for (EraseCounter = 0; EraseCounter < FMC_BACKUP_PAGE_SIZE; EraseCounter++)
    {
        fmc_page_erase(FMC_BACKUP_START_ADDR + (FMC_PAGE_SIZE * EraseCounter));
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    }

    /* lock the main FMC after the erase operation */
    fmc_lock();
}

void fmc_program_pack(uint8_t *data, uint16_t length, uint32_t address)
{
    /* unlock the flash program/erase controller */
    uint32_t *test = (uint32_t *)FMC_BACKUP_START_ADDR;
    fmc_unlock();
    uint16_t idx = 0;
    uint16_t data_halfword=0;
    while(idx<length)
    {
        data_halfword = ((uint16_t)data[idx+1] << 8) | data[idx];
        /* program flash */

        fmc_halfword_program(address, data_halfword);
        address += 2;
        idx += 2;
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    }

    /* lock the main FMC after the program operation */
    fmc_lock();
}

void fmc_init()
{
    memset(&fmc_blk, 0, sizeof(fmc_blk));
    //fmc_blk.pack_addr = FMC_BACKUP_START_ADDR;
}
