/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NOS_Lib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
extern uint8_t frameBuffer1[3 * 256];
extern uint8_t frameBuffer2[3 * 256];
extern uint8_t frameBuffer3[3 * 256];

NOS_TimeEvent tetrisUpdateEvent = {0};
NOS_TimeEvent screenUpdateEvent = {0};
NOS_TimeEvent uartTestEvent = {0};

WS2812B_Strip stripA = {0};
WS2812B_Strip stripB = {0};
WS2812B_Strip stripC = {0};

PixelColor pixelsA[128];
PixelColor pixelsB[128];
PixelColor pixelsC[128];


NOS_UART_Struct UART2;

int receiveTime = 0;

SinValue bright = {0};

NOS_Button button = {0};

GPIO_PIN PA6 = {0};
GPIO_PIN PA7 = {0};
GPIO_PIN PE3 = {0};
GPIO_PIN PE4 = {0};

int uartPixelCount = 0;

int currColor = 0;
int buttonDelay = 0;

Effect_Struct breatheA = {0};
Effect_Struct rainbowA = {0};

NOS_Short value;

UART_Message lastMessage;

typedef union
{
  Effect_Struct effect;
  uint32_t data[8];
}Effect_Wrap;


Effect_Wrap test1;
Effect_Wrap test2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
bool receive = false;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    NOS_UART_ReceiveHandler(&UART2,huart);
    receive = true;
}

bool tick = false;
bool screenUpdate = false;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_TIM4_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2,UART2.rx_buff,1);
  HAL_UART_Transmit(&huart2,"HELLO",sizeof("HELLO"),1000);
  visInit();

  NOS_WS2812B_Strip_FullInit(&stripA,&frameBuffer1,&pixelsA,128);
  NOS_WS2812B_Strip_FullInit(&stripB,&frameBuffer2,&pixelsB,48);
  NOS_WS2812B_Strip_FullInit(&stripC,&frameBuffer3,&pixelsC,48);

  NOS_TimeEvent_Init(&tetrisUpdateEvent, 50);
  NOS_TimeEvent_Init(&screenUpdateEvent, 20);
  NOS_TimeEvent_Init(&uartTestEvent, 1000);
  //NOS_WS2812B_Strip_TestFill(&stripA);

  NOS_Math_SinValue_Init(&bright,65,75,1);

  NOS_UART_ReceiveAbort(&UART2,&huart2);

  //NOS_WS2812B_Strip_ColorFill(&stripA,red);

  //addr count1 count0 0x50 speed1 speed0 step1 step0 min1 min0 max1 max0 

  //addr1 addr0 channel1 channel0 count1 count0 packetId3 packetId2 packetId1 packetId0 command3 command2 command1 command0 /* data (16 - 1008) */ crc16_1 crc16_0 / 
  //maxLenght 1024B  minLenght 32B packetInfo 16B

  if(NOS_Flash_Validate_Block(FLASH_STORAGE_A,32))
  {
    NOS_Flash_Load_Block(&test1.data,FLASH_STORAGE_A,sizeof(Effect_Struct));
    NOS_WS2812B_Strip_Effect_Breathe_Copy(&breatheA,&test1.effect);
  }
  else
  {
    NOS_WS2812B_Strip_Effect_Breathe_Init(&breatheA,100,1,60,80);
  }

  if(NOS_Flash_Validate_Block(FLASH_STORAGE_B,32))
  {
    NOS_Flash_Load_Block(&test2.data,FLASH_STORAGE_B,sizeof(Effect_Struct));
    NOS_WS2812B_Strip_Effect_Rainbow_Copy(&rainbowA,&test2.effect);
  }
  else
  {
    NOS_WS2812B_Strip_Effect_Rainbow_Init(&rainbowA,1000,1,200,800);
  }

  
  NOS_WS2812B_Strip_Effects_AddEffect(&stripA,breatheA);
  NOS_WS2812B_Strip_Effects_AddEffect(&stripB,breatheA);
  NOS_WS2812B_Strip_Effects_AddEffect(&stripC,breatheA);

  NOS_WS2812B_Strip_Effects_AddEffect(&stripA,rainbowA);
  NOS_WS2812B_Strip_Effects_AddEffect(&stripB,rainbowA);
  NOS_WS2812B_Strip_Effects_AddEffect(&stripC,rainbowA);

  NOS_WS2812B_Strip_ColorFill(&stripA,NOS_GetBaseColor(RED));
  NOS_WS2812B_Strip_ColorFill(&stripB,NOS_GetBaseColor(RED));
  NOS_WS2812B_Strip_ColorFill(&stripC,NOS_GetBaseColor(RED));  

  NOS_WS2812B_Strip_Update(&stripA);
  NOS_WS2812B_Strip_Update(&stripB);
  NOS_WS2812B_Strip_Update(&stripC);
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {    
    
    if(tick)
    {
          UART2.lastReceivedByteTime++;

          if(UART2.startReceive)
          {
            UART2.receiveTime++;
          }

          if(UART2.startReceive && UART2.lastReceivedByteTime > 5)
          {
                UART2.index = 0;
                UART2.receiveTime = 0;
                NOS_UART_ReceiveReset(&UART2);
          }

          NOS_TimeEvent_TickHandler(&screenUpdateEvent);
          NOS_TimeEvent_TickHandler(&tetrisUpdateEvent);

          NOS_WS2812B_Strip_Effects_Handler(&stripA);
          NOS_WS2812B_Strip_Effects_Handler(&stripB);
          NOS_WS2812B_Strip_Effects_Handler(&stripC);
          tick = false;
    }

    if(NOS_UART_CheckReceive(&UART2))
    {
      if(NOS_UART_ParsePacket(&UART2,&lastMessage))
      {
      
      switch (lastMessage.channel.data)
      {
        //Global leds strips commands
        case 0x01:
        
          NOS_Strip_UART_ParseCommand(&stripA,&lastMessage);
          NOS_Strip_UART_ParseCommand(&stripB,&lastMessage);
          NOS_Strip_UART_ParseCommand(&stripC,&lastMessage);

          NOS_Flash_Save_Block(&stripA.effects[0],FLASH_STORAGE_A,32);
          NOS_Flash_Save_Block(&stripA.effects[1],FLASH_STORAGE_B,32);

        break;

        //stripA commands
        case 0x02:

          NOS_Strip_UART_ParseCommand(&stripA,&lastMessage);
        
        break;


        //stripB commands
        case 0x03:

          NOS_Strip_UART_ParseCommand(&stripB,&lastMessage);
              
        break;

        //stripC commands
        case 0x04:

        NOS_Strip_UART_ParseCommand(&stripC,&lastMessage);
      
        break;

          default:
          break;
        }
      }
      /*
      switch (NOS_UART_GetReceivedData(&UART2)[0])
      {
        case 0x64:
          NOS_Strip_Uart_ParseCommand(&stripA,NOS_UART_GetReceivedData(&UART2));
          NOS_Strip_Uart_ParseCommand(&stripB,NOS_UART_GetReceivedData(&UART2));
          NOS_Strip_Uart_ParseCommand(&stripC,NOS_UART_GetReceivedData(&UART2));

          break;

      case 0x65:
              if(NOS_Strip_Uart_ParseCommand(&stripA,NOS_UART_GetReceivedData(&UART2)) == "ready!")
              {
                  HAL_UART_Transmit(&huart2,"ready!",7,1000);
              }
        break;

      case 0x66:
              if(NOS_Strip_Uart_ParseCommand(&stripB,NOS_UART_GetReceivedData(&UART2)) == "ready!")
              {
                  //HAL_UART_Transmit(&huart2,"ready!",7,1000);
              }
              break;

      case 0x67:
              if(NOS_Strip_Uart_ParseCommand(&stripC,NOS_UART_GetReceivedData(&UART2)) == "ready!")
              {
                 //HAL_UART_Transmit(&huart2,"ready!",7,1000);
              }
      break;


      default:
        break;
      }
      */
      
      NOS_UART_EndReceive(&UART2);
      HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
      
      NOS_UART_PacketApprovedNotice(&lastMessage,&huart2);
      
      NOS_WS2812B_Strip_Update(&stripA);
      NOS_WS2812B_Strip_Update(&stripB);
      NOS_WS2812B_Strip_Update(&stripC);
      visHandle();
      
    }

    if (NOS_TimeEvent_Check(&tetrisUpdateEvent))
    { 
      //NOS_Flash_Load_Block(&test2.data,FLASH_STORAGE_A,sizeof(Effect_Struct));

      NOS_TimeEvent_FinishEvent(&tetrisUpdateEvent);
    }

    if (NOS_TimeEvent_Check(&screenUpdateEvent))
    {
      
      NOS_WS2812B_Strip_Update(&stripA);
      NOS_WS2812B_Strip_Update(&stripB);
      NOS_WS2812B_Strip_Update(&stripC);
      visHandle();
      

      NOS_TimeEvent_FinishEvent(&screenUpdateEvent);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_OC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 460800;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  tick = true;
  /* USER CODE END SysTick_IRQn 1 */
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
