
NAME = ircserv
CXX = c++
CXXFLAGS = -std=c++17
CXXFLAGS += -MMD -MP
#-Werror -Wextra -Wall 

OBJ_DIR := obj

VPATH := src
SRCS := main.cpp Client.cpp

OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))
TOTAL := $(words $(SRCS))
COUNT := 0

all: $(NAME)
	

$(NAME): $(OBJS)
	@printf "\033[1A\033[2KCreate $(NAME)... "; \
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@printf "\033[0;32mcreated\033[0m\n"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@$(eval COUNT := $(shell echo $$(($(COUNT)+1))))
	@if [ $(COUNT) -eq 1 ]; then \
		printf "\n"; \
	fi
	@printf "\033[1A\033[2KCompilen ($(COUNT)/$(TOTAL)) $<\n";
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@if [ -d $(OBJ_DIR) ]; then \
		printf "Clean $(NAME)... "; \
		rm -rf $(OBJ_DIR); \
		printf "\033[0;31mcleaned.\033[0m\n"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		printf "Remove $(NAME)... "; \
		rm -f $(NAME); \
		printf "\033[0;31mremoved.\033[0m\n"; \
	fi
	@rm -f *_shrubbery; \

re: fclean all

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re
