/*
 * debounce.c
 *
 *  Created on: Apr 11, 2020
 *      Author: Deep Sanghvi
 *
 */

#include <stdint.h>
#include "stm32l4xx_hal.h"


void deBounceInit(uint16_t pin, char port, int8_t mode) {
	GPIO_InitTypeDef GPIO_InitStruct;
	switch (pin) {
	case 0:
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		break;
	case 1:
		GPIO_InitStruct.Pin = GPIO_PIN_1;
		break;
	case 2:
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		break;
	case 3:
		GPIO_InitStruct.Pin = GPIO_PIN_3;
		break;
	case 4:
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		break;
	case 5:
		GPIO_InitStruct.Pin = GPIO_PIN_5;
		break;
	case 6:
		GPIO_InitStruct.Pin = GPIO_PIN_6;
		break;
	case 7:
		GPIO_InitStruct.Pin = GPIO_PIN_7;
		break;
	case 8:
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		break;
	case 9:
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		break;
	case 10:
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		break;
	case 11:
		GPIO_InitStruct.Pin = GPIO_PIN_11;
		break;
	case 12:
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		break;
	case 13:
		GPIO_InitStruct.Pin = GPIO_PIN_13;
		break;
	case 14:
		GPIO_InitStruct.Pin = GPIO_PIN_14;
		break;
	case 15:
		GPIO_InitStruct.Pin = GPIO_PIN_15;
		break;
	}
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	switch (mode) {
	case 0:
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		break;
	case 1:
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		break;
	}
	switch (port) {
	case 'A':
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		break;
	case 'B':
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		break;
	case 'C':
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		break;
	}

}

int8_t deBounceReadPin(uint16_t pin, char port, int8_t mode) {
	int8_t pinStateWeAreLookingFor;
	GPIO_PinState pinState = GPIO_PIN_RESET;
	int8_t stableInterval = 50;
	int32_t msTimeStamp=HAL_GetTick();
	switch (port) {
	case 'A':
		pinState = HAL_GPIO_ReadPin(GPIOA, pin);
		break;
	case 'B':
		pinState = HAL_GPIO_ReadPin(GPIOB, pin);
		break;
	case 'C':
		pinState = HAL_GPIO_ReadPin(GPIOC, pin);
		break;
	}
	if (pinState == GPIO_PIN_RESET) {
		pinStateWeAreLookingFor = 0;

	} else {
		pinStateWeAreLookingFor = 1;

	}
	while(HAL_GetTick()<(msTimeStamp+stableInterval)){
		switch (port) {
		case 'A':
				pinState = HAL_GPIO_ReadPin(GPIOA, pin);
				break;
			case 'B':
				pinState = HAL_GPIO_ReadPin(GPIOB, pin);
				break;
			case 'C':
				pinState = HAL_GPIO_ReadPin(GPIOC, pin);
				break;
		}
		if (pinState!=pinStateWeAreLookingFor) {
			pinStateWeAreLookingFor=!pinStateWeAreLookingFor;
			msTimeStamp=HAL_GetTick();
		}
	}
	return (pinStateWeAreLookingFor);

}


