NAME		=	webserv
CPP			=	c++

CPPFLAGS		=	-fsanitize=address -g3#-Wall -Wextra -Werror -std=c++98 
RM			=	rm -rf
SRCS		=	main.cpp src/parseConfigFile.cpp src/parseLocation.cpp


OBJS		= ${SRCS:.cpp=.opp}

all			:	$(NAME)

$(NAME)		: $(SRCS)
			$(CPP) $(CPPFLAGS) $(SRCS) -o $(NAME)
			
clean		:
			$(RM) $(OBJS)

fclean		: clean
			$(RM) $(NAME)

re			: fclean all

.PHONY: all clean fclean re