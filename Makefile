CC = gcc
CFLAGS = -g -O0 -Wall -Wextra -Iinclude -fsanitize=address -MMD -MP
LDFLAGS = -fsanitize=address -lm
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
DEP = $(OBJ:.o=.d)
TARGET = luuke

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(TARGET)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEP)

clean:
	rm -rf build $(TARGET)
