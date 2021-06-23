NAME	=	philo
SRC		=	src
INC		=	inc
HEADER	=	philo.h

CFILES	=	main.c \
			philo.c

SOURCES	=	$(addprefix $(SRC)/, $(CFILES))
HFILES	=	$(addprefix $(INC)/, $(HEADER))

FLAGS	= -Wall -Wextra -Werror

OBJECTS	= ${SOURCES:.c=.o}

all: ${NAME}

%.o: %.c ${HFILES}
	gcc ${FLAGS} -I ${INC} ${SOURCES} -o $@

clean:
	rm -f ${OBJECTS}
fclean: clean
	rm -f ${NAME}
re: fclean all

norm:
	norminette src/* inc/*

.PHONY: all clean fclean re

