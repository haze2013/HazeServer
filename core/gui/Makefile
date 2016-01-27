#date 2016.03.30
#author Haze.C
CC = gcc
TARGET = xface
Objs =  main.o haze_framework.o haze_nelk.o haze_ui.o haze_signal.o
GTKLIB = `pkg-config --libs --cflags gtk+-2.0`

all:	$(TARGET)
$(TARGET):	$(Objs)
	gcc $(Objs) $(GTKLIB) -o $(TARGET)
main.o:	main.o
	gcc -c main.c $(GTKLIB) -o main.o
haze_framework.o:	haze_framework.o
	gcc -c haze_framework.c $(GTKLIB) -o haze_framework.o
haze_nelk.o:	haze_nelk.c
	gcc -c haze_nelk.c -o haze_nelk.o
haze_ui.o:	haze_ui.o
	gcc -c haze_ui.c $(GTKLIB) -o haze_ui.o
haze_signal.o:	haze_signal.o
	gcc -c haze_signal.c $(GTKLIB) -o haze_signal.o

clean:
	rm -rf *.o $(TARGET)


