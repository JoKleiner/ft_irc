SRC_DIR := src
OBJ_DIR := obj
CC := c++
CFLAGS := -Wall -Wextra -Werror -Wshadow -std=c++17
IFLAGS := -Iinclude

# Executable setup
NAME := bot
SRC_FILES := \
	$(SRC_DIR)/main.cpp

OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Rules
all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# Other rules
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
