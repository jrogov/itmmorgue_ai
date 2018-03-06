CC = gcc
CFLAGS = --std=gnu99 -pedantic -Wall -Wextra -Werror -ggdb3 
DEBUGFLAGS = -DDEBUG
LIBS = -lncursesw
INCS = -I /usr/include/ncursesw

HDRS = $(wildcard *.h)
SRCS = main.c squaere.c entity.c movement.c common.c chunk.c guard.c
OBJS = $(SRCS:.c=.o)

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(CFLAGS) $(DEBUGFLAGS) $(LIBS) $(INCS) -o $@

%.o: %.c $(HDRS)
	$(CC) -c $< $(CFLAGS) $(DEBUGFLAGS) $(LIBS) $(INCS) -o $@

clean:
	$(RM) $(OBJS) $(TARGET)

todo:
	@echo
	@grep -i TODO $(SRCS) $(HDRS)
	@echo
