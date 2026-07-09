/*
 * app_delay.c
 *
 * Polling SysTick delay used by the first ILI9341 solid-color test.
 * The current project is still using the default early clock setup, so this
 * code assumes the Cortex-M processor clock is 16 MHz HSI.
 */

#include "app_delay.h"

#define SYSTICK_CTRL      (*((volatile uint32_t *)0xE000E010U))
#define SYSTICK_LOAD      (*((volatile uint32_t *)0xE000E014U))
#define SYSTICK_VAL       (*((volatile uint32_t *)0xE000E018U))

#define SYSTICK_ENABLE    (1U << 0)
#define SYSTICK_CLKSOURCE (1U << 2)
#define SYSTICK_COUNTFLAG (1U << 16)

#define CPU_CLOCK_HZ      16000000U
#define SYSTICK_1MS_LOAD  ((CPU_CLOCK_HZ / 1000U) - 1U)

void app_delay_init(void)
{
	SYSTICK_LOAD = SYSTICK_1MS_LOAD;
	SYSTICK_VAL = 0;
	SYSTICK_CTRL = SYSTICK_CLKSOURCE | SYSTICK_ENABLE;
}

void app_delay_ms(uint32_t delay_ms)
{
	while(delay_ms > 0)
	{
		while((SYSTICK_CTRL & SYSTICK_COUNTFLAG) == 0)
		{
			/* Wait for the next 1 ms SysTick wrap. */
		}
		delay_ms--;
	}
}
