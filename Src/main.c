/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "lsm303d.h"
#include "stdio.h"
#include "math.h"
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
int buff_temp[30];
int buff_acc_1[30];
int long buff_acc_2[30];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void send_char(char c)									// do wysyłania printf na uart
{
 HAL_UART_Transmit(&huart2, (uint8_t*)&c, 1, 1000);
}

int __io_putchar(int ch)
{
 if (ch == '\n')
 send_char('\r');
 send_char(ch);
 return ch;
}
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  void send_string(char* s)
    	{
    	HAL_UART_Transmit(&huart2, (uint8_t*)s, strlen(s), 100);
    	}

  send_string("wyszukiwanie akcelerometru...\r\n");
  uint8_t who_am_i = lsm_read_reg(0xf);
  if(who_am_i == 0x49)
  {
	  send_string("znaleziono akcelerometr\r\n");
  }
  else
  {
	  send_string("niepoprawna odpowiedź układu (0x%02X)\r\n");
  }

lsm_write_reg(LSM303D_CTRL5, 0x80 | 0x10);			// odczyt rejestru temperatury mało precyzyjny

lsm_write_reg(LSM303D_CTRL1, 0x40 | 0x07);

HAL_Delay(100);

  while (1)
  {

	//	wysyłanie temperatury na terminal
	  int16_t temp = lsm_read_value(LSM303D_TEMP_OUT);
	//  sprintf(buff_temp, "Temp = %d\r\n", temp);
	//  HAL_UART_Transmit(&huart2, buff_temp, strlen(buff_temp), 100);
	//  HAL_Delay(1000);
	printf("Temp = %d\r\n", temp);

	  int16_t a_x = lsm_read_value(LSM303D_OUT_X_A);
	  int16_t a_y = lsm_read_value(LSM303D_OUT_Y_A);
	  int16_t a_z = lsm_read_value(LSM303D_OUT_Z_A);
//	  sprintf(buff_acc_1, "X = %d	Y = %d	Z = %d\n\r", a_x, a_y, a_z);
//	  HAL_UART_Transmit(&huart2, buff_acc_1, strlen(buff_acc_1), 100);
	  printf("X = %d	Y = %d	Z = %d\n\r", a_x, a_y, a_z);				// położenie akcelerometru



	  float x = a_x * 2.0f / 32678.0f;
	  float y = a_y * 2.0f / 32678.0f;
	  float z = a_z * 2.0f / 32678.0f;
//	  printf("X = %.2f		Y = %.2f	Z = %.2f\n\r", a_x, a_y, a_z);		// przeciążenia g ?
	//  HAL_UART_Transmit(&huart2, buff_acc_2, strlen(buff_acc_2), 100);

	  float alpha = atan2f(z, x);
//	  printf("alpha = %.2f\n", alpha * 180.0f / M_PI + 90.0f);				// poziomica

	  HAL_Delay(1000);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
