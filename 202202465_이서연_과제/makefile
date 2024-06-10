CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = ledger_system_info
SRCS = main.c ledger.c system_info.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
