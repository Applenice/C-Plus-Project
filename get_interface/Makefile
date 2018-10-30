CC = gcc
SOURCE = $(wildcard *.c)
CFLAGS = -lpcap
OBJS = get_interface.o

TARGET = get_interface

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

$(OBJS): %.o:%.c
	$(CC) -c -g $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET) core test_interface.txt
