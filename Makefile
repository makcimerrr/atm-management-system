SRC		=	./src/main.c		\
			./src/system.o		\
			./src/auth.o		\

NAME	=	atm

BIN		= 	unit_tests

OBJ		=	$(SRC:.c=.o)

CFLAGS	= 	-Wall -Wextra -I ./include -Wno-implicit-function-declaration

DEBUGFL	=	-g 	-ggdb3

all:		$(OBJ)
			gcc -o $(NAME) $(OBJ) $(CFLAGS)

clean:
			rm -f $(OBJ)
			rm -f *.gcno
			rm -f *.gcda

fclean:		clean
			rm -f $(NAME)
			rm -f $(BIN)

re:			fclean all

debug:		CFLAGS += $(DEBUGFL)
debug:		$(OBJ)
			gcc -o $(NAME) $(OBJ) $(CFLAGS)

.PHONY	=	all clean fclean re debug