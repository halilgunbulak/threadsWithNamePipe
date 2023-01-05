CC=gcc
CFLAGS=-pthread

all: file_manager file_client

thread_example: file_manager.o
	$(CC) $(CFLAGS) -o $@ $^

normal_example: file_client.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o file_manager file_client