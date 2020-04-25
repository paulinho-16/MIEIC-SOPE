CC = gcc
CFLAGS = -Wall -pthread
DEPS = utils.h
OBJS1 = U1.c utilsU1.c
OBJS2 = Q1.c utilsQ1.c
TARGET1 = U1
TARGET2 = Q1

all: U1 Q1

$(TARGET1): $(OBJS1)	
	$(CC) $(CFLAGS) $(OBJS1) -o $@

$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) $(OBJS2) -o $@

clean:
	$(RM) $(TARGET1)
	$(RM) $(TARGET2)