# Host tests for the Si5332 driver (mock device)
#   make -C tests/si5332 run

ROOT   := ../..
CC     ?= gcc
CFLAGS := -std=gnu11 -Wall -Wextra -g -O0 \
          -Istub -I$(ROOT)/mcu/software/standalone/driver -I$(ROOT)/devices/si5332

test_si5332: test_si5332.c $(ROOT)/devices/si5332/si5332.c $(ROOT)/devices/si5332/si5332_cfg.c \
             $(ROOT)/devices/si5332/si5332.h $(ROOT)/devices/si5332/si5332_cfg.h stub/bsp.h
	$(CC) $(CFLAGS) test_si5332.c $(ROOT)/devices/si5332/si5332_cfg.c -o $@

.PHONY: run clean
run: test_si5332
	./test_si5332

clean:
	rm -f test_si5332
