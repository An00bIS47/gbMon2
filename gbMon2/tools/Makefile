
all: clean spi_test spidev_test

spi_test : spi_test.c
	gcc spi_test.c -o spi_test -lwiringPi

spidev_test-test : spidev_test.c
	gcc spidev_test.c -o spidev_test-test

clean:
	rm -f spidev_test
	rm -f spi_test
	rm -f *.o