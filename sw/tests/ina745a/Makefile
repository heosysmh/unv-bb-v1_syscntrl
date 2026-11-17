# Host tests for the INA745A driver (mock device)
#   make -C tests/ina745a run

ROOT   := ../..
CC     ?= gcc
CFLAGS := -std=gnu11 -Wall -Wextra -g -O0 \
          -I$(ROOT)/mcu/software/standalone/driver -I$(ROOT)/devices/ina745a \
          -I$(ROOT)/app/src

test_ina745a: test_ina745a.c $(ROOT)/devices/ina745a/ina745a.c $(ROOT)/devices/ina745a/ina745a.h
	$(CC) $(CFLAGS) test_ina745a.c -o $@

.PHONY: run clean
run: test_ina745a
	./test_ina745a

clean:
	rm -f test_ina745a
