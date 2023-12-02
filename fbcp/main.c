
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <syslog.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <linux/fb.h>

#include "common.h"

#ifdef DEF_ST7796
#include "st7796.h"
#endif

uint16_t swap_uint16(uint16_t *num)
{
    uint16_t value;
    struct byte2
    {
        uint8_t b1;
        uint8_t b2;
    } *pbyte2;
    pbyte2 = (struct byte2 *)num;
    value = pbyte2->b2 + (pbyte2->b1 << 8);
    return value;
}

int main()
{

    int fd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    int bpp;
    int width, height;

    LCD_Init();
    fd = open("/dev/fb0", O_RDWR);
    if (fd < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        exit(1);
    }

    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo))
    {
        printf("Error reading fixed information/n");
        exit(2);
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf("Error reading variable information/n");
        exit(3);
    }
    printf("The mem is :%d\n", finfo.smem_len);
    printf("The line_length is :%d\n", finfo.line_length);
    printf("The xres is :%d\n", vinfo.xres);
    printf("The yres is :%d\n", vinfo.yres);
    printf("bits_per_pixel is :%d\n", vinfo.bits_per_pixel);

    bpp = vinfo.bits_per_pixel;
    width = vinfo.xres;
    height = vinfo.yres;

    printf("像素格式: R<%d %d> G<%d %d> B<%d %d>\n",
           vinfo.red.offset, vinfo.red.length,
           vinfo.green.offset, vinfo.green.length,
           vinfo.blue.offset, vinfo.blue.length);

    uint16_t *buf = malloc(width * height * 2);

    switch (bpp)
    {
    case 32:
        uint32_t tmp32;
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint32_t *fb0_u32;
        fb0_u32 = (uint32_t *)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        while (1)
        {
            for (int i = 0; i < width * height; i++)
            {
                tmp32 = fb0_u32[i];
                b = (tmp32 & 0xff);
                g = (tmp32 & 0xff00) >> 8;
                r = (tmp32 & 0xff0000) >> 16;

                r /= 0xff / 0x1f;
                g /= 0xff / 0x3f;
                b /= 0xff / 0x1f;

                buf[i] = (r << 3) + (b << 8) + ((g & 0x7) << 13) + ((g & 0x38) >> 3);
            }
            LCD_draw_buff((uint8_t *)buf, width * height * 2);
        }
    case 16:
        uint16_t *fb0_u16;
        fb0_u16 = (uint16_t *)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        while (1)
        {
            for (int i = 0; i < width * height; i++)
            {
                // buf[i] = fb0_u16 + i;
                buf[i] = swap_uint16(fb0_u16 + i);
            }
            LCD_draw_buff((uint8_t *)buf, width * height * 2);
        }

    default:
        printf("\n\tFailed no support you hdmi format\r\n");
    }
    return 0;
}