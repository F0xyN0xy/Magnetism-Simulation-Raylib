# Magnetic Field Simulator - Raylib C++ Build
# Supports Linux, macOS, and Windows (via MinGW)

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET   = magnetism

SRCS = main.cpp magnet.cpp physics.cpp field.cpp particles.cpp compass.cpp renderer.cpp ui.cpp
OBJS = $(SRCS:.cpp=.o)

# Raylib paths (adjust these to your actual install location)
RAYLIB_DIR = C:/raylib
RAYLIB_INCLUDE = $(RAYLIB_DIR)/raylib/src
RAYLIB_LIB     = $(RAYLIB_DIR)/lib

# Platform detection
ifeq ($(OS),Windows_NT)
    PLATFORM = WINDOWS
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM = LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        PLATFORM = MACOS
    endif
endif

# Platform-specific flags
ifeq ($(PLATFORM),WINDOWS)
    TARGET := $(TARGET).exe
    CXXFLAGS += -I$(RAYLIB_INCLUDE)
    # Static linking - no DLLs needed!
    LDFLAGS = -L$(RAYLIB_LIB) -lraylib -lopengl32 -lgdi32 -lwinmm -static-libgcc -static-libstdc++
    RM = del /Q
    RMDIR = rmdir /S /Q
else ifeq ($(PLATFORM),LINUX)
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    RM = rm -f
else ifeq ($(PLATFORM),MACOS)
    LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    RM = rm -f
endif

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
ifeq ($(PLATFORM),WINDOWS)
	-$(RM) $(subst /,\,$(OBJS)) $(subst /,\,$(TARGET))
else
	$(RM) $(OBJS) $(TARGET)
endif

run: $(TARGET)
ifeq ($(PLATFORM),WINDOWS)
	$(TARGET)
else
	./$(TARGET)
endif