
#ifndef _HW_SPI_H
#define _HW_SPI_H
#include <stdint.h>


void spi_init();    //初始化
void SPI_WR_REG(uint8_t Reg);
void SPI_WR_DATA(uint8_t Data);
void SPI_WriteReg(uint8_t LCD_Reg, uint8_t LCD_RegValue);


void SPI_WRITE_BUF_DATA(uint8_t *buf, int len);


#endif

