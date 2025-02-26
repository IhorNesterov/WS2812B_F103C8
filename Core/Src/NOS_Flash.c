#include "NOS_Flash.h"

void NOS_Flash_Save_Block(uint32_t* data, uint32_t address,size_t size)
{
      uint32_t data_to_FLASH[size];


	  /* Unlock the Flash to enable the flash control register access *************/
	  HAL_FLASH_Unlock();

	  /* Allow Access to option bytes sector */
	  HAL_FLASH_OB_Unlock();

	  /* Fill EraseInit structure*/
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
			  status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address+write_cnt, data[index]);
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

NOS_Flash_Load_Block(uint32_t* data,uint32_t address,size_t size)
{
	volatile uint32_t read_cnt=0;
	
	for(int i = 0; i < size/4 ; i++)
	{
			data[i] = *(uint32_t*)(address + read_cnt);
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