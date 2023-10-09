#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <wiringPi.h>
#include "common.h"
#include "hw_spi.h"

static uint32_t SPI_MODE = SPI_MODE_0;
static uint8_t BITS_PER_WORD = 8;
static uint32_t SPEED = SPI_RUN_SPEED_HZ;

int fd;
static void LCD_Reset(void)
{
    // printf("start reset\r\n");
    digitalWrite(PIN_RST, HIGH);
    usleep(300000);
    digitalWrite(PIN_RST, LOW);
    usleep(300000);
    digitalWrite(PIN_RST, HIGH);
    usleep(300000);
    // printf("end reset\r\n");
}
void spi_init()
{
    fd = open(DEV_SPI, O_RDWR);
    if (fd < 0)
    {
        printf("err: spidev打不开\r\n");
        exit(1);
    }

    int ret = ioctl(fd, SPI_IOC_WR_MODE32, &SPI_MODE);
    if (ret == -1)
        printf("err: can't set spi mode");
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &BITS_PER_WORD);
    if (ret == -1)
        printf("err: can't set bits per word");
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &SPEED);
    if (ret == -1)
        printf("err: can't set max speed hz");
    wiringPiSetup();
    pinMode(PIN_DC, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    LCD_Reset();
}
// mosi跟miso同时工作，从tx_buf中取出数据发送的同时，也会读取数据存入rx_buf
int spi_transfer(char *tx_buf, char *rx_buf, int len)
{
    struct spi_ioc_transfer transfer;
    transfer.tx_buf = (unsigned long)tx_buf;
    transfer.rx_buf = (unsigned long)rx_buf;
    transfer.len = len;
    transfer.delay_usecs = 1;
    transfer.speed_hz = SPEED;
    transfer.bits_per_word = BITS_PER_WORD;
    transfer.tx_nbits = 1;
    transfer.rx_nbits = 1;

    int res = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
    if (res < 0)
        printf("err: spi_transfer failed");

    return res;
}
uint8_t tx_buf[2];
uint8_t rx_buf[2];
void SPI_WR_REG(uint8_t Reg)
{
    tx_buf[0] = Reg;
    digitalWrite(PIN_DC, LOW);
    spi_transfer(tx_buf, rx_buf, 1);
}

void SPI_WR_DATA(uint8_t Data)
{
    tx_buf[0] = Data;
    digitalWrite(PIN_DC, HIGH);
    spi_transfer(tx_buf, rx_buf, 1);
}

void SPI_WriteReg(uint8_t LCD_Reg, uint8_t LCD_RegValue)
{
    SPI_WR_REG(LCD_Reg);
    SPI_WR_DATA(LCD_RegValue);
}

void SPI_WRITE_BUF_DATA(uint8_t *buf, int len)
{
    int tmp;
    digitalWrite(PIN_DC, HIGH);

    while (len > 1000)
    {
        spi_transfer(buf, buf, 1000);
        len -= 1000;
        buf += 1000;
    }
    spi_transfer(buf, buf, len);

}
