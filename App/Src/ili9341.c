/*
 * ili9341.c
 *
 * Minimal SPI command/data layer for the Adafruit 2.8" TFT Touch Shield v2
 * display controller. The shield's ILI9341 receives commands and RGB565 pixel
 * data over SPI1. PA4 is used as manual TFT chip select and PB0 is used as the
 * data/command signal.
 */

#include "ili9341.h"

#include "app_delay.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"

#define ILI9341_SWRESET     0x01U
#define ILI9341_SLPOUT      0x11U
#define ILI9341_DISPOFF     0x28U
#define ILI9341_DISPON      0x29U
#define ILI9341_CASET       0x2AU
#define ILI9341_PASET       0x2BU
#define ILI9341_RAMWR       0x2CU
#define ILI9341_MADCTL      0x36U
#define ILI9341_PIXFMT      0x3AU
#define ILI9341_FRMCTR1     0xB1U
#define ILI9341_DFUNCTR     0xB6U
#define ILI9341_PWCTR1      0xC0U
#define ILI9341_PWCTR2      0xC1U
#define ILI9341_VMCTR1      0xC5U
#define ILI9341_VMCTR2      0xC7U

#define ILI9341_MADCTL_MX   0x40U
#define ILI9341_MADCTL_BGR  0x08U
#define ILI9341_PIXFMT_16BPP 0x55U

#define TFT_CS_PORT         GPIOA
#define TFT_CS_PIN          GPIO_PIN_NO_4
#define TFT_DC_PORT         GPIOB
#define TFT_DC_PIN          GPIO_PIN_NO_0

static void TFT_WaitForSPIComplete(void)
{
	/*
	 * After the final write to SPI_DR, TXE only means the data register is
	 * empty. BSY must also clear before CS rises or DC changes, otherwise the
	 * ILI9341 can miss the last command/parameter byte.
	 */
	while(SPI_GetFlagStatus(SPI1, SPI_TXE_FLAG) == FLAG_RESET)
	{
	}

	while(SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG) == FLAG_SET)
	{
	}
}

static void TFT_Select(void)
{
	GPIO_WriteToOutputPin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);
}

static void TFT_Unselect(void)
{
	TFT_WaitForSPIComplete();
	GPIO_WriteToOutputPin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

static void TFT_CommandMode(void)
{
	TFT_WaitForSPIComplete();
	GPIO_WriteToOutputPin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_RESET);
}

static void TFT_DataMode(void)
{
	TFT_WaitForSPIComplete();
	GPIO_WriteToOutputPin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
}

static void TFT_WriteDataBuffer(const uint8_t *data, uint32_t length)
{
	if((data != 0) && (length > 0))
	{
		SPI_SendData(SPI1, (uint8_t *)data, length);
	}
}

void ILI9341_WriteCommand(uint8_t command)
{
	TFT_Select();
	TFT_CommandMode();
	SPI_SendData(SPI1, &command, 1);
	TFT_Unselect();
}

void ILI9341_WriteData(uint8_t data)
{
	TFT_Select();
	TFT_DataMode();
	SPI_SendData(SPI1, &data, 1);
	TFT_Unselect();
}

void ILI9341_Init(void)
{
	/*
	 * RESET is wired to the STM32 NRST pin, so there is no separate display
	 * reset GPIO here. These delays give the panel time to finish power-up.
	 */
	app_delay_ms(100);

	ILI9341_WriteCommand(ILI9341_SWRESET);
	app_delay_ms(150);

	ILI9341_WriteCommand(ILI9341_DISPOFF);

	ILI9341_WriteCommand(ILI9341_PWCTR1);
	ILI9341_WriteData(0x23);

	ILI9341_WriteCommand(ILI9341_PWCTR2);
	ILI9341_WriteData(0x10);

	ILI9341_WriteCommand(ILI9341_VMCTR1);
	ILI9341_WriteData(0x3E);
	ILI9341_WriteData(0x28);

	ILI9341_WriteCommand(ILI9341_VMCTR2);
	ILI9341_WriteData(0x86);

	ILI9341_WriteCommand(ILI9341_MADCTL);
	ILI9341_WriteData(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);

	ILI9341_WriteCommand(ILI9341_PIXFMT);
	ILI9341_WriteData(ILI9341_PIXFMT_16BPP);

	ILI9341_WriteCommand(ILI9341_FRMCTR1);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x18);

	ILI9341_WriteCommand(ILI9341_DFUNCTR);
	ILI9341_WriteData(0x08);
	ILI9341_WriteData(0x82);
	ILI9341_WriteData(0x27);

	ILI9341_WriteCommand(ILI9341_SLPOUT);
	app_delay_ms(120);

	ILI9341_WriteCommand(ILI9341_DISPON);
	app_delay_ms(120);
}

void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint8_t data[4];

	/*
	 * The ILI9341 address window tells the controller where following pixel
	 * data should land: column range first, then page/row range.
	 */
	ILI9341_WriteCommand(ILI9341_CASET);
	data[0] = (uint8_t)(x0 >> 8);
	data[1] = (uint8_t)(x0 & 0xFFU);
	data[2] = (uint8_t)(x1 >> 8);
	data[3] = (uint8_t)(x1 & 0xFFU);
	TFT_Select();
	TFT_DataMode();
	TFT_WriteDataBuffer(data, sizeof(data));
	TFT_Unselect();

	ILI9341_WriteCommand(ILI9341_PASET);
	data[0] = (uint8_t)(y0 >> 8);
	data[1] = (uint8_t)(y0 & 0xFFU);
	data[2] = (uint8_t)(y1 >> 8);
	data[3] = (uint8_t)(y1 & 0xFFU);
	TFT_Select();
	TFT_DataMode();
	TFT_WriteDataBuffer(data, sizeof(data));
	TFT_Unselect();
}

void ILI9341_FillScreen(uint16_t color)
{
	uint8_t pixels[64 * 2];
	uint8_t command;
	uint32_t remaining_pixels = ILI9341_WIDTH * ILI9341_HEIGHT;
	uint32_t i;

	ILI9341_SetAddressWindow(0, 0, ILI9341_WIDTH - 1U, ILI9341_HEIGHT - 1U);

	/*
	 * RGB565 pixels are sent high byte first, then low byte. A small repeated
	 * buffer keeps the code simple without allocating a full framebuffer.
	 */
	for(i = 0; i < sizeof(pixels); i += 2)
	{
		pixels[i] = (uint8_t)(color >> 8);
		pixels[i + 1U] = (uint8_t)(color & 0xFFU);
	}

	TFT_Select();
	TFT_CommandMode();
	command = ILI9341_RAMWR;
	SPI_SendData(SPI1, &command, 1);
	TFT_DataMode();

	while(remaining_pixels >= 64U)
	{
		TFT_WriteDataBuffer(pixels, sizeof(pixels));
		remaining_pixels -= 64U;
	}

	if(remaining_pixels > 0)
	{
		TFT_WriteDataBuffer(pixels, remaining_pixels * 2U);
	}

	TFT_Unselect();
}
