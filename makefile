SRCS1:=$(wildcard src/DictBuilder/*.cpp)
SRCS2:=$(wildcard src/WebProcessor/*.cpp)
SRCS3:=$(wildcard src/server/*.cpp)
SRCS4:=$(wildcard src/client/*.cpp)

OBJS1:=$(patsubst src/DictBuilder/%.cpp, objs/DictBuilder/%.o, $(SRCS1))
OBJS2:=$(patsubst src/WebProcessor/%.cpp, objs/WebProcessor/%.o, $(SRCS2))
OBJS3:=$(patsubst src/server/%.cpp, objs/server/%.o, $(SRCS3))
OBJS4:=$(patsubst src/client/%.cpp, objs/client/%.o, $(SRCS4))

OUT1:= bin/DictBuilder
OUT2:= bin/WebProcessor
OUT3:= bin/server
OUT4:= bin/client

OUT:= $(OUT1) $(OUT2) $(OUT3) $(OUT4)
SRCS:= $(SRCS1) $(SRCS2) $(SRCS3) $(SRCS4)
OBJS:= $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) 
LINKS:= -lpthread -llog4cpp -lredis++ -lhiredis -pthread -std=c++11
CC:=g++

OUTS: $(OUT)

Dict: $(OUT1)
Web: $(OUT2)
ser: $(OUT3)
cli: $(OUT4)

$(OUT1): $(OBJS1)
	$(CC) $^ -o $@ $(LINKS)

$(OUT2): $(OBJS2)
	$(CC) $^ -o $@ $(LINKS)

$(OUT3): $(OBJS3)
	$(CC) $^ -o $@ $(LINKS)

$(OUT4): $(OBJS4)
	$(CC) $^ -o $@ $(LINKS)

objs/DictBuilder/%.o : src/DictBuilder/%.cpp
	$(CC) -c $^ -o $@

objs/WebProcessor/%.o : src/WebProcessor/%.cpp
	$(CC) -c $^ -o $@

objs/server/%.o : src/server/%.cpp
	$(CC) -c $^ -o $@

objs/client/%.o : src/client/%.cpp
	$(CC) -c $^ -o $@

.PHONY: clean rebuild print 

clean: 
	rm -f $(OBJS) $(OUT)
	
rebuild: clean OUTS

print:
	echo $(OBJS)
