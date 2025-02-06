#ifndef NOS_TYPEDEFS
#define NOS_TYPEDEFS

#include "stdint.h"
#include "stm32f1xx.h"
#include "stdbool.h"
/* Unions begin */

typedef union Short_t //uint16_t data wrap
{ 
    uint8_t bytes[2];
    uint16_t data;
} NOS_Short;

typedef union Float_t //float data wrap
{
    uint8_t bytes[4];
    float data;
} NOS_Float;

typedef union Long_t
{
  uint8_t bytes[4];
  uint32_t data;
} NOS_Long;

/* Unions end */

/* Structures begin */

/*WS2812B Matrix*/
typedef struct Point_t
{
  uint16_t x;
  uint16_t y;
}Point;


typedef struct PixelColor_t
{
   uint8_t R;
   uint8_t G;
   uint8_t B;
} PixelColor;

typedef struct MatrixSize_t
{
  uint8_t col;
  uint8_t row;
} MatrixSize;


typedef struct IdealSymbol_t
{
  uint8_t* data;
  uint8_t lenght;
} IdealSymbol;



typedef struct  Symvol_t
{
  uint8_t data[5];
} Symvol;


typedef struct WS2812B_Matrix_t
{
    uint8_t* buffer;
    uint8_t bright;
    NOS_Short ledsCount;
    MatrixSize* size;
    PixelColor* textColor;
    PixelColor* foneColor;
    Symvol* symvols;
} WS2812B_Matrix;



/*WS2812B Matrix*/

typedef enum TimeFormat_e{Hour24,Hour12} TimeFormat;

typedef struct RealTime_t
{
  uint32_t milliseconds;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  TimeFormat format;
} RealTime;

/// @brief 
typedef struct NOS_TimeEvent_t
{
  uint32_t ticksToEnable;
  bool stopped;
  bool enabled;
  uint32_t ticks;
} NOS_TimeEvent;

typedef struct GPIO_PIN_t
{
  GPIO_TypeDef* Port;
  uint16_t Pin;
} GPIO_PIN;

//typedef enum TemperatureFormat_t{Celcium,Farengheit} TempFormat;
typedef enum Language_e{English,Ukrainian} Language;
typedef enum MoveDirection_e {UP,DOWN,RIGHT,LEFT} MoveDirection;

typedef enum Signal_Level_e {LOW,HIGH} Signal_Level;
typedef enum Button_State_e
{
    Idle,pressed,released,pressDone
}Button_State;

typedef struct NOS_Button_t
{
    bool pressed;
    bool released;
    bool pressDone;
    bool stopped;
    GPIO_PinState openLevel;

    uint32_t timer;
    uint32_t pressedTime;
    uint32_t releasedTime;
    uint32_t lastDoneTime;
    uint32_t stopTime;

    GPIO_PIN* pin;

}NOS_Button;

typedef struct SinValue_t
{
    int value;

    int minValue;
    int maxValue;
    int increment;
    bool rise;
    bool fall;
}SinValue;

typedef struct Effect_Struct_t
{
  SinValue value;
  NOS_Short speed;
  NOS_Short step;
  uint16_t timer;

  uint16_t minValue;
  uint16_t maxValue;

  bool enabled;
  uint8_t effectId;
} Effect_Struct;

typedef struct
{
    NOS_Short address;
    NOS_Short channel;
    NOS_Short byteCount;
    NOS_Long packetId;
    NOS_Long command;
    uint8_t data[16];
    NOS_Short CRC16;
} UART_Message;

/**
  * @brief  Strip structure.
  * Warning : No Base init avalaible
  * @param frameBuffer pointer on core frame buffer in WS2812B_Core 
  * @param pixels pointer on pixelBuffer
  * @param pixelCount int count of Leds in strip
  * @param bright int (0-100) brightness of led
  * @retval void,in future will be NOS_System_State
  */
typedef struct NOS_WS2812B_STRIP_t
{
    uint8_t* frameBuffer;
    PixelColor* pixels;
    int pixelCount;
    int bright;

    Effect_Struct effects[10];
    uint8_t effectsCounter;
}WS2812B_Strip;

/* STM32 Core structures begin*/



/* STM32 Core structures end*/

/* Structures end */
#endif