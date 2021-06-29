NAME	=	philo
SRC		=	src
INC		=	inc
HEADER	=	philo.h
CFILES	=	main.c \
			init.c \
			philo.c \
			end.c \
			utils.c
SOURCES	=	$(addprefix $(SRC)/, $(CFILES))
HFILES	=	$(addprefix $(INC)/, $(HEADER))
FLAGS	= -Wall -Wextra -Werror
OBJECTS	= ${SOURCES:.c=.o}

all: ${NAME}

$(NAME): ${OBJECTS}
	gcc -I ${INC} $(OBJECTS) -o $(NAME)

%.o: %.c ${HFILES}
	gcc ${FLAGS} -I ${INC} -c $< -o $@

clean:
	rm -f ${OBJECTS}
fclean: clean
	rm -f ${NAME}
re: fclean all

norm:
	norminette src/* inc/*

.PHONY: all clean fclean re
