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
	
	volatile HAL_StatusTypeDef status = HAL_OK;
	volatile uint32_t write_cnt=0, index=0;


	
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = address;
	EraseInitStruct.NbPages = 1;
	uint32_t PageError;
	status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	
	

	while(index < u32_count)
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

	return validated;
}

void NOS_Flash_Chunk_Init(NOS_Flash_Chunk* chunk,void* dataPtr,size_t size,uint32_t offset)
{
	chunk->dataPointer = dataPtr;
	chunk->size = size;
	chunk->offset = offset;
}

void NOS_FlashMemory_Struct_Init(NOS_Flash_Memory_Struct* flashMemory,uint32_t baseAddress)
{
	flashMemory->baseAddress = baseAddress;
	flashMemory->chunksCounter = 0;
}

void NOS_Flash_Chunk_Copy(NOS_Flash_Chunk* destination,NOS_Flash_Chunk* source)
{
	destination->dataPointer = source->dataPointer;
	destination->offset = source->offset;
	destination->size = source->size;
}

void NOS_FlashMemory_Struct_AddChunk(NOS_Flash_Memory_Struct* flashMemory, NOS_Flash_Chunk* chunk)
{
	if(flashMemory->chunksCounter < FLASH_CHUNKS_MAX_COUNTER)
	{
		NOS_Flash_Chunk_Copy(&flashMemory->chunks[flashMemory->chunksCounter],chunk);
		flashMemory->chunksCounter++;
		flashMemory->totalSize += chunk->size;
	}
}

size_t NOS_FlashMemory_Struct_GetTotalSize(NOS_Flash_Memory_Struct* flashMemory)
{
	return flashMemory->totalSize;
}

uint32_t* NOS_Flash_Memory_Struct_GetU32Data(NOS_Flash_Memory_Struct* flashMemory)
{
	uint32_t data[flashMemory->totalSize/4];
	uint32_t dataCounter = 0;

	for(int currChunk = 0; currChunk < flashMemory->chunksCounter; currChunk++)
	{
		uint32_t* pointer = flashMemory->chunks[currChunk].dataPointer;
		uint32_t currChunkU32Size = flashMemory->chunks[currChunk].size/4;

		for(int i = 0; i < currChunkU32Size; i++)
		{
			data[dataCounter] = *pointer;
			pointer++;
			dataCounter++;
		}
	}

	return data;
}

uint32_t* NOS_Flash_Memory_Struct_GetChunk(NOS_Flash_Memory_Struct* flashMemory,uint32_t offset)
{
	for(int currChunk = 0; currChunk < flashMemory->chunksCounter; currChunk++)
	{
		if(flashMemory->chunks[currChunk].offset == offset)
		{
			uint32_t* pointer = flashMemory->chunks[currChunk].dataPointer;
			uint32_t currChunkU32Size = flashMemory->chunks[currChunk].size/4;
			uint32_t data[currChunkU32Size];
			uint32_t dataCounter = 0;

			for(int i = 0; i < currChunkU32Size; i++)
			{
				data[dataCounter] = *pointer;
				pointer++;
				dataCounter++;
			}
			return data;
		}
	}
	return NULL;
}

void NOS_Flash_Memory_Struct_Save(NOS_Flash_Memory_Struct* flashMemory)
{
	uint32_t address = flashMemory->baseAddress;
	uint32_t u32Count = flashMemory->totalSize/4;
	uint32_t data_to_flash[u32Count];

	uint32_t data[flashMemory->totalSize/4];
	uint32_t dataCounter = 0;

	for(int currChunk = 0; currChunk < flashMemory->chunksCounter; currChunk++)
	{
		uint32_t* pointer = flashMemory->chunks[currChunk].dataPointer;
		uint32_t currChunkU32Size = flashMemory->chunks[currChunk].size/4;

		for(int i = 0; i < currChunkU32Size; i++)
		{
			data_to_flash[dataCounter] = *pointer;
			pointer++;
			dataCounter++;
		}
	}

	HAL_FLASH_Unlock();
	HAL_FLASH_OB_Unlock();
	
	volatile HAL_StatusTypeDef status = HAL_OK;
	volatile uint32_t write_cnt=0, index=0;

	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = address;
	EraseInitStruct.NbPages = 1;
	uint32_t PageError;
	status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	
	

	while(index < u32Count)
	{
		if (status == HAL_OK)
		{
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address+write_cnt, data_to_flash[index]);
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

void NOS_Flash_Memory_Struct_Load(NOS_Flash_Memory_Struct* flashMemory)
{
	for(int currChunk = 0; currChunk < flashMemory->chunksCounter; currChunk++)
	{
		uint32_t currAddress = flashMemory->baseAddress + flashMemory->chunks[currChunk].offset;
		uint32_t* pointer = flashMemory->chunks[currChunk].dataPointer;
		uint32_t currChunkU32Size = flashMemory->chunks[currChunk].size/4;
		uint32_t data = 0;

		for(int i = 0; i < currChunkU32Size; i++)
		{	
			data = *(uint32_t*)currAddress;
			*pointer = data;
			currAddress += 4;
			pointer++;
		}

	}
}