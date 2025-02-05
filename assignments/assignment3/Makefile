.DEFAULT_GOAL := taco

CC := gcc
CFLAGS := -O0 -g3

BUILD_DIR := build
SRC_DIR := source

SOURCES := grønnsaker.c protein.c taco_krydder.c taco_lefse.c taco_saus.c drikke.c main.c
INGRIDIENSER := $(SOURCES:%.c=$(BUILD_DIR)/%.o)
SRC := $(SOURCES:%.c=$(SRC_DIR)/%.c)


.PHONY : clean
clean:
	rm -rf $(.DEFAULT_GOAL) $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

taco: $(INGRIDIENSER)
	$(CC) $(CFLAGS) -o $@ $(INGRIDIENSER)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
