NAME       =	ircserv 

SRC        =	src/main.cpp 		\
				src/Server.cpp		\
				src/Client.cpp		\
				src/split.cpp		\
				src/cmd.cpp		\
				src/Channel.cpp

CC 		   =	c++

CFLAGS 	   =	-Wall -Wextra -Werror -std=c++98 -I./inc 

OFLAG      = 	-MMD -g3

OBJ_DIR	   =	obj/

SRCS       =    $(SRC)

OBJ 	   =    $(patsubst src/%.cpp, $(OBJ_DIR)%.o, $(SRCS))


MAKE_DIR   =    mkdir -p

SMAKE	   =    make --no-print-directory

all:	        $(NAME)

$(NAME):        $(OBJ)
				@$(CC) $(CFLAGS) $(OFLAG) $(OBJ) -o $@

	@echo -n "\033[33m [EXECUTION] : \e[38;5;221m Compiling ft_irc... "
	@total_iterations=30; \
	i=0; \
	while [ $$i -lt $$total_iterations ]; do \
		sleep 0.03; \
		printf "\e[38;5;212m▇\033[0m"; \
		i=$$((i + 1)); \
	done
	@echo
	@echo
	@echo "\e[38;5;219m =========== Compilation completed successfully.=========== \033[0m"
	@echo 
	@echo "\e[38;5;214m		    ██╗██████╗  ██████╗ \033[0m"
	@echo "\e[38;5;215m		    ██║██╔══██╗██╔════╝ \033[0m"
	@echo "\e[38;5;216m		    ██║██████╔╝██║      \033[0m"
	@echo "\e[38;5;217m		    ██║██╔══██╗██║      \033[0m"
	@echo "\e[38;5;218m		    ██║██║  ██║╚██████╗ \033[0m"
	@echo "\e[38;5;219m		    ╚═╝╚═╝  ╚═╝ ╚═════╝ \033[0m"



				
$(OBJ_DIR)%.o:  src/%.cpp
			    @mkdir -p $(OBJ_DIR)
			    @$(CC) $(CFLAGS) $(OFLAG) -c $< -o $@ 

clean:
				@rm -rf $(OBJ_DIR)
				@echo "\033[1;35m======== irc object files removed ========\033[0m"

fclean:         clean
				@$(RM) $(NAME)
				@echo "\033[1;31m========  irc executable removed  ========\033[0m"

re:             fclean all

.PHONY: clean fclean all re
