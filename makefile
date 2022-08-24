# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := FT4222.exe

BUILD_DIR := _Build

OUT_DIR := _Out

SRC_DIRS := Src

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(wildcard Src/*.c)
# SRCS += $(wildcard D:/Neil/Project/driver_manager_lib/dml/src/*.c)
# SRCS += $(wildcard D:/Neil/Project/driver_manager_lib/dml/vendor/*/*.c)

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
# OBJS := $(SRCS:Src/%.c=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
# DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := "Inc"
INC_DIRS += "imports\ftd2xx"
INC_DIRS += "imports\LibFT4222\inc"

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
OTP := -O1

CFLAGS += $(INC_FLAGS) -std=c99 -MMD -MP -m32
CFLAGS += $(OTP)
CFLAGS += -Wall -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable
CFLAGS += -fdata-sections -ffunction-sections

CC := gcc

# Link libraries
LIBS := -l:ftd2xx.lib -l:LibFT4222.lib
LIB_DIRS += "imports\ftd2xx\i386"
LIB_DIRS += "imports\LibFT4222\dll\i386"

LDFLAGS += -m32
LDFLAGS += $(addprefix -L,$(LIB_DIRS))
LDFLAGS += $(LIBS)
LDFLAGS += -fdata-sections -ffunction-sections

################################################################

all: $(BUILD_DIR)/$(TARGET_EXEC) post_build

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
vpath %.c $(sort $(dir $(SRCS)))

# Create build folder
$(BUILD_DIR):
	@mkdir $@

# Build step for C source
$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJECTS) $(BUILD_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@


# Creat output folder
$(OUT_DIR):
	@mkdir $@


post_build: $(BUILD_DIR)/$(TARGET_EXEC) $(OUT_DIR)
	@copy $(subst /,\,$<) $(OUT_DIR)
	@copy $(subst /,\,imports/LibFT4222/dll/i386/LibFT4222.dll) $(OUT_DIR)

Execute: $(OUT_DIR)/$(TARGET_EXEC)
	@$<

debug:
	@echo $(sort $(dir $(SRCS)))

################################################################

.PHONY: clean
clean:
	@if exist $(BUILD_DIR) rd /s /q $(BUILD_DIR)
	@if exist $(OUT_DIR) rd /s /q $(OUT_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(BUILD_DIR)/%.o