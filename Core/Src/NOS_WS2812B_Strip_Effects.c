#include "NOS_WS2812B_Strip_Effects.h"
#include "NOS_WS2812B_Strip.h"

void NOS_WS2812B_Strip_Effects_AddEffect(WS2812B_Strip* strip,Effect_Struct effect)
{
    if(strip->effectsCounter < 10)
    {
        switch(effect.effectId)
        {
            case 0x20:
            NOS_WS2812B_Strip_Effect_Init(&strip->effects[strip->effectsCounter],&effect.color,effect.speed.data,effect.step.data,effect.minValue,effect.maxValue,effect.effectId);
                break;
            case 0x21:
            NOS_WS2812B_Strip_Effect_Init(&strip->effects[strip->effectsCounter],&effect.color,effect.speed.data,effect.step.data,effect.minValue,effect.maxValue,effect.effectId);
                break;
                case 0x22:
                NOS_WS2812B_Strip_Effect_Init(&strip->effects[strip->effectsCounter],&effect.color,effect.speed.data,effect.step.data,effect.minValue,effect.maxValue,effect.effectId);
                    break;
        }

        strip->effectsCounter++;
    }
}

void NOS_WS2812B_Strip_Effects_Handler(WS2812B_Strip* strip)
{
    for(int i = 0; i < strip->effectsCounter; i++)
    {
        switch(strip->effects[i].effectId)
        {
            case 0x20:
                NOS_WS2812B_Strip_Effect_Breathe_Handler(strip,&strip->effects[i]);
                break;
            case 0x21:
                NOS_WS2812B_Strip_Effect_Rainbow_Handler(strip,&strip->effects[i]);
                break;
            case 0x22:
                NOS_WS2812B_Strip_Effect_Dots_Handler(strip,&strip->effects[i]);
                break;
        }
    }
}

void NOS_WS2812B_Strip_Effects_UpdateEffect(WS2812B_Strip* strip,Effect_Struct effect,uint8_t pos)
{
    if(pos < 10)
    {
        switch(effect.effectId)
        {
            case 0x20:
                NOS_WS2812B_Strip_Effect_Init(&strip->effects[pos],&effect.color,effect.speed.data,effect.step.data,effect.minValue,effect.maxValue,effect.effectId);
                break;
            case 0x21:
                NOS_WS2812B_Strip_Effect_Init(&strip->effects[pos],&effect.color,effect.speed.data,effect.step.data,effect.minValue,effect.maxValue,effect.effectId);
                break;
            case 0x22:
                NOS_WS2812B_Strip_Effect_Init(&strip->effects[pos],&effect.color,effect.speed.data,effect.step.data,effect.minValue,effect.maxValue,effect.effectId);
                break;
        }
    }
}

void NOS_WS2812B_Strip_Effect_Init(Effect_Struct* effect,PixelColor* color,uint16_t speed,uint16_t step,uint16_t minValue,uint16_t maxValue,uint8_t effectID)
{
        NOS_Math_SinValue_Init(&effect->value,minValue,maxValue,step);
        effect->minValue = minValue;
        effect->maxValue = maxValue;
        effect->speed.data = speed;
        effect->step.data = step;
        effect->enabled = true;
        NOS_PixelColor_SetColor(&effect->color,color->R,color->G,color->B);
        effect->effectId = effectID;   
}

void NOS_WS2812B_Strip_Effect_Copy(Effect_Struct* destination,Effect_Struct* source)
{
    NOS_Math_SinValue_Init(&destination->value,source->minValue,source->maxValue,source->step.data);
    destination->minValue = source->minValue;
    destination->maxValue = source->maxValue;
    destination->speed.data = source->speed.data;
    destination->step.data = source->step.data;
    destination->enabled = true;
    destination->effectId = source->effectId;   
}

void NOS_WS2812B_Strip_Effect_Breathe_Handler(WS2812B_Strip* strip,Effect_Struct* effect)
{
    if(strip != NULL)
    {
        if(effect != NULL)
        {
            if(effect->enabled)
            {
                if(effect->timer > effect->speed.data)
                {
                    NOS_Math_SinValue_Handler(&effect->value);
                    NOS_WS2812B_Strip_SetBright(strip,effect->value.value);
                    NOS_WS2812B_Strip_Update(strip);
                    effect->timer = 0;
                }
                effect->timer++;
            }
        }
    }
}

void NOS_WS2812B_Strip_Effect_Rainbow_Handler(WS2812B_Strip* strip,Effect_Struct* effect)
{
    if(strip != NULL)
    {
        if(effect != NULL)
        {
            if(effect->enabled)
            {
                if(effect->timer > effect->speed.data)
                {
                    NOS_Math_SinValue_Handler(&effect->value);

                    uint16_t number = NOS_Math_GetSinValue(&effect->value);
                    uint8_t iteration = number / 0x7F;

                    PixelColor curr = { 0,0,0 };
                    number = number - 0x7F * iteration;

                    switch (iteration)
                    {
                        case 0:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.G = number;
                        break;

                        case 1:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.G = 0x7F + number;
                        break;

                        case 2:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.B = 0xFF - number;
                        break;

                        case 3:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.B = 0x7F - number;
                        break;

                        case 4:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.R = number;
                        break;

                        case 5:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.R = 0x7F + number;
                        break;

                        case 6:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.G = 0xFF - number;
                        break;

                        case 7:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.G = 0x7F - number;
                        break;

                        case 8:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.B = number;
                        break;

                        case 9:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.B = 0x7F + number;
                        break;

                        case 10:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.R = 0xFF - number;
                        break;

                        case 11:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.R = 0x7F - number;
                        break;

                        default:
                        break;
                    }
                    

                    NOS_WS2812B_Strip_ColorFill(strip,curr);
                    NOS_WS2812B_Strip_Update(strip);
                    effect->timer = 0;
                }
                effect->timer++;
            }
        }
    }
}

void NOS_WS2812B_Strip_Effect_Dots_Handler(WS2812B_Strip* strip,Effect_Struct* effect)
{
	uint32_t i;
	PixelColor temp = {0,0,0};
	bool isR;
	bool isG;
	bool isB;
    uint32_t random = 50;
    uint32_t fadeOutFactor = 40;
    PixelColor color = {0,0,0};

    NOS_PixelColor_SetColor(&color,effect->color.R,effect->color.G,effect->color.B);

    if(effect->timer >= effect->speed.data)
    {

	if(color.R == 0xFF)
	{
		isR = true;
	}
	else
	{
		isR = false;
	}

	if(color.G == 0xFF)
	{
		isG = true;
	}
	else
	{
		isG = false;
	}
	if(color.B == 0xFF)
	{
		isB = true;
	}
	else
	{
		isB = false;
	}

	for( i = 0; i < strip->pixelCount; i++)
	{
        temp.R = strip->pixels[i].R;
        temp.G = strip->pixels[i].G;
        temp.B = strip->pixels[i].B;

		if(rand() % random == 0)
		{	
			if(isR)
			{
				temp.R = 255;
			}
			
			if(isG)
			{
				temp.G = 255;
			}

			if(isB)
			{
				temp.B = 255;
			}
		}

		if(isR)
		{
		if(temp.R > fadeOutFactor)
			temp.R -= temp.R/fadeOutFactor;
		else
			temp.R = 0;
		}

		if(isG)
		{
		if(temp.G > fadeOutFactor)
			temp.G -= temp.G/fadeOutFactor;
		else
			temp.G = 0;
		}

		if(isB)
		{
		if(temp.B > fadeOutFactor)
			temp.B -= temp.B/fadeOutFactor;
		else
			temp.B = 0;
		}

        NOS_WS2812B_Strip_SetPixelByPixelColor(strip,i,temp);
	}
    effect->timer = 0;
}
effect->timer++;
}
