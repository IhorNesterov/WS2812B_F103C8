#include "NOS_Strip_UART.h"
#include "NOS_WS2812B_Strip.h"

/*
const char* NOS_Strip_Uart_ParseCommand(WS2812B_Strip* strip,uint8_t* command)
{
    NOS_Short tempShort;
    int currPos = 4;
    int tempInt1,tempInt2;
    uint8_t r,g,b;

    if(command != NULL)
    {
            switch (command[3])
            {
            case CHECK_CONNECTION:
                return "ready!";
            
            case SET_ONE_PIXEL_COLOR_COMMAND:

                //pixelPos 
                tempShort.bytes[1] = command[4];
                tempShort.bytes[0] = command[5];
                tempInt1 = tempShort.data;

                //color
                r = command[6];
                g = command[7];
                b = command[8];

                NOS_WS2812B_Strip_SetPixel(strip,tempInt1,r,g,b);
                break;

            case SET_ROW_PIXEL_COLOR_COMMAND:

                //pixelPos
                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                tempInt1 = tempShort.data;

                //pixelCount
                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                tempInt2 = tempShort.data;

                //color
                r = command[currPos++];
                g = command[currPos++];
                b = command[currPos++];

                //fill in row
                for(int i = tempInt1; i < tempInt2; i++)
                {
                    NOS_WS2812B_Strip_SetPixel(strip,i,r,g,b);
                }

                break;

            case SET_ALL_PIXEL_ONE_COLOR_COMMAND:

                //color
                r = command[currPos++];
                g = command[currPos++];
                b = command[currPos++];

                //fill all pixels
                for(int i = 0; i < strip->pixelCount; i++)
                {
                    NOS_WS2812B_Strip_SetPixel(strip,i,r,g,b);
                }

                break;

            case SET_BRIGHT_COMMAND:

                NOS_WS2812B_Strip_SetBright(strip,command[currPos++]);

                break;

            case SET_ALL_PIXEL_COMMAND:

                strip->bright = 100;
                strip->effects[0].enabled = false;
                strip->effects[1].enabled = false;
                //Pixel count
                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                tempInt1 = tempShort.data;

                if(tempInt1 <= strip->pixelCount)
                {
                    for(int i = 0; i < tempInt1; i++)
                    {
                        r = command[currPos++];
                        g = command[currPos++];
                        b = command[currPos++];

                        NOS_WS2812B_Strip_SetPixel(strip,i,r,g,b);
                    }
                }

            case SET_BREATHE_EFFECT_COMMAND:

                uint8_t pos = command[currPos++];

                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                uint16_t speed = tempShort.data;

                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                uint16_t step = tempShort.data;

                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                uint16_t min = tempShort.data;

                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                uint16_t max = tempShort.data;

                Effect_Struct tempEffect = {0};
                tempEffect.effectId = EFFECT_BREATHE_ID;
                NOS_WS2812B_Strip_Effect_Breathe_Init(&tempEffect,speed,step,min,max);
                NOS_WS2812B_Strip_Effects_UpdateEffect(strip,tempEffect,pos);

                break;

                case SET_RAINBOW_EFFECT_COMMAND:

                pos = command[currPos++];

                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                speed = tempShort.data;

                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                step = tempShort.data;

                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                min = tempShort.data;

                tempShort.bytes[1] = command[currPos++];
                tempShort.bytes[0] = command[currPos++];
                max = tempShort.data;

                tempEffect.effectId = EFFECT_RAINBOW_ID;
                NOS_WS2812B_Strip_Effect_Rainbow_Init(&tempEffect,speed,step,min,max);
                NOS_WS2812B_Strip_Effects_UpdateEffect(strip,tempEffect,pos);

                break;

                case SET_EFFECT_ACTIVE:

                pos = command[currPos++];

                uint8_t state = command[currPos++];

                if(state == 0xFF)
                {
                    strip->effects[pos].enabled = true;
                }
                else
                {
                    strip->effects[pos].enabled = false;
                }

                break;
                      
            default:
                return false;
                break;
            }


        }
}
*/

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
            NOS_WS2812B_Strip_Effect_Breathe_Init(&tempEffect,speed,step,min,max);
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
            NOS_WS2812B_Strip_Effect_Rainbow_Init(&tempEffect,speed,step,min,max);
            NOS_WS2812B_Strip_Effects_UpdateEffect(strip,tempEffect,pos);
            return true;
            break;

    default:

        break;
    }

    return false;
}
