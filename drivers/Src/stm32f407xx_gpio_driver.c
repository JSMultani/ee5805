/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: May 20, 2026
 *      Author: Jah Multani
 */

#include "stm32f407xx_gpio_driver.h"

/* Peripheral Clock setup */
/*
 * @brief  enables or disables peri clock for the given GPIO PORT
 * @param
 * 			1. base addr of GPIO peripheral
 * 			2. ENBALE or DISABLE macros
 * @retval  NONE
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
	} else
	{
		//TODO
	}
}


/* Init and De-Init */
/*
 * @brief
 * @param
 * @retval	NONE
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{

}

/*
 * @brief
 * @param
 * @retval	NONE
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){

}


/* Data R/W */
/*
 * @brief
 * @param
 * @retval
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{

}

/*
 * @brief
 * @param
 * @retval
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{

}

/*
 * @brief
 * @param
 * @retval	NONE
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{

}

/*
 * @brief
 * @param
 * @retval	NONE
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{

}

/*
 * @brief
 * @param
 * @retval	NONE
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{

}


/* IRQ Configuration and ISR handling */
/*
 * @brief
 * @param
 * @retval	NONE
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi) // you can also take in IRQ Grouping
{

}

/*
 * @brief
 * @param
 * @retval	NONE
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{

}
