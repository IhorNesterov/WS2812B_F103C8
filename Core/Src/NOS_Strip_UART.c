#include "NOS_Strip_UART.h"
#include "NOS_WS2812B_Strip.h"

bool NOS_Strip_UART_ParseCommand(WS2812B_Strip* strip,UART_Message* message)
{
    int currPos = 0;
    NOS_Short tempShort;
    Effect_Struct tempEffect = {0};
    PixelColor color = NOS_GetBaseColor(GREEN);
    NOS_WS2812B_Strip_Effect_Init(&tempEffect,&color,0,0,0,0,0,0,0,0,0,0);

    switch (message->command.data)
    {
        case SET_STRIP_PIXEL_COUNT:

        tempShort.bytes[1] = message->data[currPos++];
        tempShort.bytes[0] = message->data[currPos++];


        if(tempShort.data > 0 && tempShort.data <= 512)
        {
            NOS_WS2812B_Strip_Clear(strip);
            strip->pixelCount = tempShort.data;
        }
        
        return true;
        break;

    case SET_EFFECT_BREATHE:

            if(message->data[currPos++] == EFFECT_BREATHE_ID)
            {
                tempEffect.effectId = EFFECT_BREATHE_ID;
            }

            if(message->data[currPos++] == 0xFF)
            {
                tempEffect.enabled = true;
            }
            else
            {
                tempEffect.enabled = false;
            }

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

            tempEffect.color.R = message->data[currPos++];
            tempEffect.color.G = message->data[currPos++];
            tempEffect.color.B = message->data[currPos++];

            NOS_WS2812B_Strip_Effect_Init(&tempEffect,&tempEffect.color,speed,step,min,max,0,0,0,0,tempEffect.effectId,tempEffect.enabled);
            NOS_WS2812B_Strip_Effects_UpdateEffect(strip,tempEffect);

            return true;       
            break;
    
        case SET_EFFECT_RAINBOW:

            if(message->data[currPos++] == EFFECT_RAINBOW_ID)
            {
                tempEffect.effectId = EFFECT_RAINBOW_ID;
            }

            if(message->data[currPos++] == 0xFF)
            {
                tempEffect.enabled = true;
            }
            else
            {
                tempEffect.enabled = false;
            }

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

            tempEffect.color.R = message->data[currPos++];
            tempEffect.color.G = message->data[currPos++];
            tempEffect.color.B = message->data[currPos++];

            NOS_WS2812B_Strip_Effect_Init(&tempEffect,&tempEffect.color,speed,step,min,max,0,0,0,0,tempEffect.effectId,tempEffect.enabled);
            NOS_WS2812B_Strip_Effects_UpdateEffect(strip,tempEffect);
            return true;
            break;

        case SET_EFFECT_DOTS:

            if(message->data[currPos++] == EFFECT_DOTS_ID)
            {
                tempEffect.effectId = EFFECT_DOTS_ID;
            }

            if(message->data[currPos++] == 0xFF)
            {
                tempEffect.enabled = true;
            }
            else
            {
                tempEffect.enabled = false;
            }

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

            tempEffect.color.R = message->data[currPos++];
            tempEffect.color.G = message->data[currPos++];
            tempEffect.color.B = message->data[currPos++];

            NOS_WS2812B_Strip_Effect_Init(&tempEffect,&tempEffect.color,speed,step,min,max,0,0,0,0,tempEffect.effectId,tempEffect.enabled);
            NOS_WS2812B_Strip_Effects_UpdateEffect(strip,tempEffect);

            return true;       
            break;

        case SET_EFFECT_WALKING_PIXELS:
            
            if(message->data[currPos++] == EFFECT_DOTS_ID)
            {
                tempEffect.effectId = EFFECT_DOTS_ID;
            }

            if(message->data[currPos++] == 0xFF)
            {
                tempEffect.enabled = true;
            }
            else
            {
                tempEffect.enabled = false;
            }

            tempEffect.color.R = message->data[currPos++];
            tempEffect.color.G = message->data[currPos++];
            tempEffect.color.B = message->data[currPos++];

            NOS_WS2812B_Strip_Effect_Init(&tempEffect,&tempEffect.color,speed,step,min,max,0,0,0,0,tempEffect.effectId,tempEffect.enabled);
            NOS_WS2812B_Strip_Effects_UpdateEffect(strip,tempEffect);


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
