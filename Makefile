# Makefile
# OGL_Project_2

# Compiler Vars
CC := clang -std=c17
CFLAGS := -Wall -Wno-deprecated -Werror
CXX := clang++ -std=c++17
CXXFLAGS := -Wall -Wno-deprecated -Werror 

ifeq ($(DEBUG_BUILD),1)
	CXXFLAGS += -g
else
	CXXFLAGS += -O1
endif

# Directory Vars
BIN_DIR := ./bin
BUILD_DIR := ./build
SOURCE_DIR := ./src
SOURCE_HEADER_DIR := $(SOURCE_DIR)/headers
TARGET_EXEC := $(BIN_DIR)/myglapp

# Extra Flags
INCLUDES := -Iinclude -I$(SOURCE_HEADER_DIR)
LIBRARIES := -Llib -lglad -lglfw.3.3
FWFLAGS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
LDFLAGS := $(LIBRARIES) $(FWFLAGS)

# Targets
SRCS := $(shell find $(SOURCE_DIR) -name '*.cpp')
OBJS := $(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Directives
vpath %.{cpp,hpp} $(SOURCE_DIR)
.PHONY: all clean object_files sloc

sloc:
	@wc -l $(SRCS)

object_files: $(OBJS)

all: object_files $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< $(INCLUDES) -o $@

clean:
	rm -f $(TARGET_EXEC)
