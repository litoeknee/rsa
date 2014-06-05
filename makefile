DIRS = .
SRCS = $(foreach dir, $(DIRS), $(shell find $(dir) -name "*.cpp"))
HDRS = $(foreach dir, $(DIRS), $(shell find $(dir) -name "*.h"))

main: $(SRCS) $(HDRS)
	g++-mp-4.8 $(SRCS) -o main -std=c++11 -I../
