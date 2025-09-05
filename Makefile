
NAME = ircserv
CXX = c++
CXXFLAGS = -std=c++17
CXXFLAGS += -Werror -Wextra -Wall
CXXFLAGS += -MMD -MP
CXXFLAGS += -I $(INC_DIR)
#CXXFLAGS += -g -fsanitize=address

OBJ_DIR := obj
INC_DIR := inc

VPATH := src src/server
CFILES_SRCS := main.cpp Client.cpp Channel.cpp utils.cpp
CFILES_SERV := Server.cpp server_start.cpp msg_handler.cpp server_function.cpp join.cpp privmsg.cpp CommandServer.cpp

SRCS := $(CFILES_SRCS) $(CFILES_SERV) 

OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))
TOTAL := $(words $(SRCS))
COUNT := 0

GREEN := \033[32m
RED := \033[31m
RESET := \033[0m
CLEAR_LINE := \033[A\033[2K

all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(CLEAR_LINE)Create $(NAME)..."; \
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@printf "$(GREEN) created$(RESET)\n"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@if [ $(COUNT) -eq 0 ]; then \
		printf "\n"; \
	fi
	@$(eval COUNT := $(shell expr $(COUNT) + 1))
	@printf "$(CLEAR_LINE)Compile ($(COUNT)/$(TOTAL)) $<\n";
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@if [ -d $(OBJ_DIR) ]; then \
		printf "Clean $(NAME)..."; \
		rm -rf $(OBJ_DIR); \
		printf "$(RED) cleaned.$(RESET)\n"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		printf "Remove $(NAME)..."; \
		rm -f $(NAME); \
		printf "$(RED) removed.$(RESET)\n"; \
	fi

-include $(OBJS:.o=.d)

re: fclean all

.PHONY: all clean fclean re
