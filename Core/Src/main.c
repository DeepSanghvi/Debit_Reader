/* USER CODE BEGIN Header */
/**

/* FILE				:		Dsanghvi_assignment_4.c

 * PROJECT			:		PROG8125 - Assignment #4

 * PROGRAMMER		:		Deep Sanghvi

 *FIRST VERSION		:		2020-04-11

 *DESCRIPTION		:		The purpose of this project is to create a debitcard reader machine using stm32 board.
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
#include "main.h"								//
#include "stdio.h"								//header files
#include "stm32l4xx_hal.h"						//
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

int OKBTN() {													//Function for OK button pressed
	int btn1 = 1, btn2 = 1;

	deBounceInit(GPIO_PIN_3, 'A', 1);							//Using debounce function for buttons
	deBounceInit(GPIO_PIN_4, 'A', 1);

	for (int var = 0; btn1 == 1 || btn2 == 1; var++) 			//for loop to check which button is pressed
	{
		btn1 = deBounceReadPin(GPIO_PIN_3, 'A', 1);
		btn2 = deBounceReadPin(GPIO_PIN_4, 'A', 1);
		if (btn1 == 0 || btn2 == 0) 							//if else condition to perform action according to btn pressed
		{
			break;
		}
	}if (btn1 == 0)
		return 1;
	else
		return 0;
}

int CHKIN() {													//Function to check for the account type
	int btn1 = 1, btn2 = 1;

	deBounceInit(GPIO_PIN_3, 'A', 1);
	deBounceInit(GPIO_PIN_4, 'A', 1);

	for (int var = 0; btn1 == 1 || btn2 == 1; var++)			//for loop for keeping button off
	{
		btn1 = deBounceReadPin(GPIO_PIN_3, 'A', 1);
		btn2 = deBounceReadPin(GPIO_PIN_4, 'A', 1);

		if (btn1 == 0 || btn2 == 0) {							//if else condition to perform action according to btn pressed
			break;
		}
	}if (btn1 == 0)
		return 1;
	else
		return 0;
}

int passcode() {												//Function to check id passcode is correct or not
	int btn1 = 1, btn2 = 1, btn3 = 1;

	deBounceInit(GPIO_PIN_3, 'A', 1);
	deBounceInit(GPIO_PIN_4, 'A', 1);
	deBounceInit(GPIO_PIN_5, 'A', 1);

	for (int var = 0; btn1 == 1 || btn2 == 1 || btn3 == 1; var++)
	{
		btn1 = deBounceReadPin(GPIO_PIN_3, 'A', 1);
		btn2 = deBounceReadPin(GPIO_PIN_4, 'A', 1);
		btn3 = deBounceReadPin(GPIO_PIN_5, 'A', 1);

		if (btn1 == 0 || btn2 == 0 || btn3 == 0) {				//if else condition to perform action according to btn pressed
			break;
		}
	}if (btn1 == 0) {
		return 1;
	} else if (btn2 == 0) {
		return 2;
	} else {
		return 3;
	}
}
int main(void) {

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	HD44780_Init();

	int process = 0;
	int accounts[10] = { 1111, 1121, 1133, 1213, 2111, 2331, 3111, 3211, 3333 };	//array of passcodes
	float checking = 2000;
	float savings = 1000;
	int chkin = 0, savin = 0;							//initialize to zero
	float acc = 0;										//initialize to 0
	int pin = 0;

	while (1) {

		switch (process) {								//switch case for using each case of process

		case 0:
			HD44780_ClrScr();
			HD44780_GotoXY(0, 1);						//specify the position to be displayed on lcd
			HD44780_PutStr("Welcome");
			printf("Welcome--> Enter the amount to continue  \r \n");				//welcome screen on putty
			scanf("%f", &acc);
			process = 1;
		break;

		case 1:
				HD44780_ClrScr();
				char row[16];
				HD44780_GotoXY(0, 0);
				sprintf(row, "%0.1f $", acc);
				HD44780_PutStr(row);									//puts the values on one row
				HD44780_GotoXY(0, 1);
				HD44780_PutStr("OK or Cancel ?");						//Press OK to continue and cancel to terminate
				if (OKBTN()) {
					process = 2;
				} else {
					process = 7;
				}
		break;

		case 2:
				HD44780_ClrScr();
				HD44780_GotoXY(0, 0);
				HD44780_PutStr("CHK or SAV ?");							//select checking or savings account
				if (CHKIN()) {
					chkin = 1;
				} else {
					savin = 1;
				}
				process = 3;
		break;

		case 3:
				HD44780_ClrScr();
				HD44780_GotoXY(0, 0);
				HD44780_PutStr("Enter pin...");			//check for the passcode
				for (int i = 0; i <= 3; i++) {
					pin = pin * 10 + passcode();		//if passcode is correct go to next screen
					HD44780_GotoXY(i , 1);
					HD44780_PutStr("*");     			//puts * in place of password

				}
				HD44780_ClrScr();
				HD44780_GotoXY(0, 0);
				HD44780_PutStr("OK or Cancel ?");			//select ok or cancel button
				if (OKBTN()) {
					process = 4;
				} else {
					process = 7;
				}
		break;

		case 4:
				HD44780_ClrScr();
				HD44780_GotoXY(0, 1);
				HD44780_PutStr("processing...");					//if passcode ok it will continue
				HAL_Delay(1000);
				for (int j = 0; j < 10; j++) {						//for loop checking the array of passcodes
					if (pin == accounts[j]) {
						process = 5;
						break;
					} else {
						process = 6;
					}
				}
		break;

		case 5:
				if (chkin == 1 && checking >= acc && pin != 3333) {
					checking = checking - acc;
					printf("Transaction Successful \r \n");
					HD44780_ClrScr();
					HD44780_GotoXY(0, 0);
					HD44780_PutStr("Transaction Successful");			//if passcode ok transaction will be completed
				} else if (savin == 1 && savings >= acc && pin != 3333) {
					savings = savings - acc;
					printf("Transaction Success \r \n");
					HD44780_ClrScr();
					HD44780_GotoXY(0, 0);
					HD44780_PutStr("Transaction Success");
				} else {
					printf("Insufficient Balance \r \n");				//if balance is low error will be displayed
					HD44780_ClrScr();
					HD44780_GotoXY(0, 0);
					HD44780_PutStr("Transaction Declined");
				}
				HAL_Delay(3200);
				process=7;
		break;

		case 6:
				printf("Incorrect password \r \n");
				HD44780_ClrScr();
				HD44780_GotoXY(0, 0);
				HD44780_PutStr("Transaction Declined");				//if passcode incorrect it will terminate
				HAL_Delay(3000);
				process=7;
		break;

		case 7:
				HD44780_ClrScr();
				HD44780_GotoXY(0, 0);
				HD44780_PutStr("Thank You");						//display thankyou message
				HAL_Delay(2000);
				process=8;
		break;

		case 8:
				chkin = 0;
				savin = 0;
				acc = 0;
				pin = 0;
				process=0;
		break;
			}
		}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE
			| RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 16;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}
	/** Enable MSI Auto calibration
	 */
	HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
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
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA3 PA4 PA5 PA6 */
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : LD3_Pin */
	GPIO_InitStruct.Pin = LD3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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

/************************ (C) COPYRIGHT STMicroelecntonics *****END OF FILE****/
