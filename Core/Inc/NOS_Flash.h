#ifndef NOS_FLASH
#define NOS_FLASH

#include "NOS_Includes.h"

#define FLASH_STORAGE_A 0x08005000
#define FLASH_STORAGE_B 0x08005500

void NOS_Flash_Load_Block(void* pointer, uint32_t address,uint32_t offset,size_t size);
void NOS_Flash_Save_Block(void* pointer, uint32_t address,uint32_t offset,size_t size);

void NOS_Flash_Chunk_Init(NOS_Flash_Chunk* chunk,void* dataPtr,size_t size,uint32_t offset);
void NOS_Flash_Chunk_Copy(NOS_Flash_Chunk* destination,NOS_Flash_Chunk* source);

void NOS_FlashMemory_Struct_Init(NOS_Flash_Memory_Struct* flashMemory,uint32_t baseAddress);
void NOS_FlashMemory_Struct_AddChunk(NOS_Flash_Memory_Struct* flashMemory, NOS_Flash_Chunk* chunk);
size_t NOS_FlashMemory_Struct_GetTotalSize(NOS_Flash_Memory_Struct* flashMemory);
uint32_t* NOS_Flash_Memory_Struct_GetU32Data(NOS_Flash_Memory_Struct* flashMemory);

uint32_t* NOS_Flash_Memory_Struct_GetChunk(NOS_Flash_Memory_Struct* flashMemory,uint32_t offset);

void NOS_Flash_Memory_Struct_Save(NOS_Flash_Memory_Struct* flashMemory);
void NOS_Flash_Memory_Struct_Load(NOS_Flash_Memory_Struct* flashMemory);


#endif