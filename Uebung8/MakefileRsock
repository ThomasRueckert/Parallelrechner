CFLAGS += -Wall -lrdmacm

all: client server

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) 

clean:
	rm -f client
	rm -f server
