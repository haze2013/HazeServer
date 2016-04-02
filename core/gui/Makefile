#date 2016.03.30
#author Haze.C
CC = gcc
TARGET = xface
Objs =  main.o haze_framework.o haze_sock.o haze_signal.o haze_event.o haze_ui.o  
GTKLIB = `pkg-config gtk+-2.0 --cflags --libs gthread-2.0`

all:	$(TARGET)
$(TARGET):	$(Objs)
	gcc $(Objs) $(GTKLIB) -o $(TARGET)
haze_ui.o:	haze_ui.c
	gcc -c haze_ui.c $(GTKLIB) -o haze_ui.o
haze_signal.o:	haze_signal.c
	gcc -c haze_signal.c $(GTKLIB) -o haze_signal.o
haze_event.o:	haze_event.c
	gcc -c haze_event.c $(GTKLIB) -o haze_event.o
haze_sock.o:	haze_sock.c
	gcc -c haze_sock.c $(GTKLIB) -o haze_sock.o
main.o:	main.c
	gcc -c main.c $(GTKLIB) -o main.o
haze_framework.o:	haze_framework.c
	gcc -c haze_framework.c $(GTKLIB) -o haze_framework.o

clean:
	rm -rf *.o $(TARGET)


