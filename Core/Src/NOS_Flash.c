#include "NOS_Flash.h"

void NOS_Flash_Save_Block(void* pointer, uint32_t address,uint32_t offset,size_t size)
{
    uint32_t* data = pointer;

    int u32_count = size / 4;

	uint32_t data_to_flash[u32_count];

	for(int i = 0; i < u32_count; i++)
	{
		data_to_flash[i] = *data;
		data++;
	}

	HAL_FLASH_Unlock();


	HAL_FLASH_OB_Unlock();


	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = address;
	EraseInitStruct.NbPages = 1;
	uint32_t PageError;

	volatile uint32_t write_cnt=0, index=0;

	volatile HAL_StatusTypeDef status;
	status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	
	while(index < 8)
	{
		if (status == HAL_OK)
		{
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address+write_cnt+offset, data_to_flash[index]);
			if(status == HAL_OK)
			{
				write_cnt += 4;
				index++;
			}
		}
	}

	HAL_FLASH_OB_Lock();
	HAL_FLASH_Lock();
}

void NOS_Flash_Load_Block(void* pointer, uint32_t address,uint32_t offset,size_t size)
{
    uint32_t* data = pointer;

    int u32_count = size / 4;

	volatile int read_cnt = 0;

    for (int i = 0; i < u32_count; i++)
    {
		data[i] = *(uint32_t*)(address + read_cnt+offset);
		read_cnt += 4;
    }
}

bool NOS_Flash_Validate_Block(uint32_t address,size_t size)
{
	bool validated = true;
	int _size = size / 4;
	
	for(int i = 0; i < _size; i++)
	{
		if(*(uint32_t*)address == 0xFFFFFFFF)
		{
			validated = false;
		}
	}
}