targets = 3_2_1_cas.s 3_2_1_cas_test 3_2_1_embedded_cas_test
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

%.s: %.c
	$(CC) -O3 -S $<

.PHONY: clean
clean:
	rm -f $(targets)
