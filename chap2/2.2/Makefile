targets = 2_2_1_1_pthread 2_2_1_2_pthread 2_2_2_wo_volatile.s 2_2_2_volatile.s
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

%.s: %.c
	$(CC) -O3 -S $<

.PHONY: clean
clean:
	rm -f $(targets)
