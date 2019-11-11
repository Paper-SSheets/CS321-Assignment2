CC = gcc
CFLAGS = -Werror
BIN = assignment2
SRCS = assignment2.c
OBJS = $(SRCS:.c=.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o vgcore.* assignment2
