NAME = webserv

SRC = $(addprefix src/,  main.cpp) \
      $(addprefix src/parsing/, Parse.cpp server.cpp) \
	  $(addprefix src/The_server/, Server_.cpp servers.cpp cgi.cpp) \
	  $(addprefix src/Response/, response.cpp Message.cpp readDir.cpp parseRequest.cpp SelfServerBody.cpp removeSession.cpp)

INCLUDE_DIR = inc/ # zido hna b / f lakhr

INCLUDES = inc/*.h inc/*.hpp #will be updated to a wildcard after

# dakxi lakhor ma t9isohx

OBJ_DIR = obj

OBJ = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SRC:.cpp=.o))

CC = c++ 

SRC_DIR = src

CPPFLAGS = -Wall -Wextra -Werror -std=c++98  -fsanitize=address -g -I$(INCLUDE_DIR)

all : $(OBJ_DIR) $(NAME)

$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $(OBJ))
	@mkdir -p properDataBase
	@mkdir -p session

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(INCLUDES)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME) : $(OBJ) 
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean : 
	rm -f $(OBJ)
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)
	@rm -rf session
	@rm -rf properDataBase 

re : fclean all

.PHONY : all clean fclean re
