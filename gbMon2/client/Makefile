all: clean client client-test

client : client.c
	gcc client.c -o client

client-test : client-test.c
	gcc client-test.c -o client-test

clean:
	rm -f client-test
	rm -f client
	rm -f *.o