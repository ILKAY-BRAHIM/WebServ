NAME = server

SRC = $(addprefix src/, Parse.cpp main.cpp server.cpp printServers.cpp request.cpp fillServer.cpp )

INCLUDE_DIR = inc/

OBJ_DIR = obj

OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))

CC = c++ 

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g -I$(INCLUDE_DIR)

all : $(OBJ_DIR) $(NAME)

$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o : src/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME) : $(OBJ) $(INCLUDE_DIR)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean : 
	rm -f $(OBJ)
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re
