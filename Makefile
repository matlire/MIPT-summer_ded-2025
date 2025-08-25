# Directories and output file
SRC_DIR   := quadratic
BUILD_DIR := $(SRC_DIR)/build
DIST_DIR  := $(SRC_DIR)/dist
BIN_NAME  := solver.out
BIN       := $(BUILD_DIR)/$(BIN_NAME)

# GCC setup
CC     := gcc
CFLAGS := -Wall -g -D_GNU_SOURCE               \
		  -I $(SRC_DIR)/utils/colors           \
		  -I $(SRC_DIR)/quadratic              \
		  -I $(SRC_DIR)/smart_parse            \
		  -I $(SRC_DIR)/test_core              \
		  -I $(SRC_DIR)/test_core/tests        \
		  -I $(SRC_DIR)/utils/io               \
		  -I $(SRC_DIR)/utils/parameter_parser \

LDFLAGS := -lm

# C source files
SRC_FILES  := $(shell find $(SRC_DIR) -type f -name '*.c' -not -path '$(SRC_DIR)/build/*' -print; [ -f main.c ] && echo main.c)

# Object files
OBJ_FILES  := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Dependencies files
DEPS_FILES := $(OBJ_FILES:.o=.d)

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
	@mkdir -p $(DIST_DIR)
	@cp $(BIN) $(DIST_DIR)/$(BIN_NAME)

run: all
	$(BIN)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DIST_DIR)

-include $(DEPS)
