# makes use of implicit rules
# -> https://codereview.stackexchange.com/questions/78855/makefile-for-c-opengl-with-glfw-and-glad
# -> https://www.gnu.org/software/make/manual/html_node/Implicit-Rules.html

# the compiler
CC=gcc
CXX=g++
# alt. compiler
#CPP=x86_64-w64-mingw32-g++

# include and lib paths | NOTE: string together unique paths for more flexibility
INCL_PATH=-ID:/dev/dev_lib/include
LIB_PATH=-LD:/dev/dev_lib/lib/x64

# NOTE: I moved both the include and lib (libglu32.a & libopengl32.a) files from
# D:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw (/include/GL and /lib respectively)
# -> this means that this is only set up to compile for x64/cygwin64


# the flags to pass to the compiler/linker | NOTE: using standard names to leverage implicit rules
CFLAGS=-w -Wall -g -ggdb3 $(INCL_PATH)
CXXFLAGS=-std=c++14 -w -Wall -g -ggdb3 $(INCL_PATH)
LDFLAGS=$(LIB_PATH) -Wl,-subsystem,windows -lSDL2main -lSDL2 -lglu32 -lopengl32

BUILD_DIR=../build

TARGET=main.exe
C_FILES=$(wildcard *.c)
CPP_FILES=$(wildcard *.cpp)
OBJS=$(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(STATIC_LIBS) $(LDFLAGS)

.PHONY: clean
clean:
	-rm $(TARGET) $(OBJS)