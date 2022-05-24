CC=gcc
OBJS = main.o learn.o
CFLAGS = -Wall -O2 -std=c99 -lm
TARGET=siberia
$(TARGET):	$(OBJS)
	$(CC)  $(CFLAGS) -o  $@ $(OBJS)
main.o:	main.c
	$(CC) $(CFLAGS) -c   -o main.o main.c
learn.o:	learn.c
	$(CC) $(CFLAGS) -c   -o learn.o learn.c


clean:
	rm $(OBJS) $(TARGET)
