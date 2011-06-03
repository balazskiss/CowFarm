CC=g++
CFLAGS=-c -Wall
OBJS = main.o CowFarmServer.o Farm.o Cow.o

all: cowfarm-server


$(OBJS):
	$(CC) $(CFLAGS) *.cpp

cowfarm-server: $(OBJS)
	$(CC) $(OBJS) -o cowfarm-server
	rm *.o
