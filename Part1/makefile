CC = gcc
CFLAGS = -Wall -pthread
TARGET = U1 Q1

all: $(TARGET)

$(TARGET): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(TARGET)