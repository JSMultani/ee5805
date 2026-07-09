/*
 * ili9341.h
 *
 * Minimal ILI9341 TFT display interface for the first SPI bring-up test.
 * This module only supports initialization, setting an address window, and
 * filling the full 240x320 display with a solid RGB565 color.
 */

#ifndef APP_INC_ILI9341_H_
#define APP_INC_ILI9341_H_

#include <stdint.h>

#define ILI9341_WIDTH       240U
#define ILI9341_HEIGHT      320U

#define ILI9341_BLACK       0x0000U
#define ILI9341_RED         0xF800U
#define ILI9341_GREEN       0x07E0U
#define ILI9341_BLUE        0x001FU

void ILI9341_Init(void);
void ILI9341_WriteCommand(uint8_t command);
void ILI9341_WriteData(uint8_t data);
void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ILI9341_FillScreen(uint16_t color);

#endif /* APP_INC_ILI9341_H_ */
