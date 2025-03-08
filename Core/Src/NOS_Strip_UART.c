#include "NOS_Strip_UART.h"
#include "NOS_WS2812B_Strip.h"

bool NOS_Strip_UART_ParseCommand(WS2812B_Strip* strip,UART_Message* message)
{
    int currPos = 0;
    NOS_Short tempShort;
    
    switch (message->command.data)
    {
    case SET_BREATHE_EFFECT_COMMAND:

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            uint16_t pos = tempShort.data;

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            uint16_t speed = tempShort.data;

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            uint16_t step = tempShort.data;

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            uint16_t min = tempShort.data;

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            uint16_t max = tempShort.data;

            Effect_Struct tempEffect = {0};
            tempEffect.effectId = EFFECT_BREATHE_ID;
            tempEffect.color.R = 0;
            tempEffect.color.G = 0;
            tempEffect.color.B = 0;

            NOS_WS2812B_Strip_Effect_Init(&tempEffect,&tempEffect.color,speed,step,min,max,tempEffect.effectId);
            NOS_WS2812B_Strip_Effects_UpdateEffect(strip,tempEffect,pos);

            return true;       
            break;
    
        case SET_RAINBOW_EFFECT_COMMAND:

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            pos = tempShort.data;

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            speed = tempShort.data;

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            step = tempShort.data;

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            min = tempShort.data;

            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            max = tempShort.data;

            tempEffect.effectId = EFFECT_RAINBOW_ID;
            tempEffect.color.R = 0;
            tempEffect.color.G = 0;
            tempEffect.color.B = 0;

            NOS_WS2812B_Strip_Effect_Init(&tempEffect,&tempEffect.color,speed,step,min,max,tempEffect.effectId);
            NOS_WS2812B_Strip_Effects_UpdateEffect(strip,tempEffect,pos);
            return true;
            break;

            
        case SET_ONE_PIXEL_COLOR_COMMAND:

            //pixelPos 
            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            int tempInt1 = tempShort.data;

            //color
            uint8_t r = message->data[currPos++];
            uint8_t g = message->data[currPos++];
            uint8_t b = message->data[currPos++];

            NOS_WS2812B_Strip_SetPixel(strip,tempInt1,r,g,b);
            return true;
            break;

        case SET_ROW_PIXEL_COLOR_COMMAND:

            //pixelPos
            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            tempInt1 = tempShort.data;

            //pixelCount
            tempShort.bytes[1] = message->data[currPos++];
            tempShort.bytes[0] = message->data[currPos++];
            int tempInt2 = tempShort.data;

            //color
            r = message->data[currPos++];
            g = message->data[currPos++];
            b = message->data[currPos++];

            //fill in row
            for(int i = tempInt1; i < tempInt2; i++)
            {
                    NOS_WS2812B_Strip_SetPixel(strip,i,r,g,b);
            }

            return true;
            break;

        case SET_ALL_PIXEL_ONE_COLOR_COMMAND:

            //color
            r = message->data[currPos++];
            g = message->data[currPos++];
            b = message->data[currPos++];

            //fill all pixels
            for(int i = 0; i < strip->pixelCount; i++)
            {
                NOS_WS2812B_Strip_SetPixel(strip,i,r,g,b);
            }
            return true;
            break;

        case SET_ALL_PIXEL_COMMAND:

                strip->bright = 100;
                strip->effects[0].enabled = false;
                strip->effects[1].enabled = false;
                //Pixel count
                tempShort.bytes[1] = message->data[currPos++];
                tempShort.bytes[0] = message->data[currPos++];
                tempInt1 = tempShort.data;

                if(tempInt1 <= strip->pixelCount)
                {
                    for(int i = 0; i < tempInt1; i++)
                    {
                        r = message->data[currPos++];
                        g = message->data[currPos++];
                        b = message->data[currPos++];

                        NOS_WS2812B_Strip_SetPixel(strip,i,r,g,b);
                    }
                }
            
            return true;
            break;


    default:

        break;
    }

    return false;
}
