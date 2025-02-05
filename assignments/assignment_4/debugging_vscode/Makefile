SOURCES := character_count.c

BUILD_DIR := build
LINKER_SCRIPT := $(BUILD_DIR)/linker_script_nrf52.ld

CC	:= gcc

QUIET := @

# Set GNU dialect to ISO C99
CFLAGS += -std=gnu99

.PHONY: default debug clean

default:
	@echo Compiling project
	$(CC) $(CFLAGS) $(SOURCES) -o $(BUILD_DIR)/main.o

	
debug:
	@echo Compiling project with debug
	$(CC) $(CFLAGS) -ggdb -Og -g3 $(SOURCES) -o $(BUILD_DIR)/main.o

clean:
	$(QUIET)rm $(BUILD_DIR)/main.o
