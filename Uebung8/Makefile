CFLAGS += -Wall 

all: clientSock serverSock

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) 

clean:
	rm -f clientSock
	rm -f serverSock
