/**
  ******************************************************************************
  * @file     STM32Blink
  * @author   A. Nerovny
  * @brief    Working STM32 BluePill PlatformIO "Blink" project template using HAL.
              Toggles onboard PC13 LED on and off.
  ******************************************************************************
  */

#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"


#define LED_PIN                                GPIO_PIN_13
#define LED_GPIO_PORT                          GPIOC
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOC_CLK_ENABLE()

void SystemClock_Config(void);
void _Error_Handler(const char* file, int line);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  LED_GPIO_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-pull mode
  GPIO_InitStruct.Pull = GPIO_NOPULL; // No pullup or pulldown
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct); 

  while (1)
  {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    HAL_Delay(1000);
  }

  return 0;
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  //Configure the SysTick frequency, clock source and interrupt priority
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

void _Error_Handler(const char* file, int line) {
	//Error occured.
}

// We must hook the SysTick in order for HAL_Delay to work
extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}