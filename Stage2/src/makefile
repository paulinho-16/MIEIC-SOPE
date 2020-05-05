CC = gcc
CFLAGS = -Wall -pthread -lrt
DEPS = utils.h
OBJS1 = U2.c utilsU2.c
OBJS2 = Q2.c utilsQ2.c
TARGET1 = U2
TARGET2 = Q2

all: U2 Q2

$(TARGET1): $(OBJS1)	
	$(CC) $(CFLAGS) $(OBJS1) -o $@

$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) $(OBJS2) -o $@

clean:
	$(RM) $(TARGET1)
	$(RM) $(TARGET2)