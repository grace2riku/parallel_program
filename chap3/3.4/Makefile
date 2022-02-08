targets = 3_4_2_semaphore_posix
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

%.s: %.c
	$(CC) -O3 -S $<

.PHONY: clean
clean:
	rm -f $(targets)
