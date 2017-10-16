# Makefile
# face_detection: face_detection.cpp
#     gcc -I /usr/local/include -o face_detection face_detection.cpp

CC		= g++
CFLAGS		= -std=c++14 -O4 -Wall -I/usr/local/include
CPPFLAGS		= -std=c++14 -O4 -Wall -I/usr/local/include
DEST		= /usr/local/bin
LDFLAGS		= -L/usr/local/lib -lwiringPi
LIBS		= -lm `pkg-config --libs opencv`
OBJS		= main.o
PROGRAM		= robot

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

clean:		rm -f *.o *~ $(PROGRAM)

