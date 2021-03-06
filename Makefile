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
LDFLAGS=$(LIB_PATH) -Wl,-subsystem,windows -lSDL2main -lSDL2 -lSDL2_image -lglu32 -lopengl32

TARGET=main.exe
BUILD_DIR=build
OBJS=$(BUILD_DIR)/Physics.o $(BUILD_DIR)/OpenGLGraphics.o \
	  $(BUILD_DIR)/GameObjs.o $(BUILD_DIR)/Command.o \
	  $(BUILD_DIR)/InputComponent.o $(BUILD_DIR)/PhysicsComponent.o $(BUILD_DIR)/GraphicsComponent.o \
	  $(BUILD_DIR)/SceneManager.o \
     $(BUILD_DIR)/Wasteland.o
ADD_OBJS=GL/gl3w.o

all: $(TARGET)

$(TARGET): $(OBJS) $(ADD_OBJS)
	$(CXX) -g -ggdb3 -o $@ $^ $(STATIC_LIBS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	-rm $(TARGET) $(OBJS)