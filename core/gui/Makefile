#date 2016.03.30
#author haze
CC = gcc
TARGET = xface
Objs = main.o haze_framework.o haze_nelk.o
GTKLIB = `pkg-config --libs --cflags gtk+-2.0`

all:	$(TARGET)

$(TARGET):	$(Objs)
	$(CC) $(Objs) $(GTKLIB) -o $(TARGET)
main.o:	main.c
	$(CC) -c main.c $(GTKLIB) -o main.o
haze_framework.o:	haze_framework.c
	$(CC) -c haze_framework.c $(GTKLIB) -o haze_framework.o
haze_nelk.o:	haze_nelk.c
	$(CC) -c haze_nelk.c $(GTKLIB) -o haze_nelk.o

clean:
	rm -rf *.o $(TARGET)
