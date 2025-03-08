#include "NOS_Includes.h"
#include "NOS_BaseColors.h"

#define EFFECT_BREATHE_ID 0x20
#define EFFECT_RAINBOW_ID 0x21
#define EFFECT_DOTS_ID 0x22

/* Effects main */
void NOS_WS2812B_Strip_Effects_AddEffect(WS2812B_Strip* strip,Effect_Struct effect);

void NOS_WS2812B_Strip_Effects_UpdateEffect(WS2812B_Strip* strip,Effect_Struct effect,uint8_t pos);

void NOS_WS2812B_Strip_Effects_Handler(WS2812B_Strip* strip);

void NOS_WS2812B_Strip_Effect_Copy(Effect_Struct* destination,Effect_Struct* source);

void NOS_WS2812B_Strip_Effect_Init(Effect_Struct* effect,PixelColor* color,uint16_t speed,uint16_t step,uint16_t minValue,uint16_t maxValue,uint8_t effectID);

/* Breathe effect */

void NOS_WS2812B_Strip_Effect_Breathe_Handler(WS2812B_Strip* strip,Effect_Struct* effect);

/* Rainbow effect */

void NOS_WS2812B_Strip_Effect_Rainbow_Handler(WS2812B_Strip* strip,Effect_Struct* effect);

/* Dots Effect */

void NOS_WS2812B_Strip_Effect_Dots_Handler(WS2812B_Strip* strip,Effect_Struct* effect);
