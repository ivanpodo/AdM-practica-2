/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "asm_func.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void 	 pack32to16 (int32_t * vectorIn, int16_t * vectorOut, uint32_t longitud);
uint32_t max (int32_t * vectorIn, uint32_t longitud);
void 	 downSample (int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N);
void 	 invertir (uint16_t * vector, uint32_t longitud);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	DWT->CTRL |= 1 << DWT_CTRL_CYCCNTENA_Pos;
	uint32_t ciclos_C, ciclos_ASM;
	int32_t vectorIn[] = {0xffff0000, 0xaaaa0000, 0xbbbb0000};
	int16_t vectorOut[3];

	DWT->CYCCNT = 0;
	pack32to16(vectorIn, vectorOut, 3);
	ciclos_C = DWT->CYCCNT; // 0xa7

	DWT->CYCCNT = 0;
	asm_pack32to16(vectorIn, vectorOut, 3);
	ciclos_ASM = DWT->CYCCNT; // 0x2a (~4 times more efective)

	(void)ciclos_C;		//to clean warning
	(void)ciclos_ASM;	//to clean warning

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void pack32to16(int32_t * vectorIn, int16_t * vectorOut, uint32_t longitud)
{
	if(NULL == vectorIn || NULL == vectorOut)
	{
		return;
	}

	for(uint32_t i = longitud; i > 0U; i--)
	{
		vectorOut[i-1] = (uint16_t)(vectorIn[i-1] >> 16);
	}
}

uint32_t max(int32_t * vectorIn, uint32_t longitud)
{
	if(NULL == vectorIn)
	{
		return 0;
	}

	uint32_t max_pos = 0;
	int32_t max_val = vectorIn[0];

	for(uint32_t i = longitud; i > 0U; i--)
	{
		if(vectorIn[i-1] > max_val)
		{
			max_val = vectorIn[i-1];
			max_pos = (i-1);
		}
	}

	return max_pos;
}

void downSample(int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N)
{
	if(NULL == vectorIn || NULL == vectorOut || 0U == N)
	{
		return;
	}

	uint32_t itOut	= longitud - (longitud / N);

	for(uint32_t itIn = longitud; itIn > 0 ; itIn--)
	{
		if( 0U != (itIn % N))
		{
			vectorOut[itOut - 1] = vectorIn[itIn - 1];
			itOut--;
		}
	}
}

void invertir(uint16_t * vector, uint32_t longitud)
{
	uint16_t temp1 = 0;
	uint16_t temp2 = 0;

	// con esta condición, en el valor central de los impares hace el swap con sigo mismo
	for(uint32_t i = longitud; i > (longitud / 2); i--)
	{
		temp1 = vector[longitud - i];
		temp2 = vector[i - 1];

		vector[longitud - i] = temp2;
		vector[i - 1] 		 = temp1;
	}
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
