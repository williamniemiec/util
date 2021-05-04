OBJS	= 	Scheduler.o
SOURCE	=	src/wniemiec/util/task/Scheduler.cpp
CC	 = g++ 
FLAGS	 = -c
LFLAGS	 =

all: $(OBJS)
	$(CC) -c $(OBJS) $(LFLAGS) -std=c++17

Scheduler.o: src/wniemiec/util/task/Scheduler.cpp
	$(CC) $(FLAGS) src/wniemiec/util/task/Scheduler.cpp -std=c++17

clean:
	rm -f $(OBJS) $(OUT)

