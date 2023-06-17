# CPPFLAGS = -std=c++11 -fsanitize=address,undefined
CPPFLAGS = -std=c++11
SRCS = main.cpp board.cpp boardtree.cpp hashtable.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all: tic-tac-toe.exe

tic-tac-toe.exe: $(OBJS)
	g++ -o $@ $(CPPFLAGS) $^

%.o: %.cpp
	g++ $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	del -f *.o *.d

# rm -f *.o pa3.exe *.d temp.txt
# del *.o pa3.exe *.d temp.txt

.PRECIOUS: $(OBJS)