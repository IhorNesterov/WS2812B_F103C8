#ifndef NOS_STRIP_UART
#define NOS_STRIP_UART

#include "NOS_Includes.h"

#define UART_ADDRESS 0x65

//commands
#define CHECK_CONNECTION 0x20
#define SET_ONE_PIXEL_COLOR_COMMAND 0x30
#define SET_ROW_PIXEL_COLOR_COMMAND 0x31
#define SET_ALL_PIXEL_ONE_COLOR_COMMAND 0x32
#define SET_BRIGHT_COMMAND 0x33
#define SET_BREATHE_EFFECT_COMMAND 0x34
#define SET_ALL_PIXEL_COMMAND 0x35

#define SET_BREATHE_EFFECT_COMMAND 0x50
#define SET_RAINBOW_EFFECT_COMMAND 0x51

#define SET_EFFECT_ACTIVE 0x71

#define EFFECTS_MAX_COUNT 10
#define EFFECT_BREATHE_ID 0x20
#define EFFECT_RAINBOW_ID 0x21

//const char* NOS_Strip_Uart_ParseCommand(WS2812B_Strip* strip,uint8_t* command);

bool NOS_Strip_UART_ParseCommand(WS2812B_Strip* strip,UART_Message* message);

#endif