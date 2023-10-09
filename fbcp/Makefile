CC = gcc
CFLAGS =  

OBJS = hw_spi.o  wiringPi.o  st7796.o common.h

all: st7796

st7796:  ${OBJS}
	$(CC) $(CFLAGS)  -DDEF_ST7796  main.c ${OBJS} -o $@



%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	-rm -f *.o 
	-rm -f st7796
