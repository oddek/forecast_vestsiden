
PROG = forecastCrawler 

CC = g++
CXXFLAGS = -Wall -Wextra -std=c++17
CPPFLAGS = #-I /usr/include -L /usr/lib
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
LDFLAGS = -lcurl -lmysqlcppconn

all: build

run:
	./$(PROG) 

build: $(PROG)

$(PROG): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $(OBJ) $(CPPFLAGS) $(LDFLAGS)

clean:
	rm -f *stackdump $(PROG) $(OBJ)
