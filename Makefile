# makes use of implicit rules
# -> https://codereview.stackexchange.com/questions/78855/makefile-for-c-opengl-with-glfw-and-glad
# -> https://www.gnu.org/software/make/manual/html_node/Implicit-Rules.html

# the compiler
CC=gcc
CXX=g++

# include and lib paths | NOTE: string together unique paths for more flexibility
INCLUDES=-ID:/dev/vs_dev_lib/include
LIBRARIES=-LD:/dev/vs_dev_lib/lib/x64

# the flags to pass to the compiler/linker | NOTE: using standard names to leverage implicit rules
CFLAGS=-Wall -ggdb $(INCLUDES)
CXXFLAGS=-std=c++14 -Wall -ggdb $(INCLUDES)
LDFLAGS=$(LIBRARIES) -lopengl32 -lSDL2main -lSDL2 -lSDL2_image

TARGET=main.exe
C_FILES=$(wildcard *.c)
CPP_FILES=$(wildcard *.cpp)
OBJS=$(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	-rm $(TARGET) $(OBJS)