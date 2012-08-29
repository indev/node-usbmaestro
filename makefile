
CC = g++

VPATH = src

BIN = usbmaestro

IFLAGS = -I/usr/include/libusb-1.0 -I/usr/local/include/libusb-1.0
LFLAGS = -L/usr/lib/ -lusb-1.0
CFLAGS = -Wall -Werror

OBJ = main.o UsbDevice.o

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)  $(IFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS) 

.PHONY: clean

clean:
	rm -f $(OBJ)