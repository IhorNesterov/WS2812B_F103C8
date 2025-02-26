#ifndef NOS_FLASH
#define NOS_FLASH

#include "NOS_Includes.h"

#define FLASH_STORAGE_A 0x08005000
#define FLASH_STORAGE_B 0x08005500
#define page_size 0x800

void NOS_Flash_Save_Block(uint32_t *data, uint32_t address,size_t size);

void NOS_Flash_Load_Block(uint32_t* data,uint32_t address,size_t size);

bool NOS_Flash_Validate_Block(uint32_t address,size_t size);

#endif