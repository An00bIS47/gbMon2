
all: clean client-publish-service client-browse-service gbmon-test

client-publish-service: client-publish-service.c
	gcc -Wall -o client-publish-service client-publish-service.c -lavahi-client -lavahi-common

client-browse-service: client-browse-service.c
	gcc -Wall -o client-browse-service client-browse-service.c -lavahi-client -lavahi-common
	
gbmon-test: gbmon-test.c
	gcc -Wall -o gbmon-test gbmon-test.c -lavahi-client -lavahi-common

clean:
	rm -f client-publish-service
	rm -f client-browse-service
	rm -f *.o