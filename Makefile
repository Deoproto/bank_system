CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -pedantic -g
TARGET  = bank_system
SRCS    = main.c bank.c account.c savings.c checking.c transaction.c user.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
