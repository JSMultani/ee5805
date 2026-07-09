/*
 * app_delay.h
 *
 * Small application-level delay helper for the TFT bring-up test.
 * This project does not currently have a separate delay driver, so this
 * module uses the Cortex-M SysTick timer in polling mode to create a simple
 * millisecond delay without interrupts, HAL, LL, or generated code.
 */

#ifndef APP_INC_APP_DELAY_H_
#define APP_INC_APP_DELAY_H_

#include <stdint.h>

void app_delay_init(void);
void app_delay_ms(uint32_t delay_ms);

#endif /* APP_INC_APP_DELAY_H_ */
