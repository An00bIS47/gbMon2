
all: clean client-publish-service client-browse-services gbmon-test


client-browse-services: client-browse-services.c
	gcc -Wall -o client-browse-services client-browse-services.c -lavahi-client -lavahi-common

client-publish-service: client-publish-service.c
	gcc -Wall -o client-publish-service client-publish-service.c -lavahi-client -lavahi-common
	
gbmon-test: gbmon-test.c
	gcc -Wall -o gbmon-test gbmon-test.c -lavahi-client -lavahi-common

clean:
	rm -f client-publish-service
	rm -f client-browse-services
	rm -f *.o