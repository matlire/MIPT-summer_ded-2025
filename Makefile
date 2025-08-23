# Directories and output file
SRC_DIR   := quadratic
BUILD_DIR := $(SRCDIR)/build
DIST_DIR  := $(SRCDIR)/dist
BIN_NAME  := solver.out
BIN       := $(BUILD_DIR)/$(BIN_NAME)

# GCC setup
CC     := gcc
CFLAGS := -Wall -g                  \
		  -I $(SRCDIR)/utils/colors \
		  -I $(SRCDIR)/quadratic    \
		  -I $(SRCDIR)/test_core    \
		  -I $(SRCDIR)/test_core/tests
LDFLAGS := -lm

# C source files
SRC_FILES  := $(shell find $(SRCDIR) -type f -name '*.c' -not -path '$(SRCDIR)/build/*' -print; [ -f main.c ] && echo main.c)

# Object files
OBJ_FILES  := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

# Dependencies files
DEPS_FILES := $(OBJS:.o=.d)

.PHONY: all clean run files
all: $(BIN)

files:
	@echo "C source dir = $(SRC_DIR)"
	@echo "SRC_FILES    = $(SRC_FILES)"
	@echo "OBJ_FILES    = $(OBJ_FILES)"

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	
$(BIN): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $@
	@cp $(BIN) $(DIST_DIR)/$(BIN_NAME)

run: all
	$(BIN)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
