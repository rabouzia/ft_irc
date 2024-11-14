NAME       =	ircserv

SRC        =	src/main.cpp 		\

CC 		   =	c++

CFLAGS 	   =	-Wall -Wextra -Werror -std=c++98 -I./inc 

OFLAG      = 	-MMD -g3

OBJ_DIR	   =	obj/

SRCS       =    $(SRC)

OBJ 	   =    $(patsubst src/%.cpp, $(OBJ_DIR)%.o, $(SRCS))

DEP 	   =    $(patsubst src/%.cpp, $(OBJ_DIR)%.d, $(SRCS))

MAKE_DIR   =    mkdir -p

SMAKE	   =    make --no-print-directory

all:	        $(NAME)

$(NAME):        $(OBJ) $(DEP)
				@$(CC) $(CFLAGS) $(OFLAG) $(OBJ) -o $@
				
$(OBJ_DIR)%.o:  src/%.cpp
			    @mkdir -p $(OBJ_DIR)
			    @$(CC) $(CFLAGS) $(OFLAG) -c $< -o $@ 

clean:
				@rm -rf $(OBJ_DIR) $(DEP)
				@echo "\033[1;31m======== irc object files removed ========\033[0m"

fclean:         clean
				@$(RM) $(NAME)
				@echo "\033[1;31m======= irc executable removed =======\033[0m"

re:             fclean all

.PHONY: clean fclean all re
