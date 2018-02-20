CC = gcc
CFLAGS = --std=gnu99 -pedantic -Wall -Wextra -Werror
LIBS = -lncursesw
INCS = -I /usr/include/ncursesw

SRCS = squaere.c entity.c movement.c
OBJS = $(SRCS:.c=.o)

TARGET = squaere

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(CFLAGS) $(LIBS) $(INCS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) $(LIBS) $(INCS) -o $@

clean:
	$(RM) $(OBJS) $(TARGET)