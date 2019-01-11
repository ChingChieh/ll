CC = gcc
CFLAGS = 
LDFLAGS += -pthread

EXEC = main test_insert
all: $(EXEC)

main: ll.c main.c
	$(CC) ll.c main.c -o main $(LDFLAGS)

test_insert: ll.c t.c
	$(CC) ll.c t.c -o test_insert $(LDFLAGS)

clean:
	$(RM) -f $(EXEC)	

.PHONY: all clean
