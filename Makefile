NAME		=	webserv
CPP			=	c++

CPPFLAGS	=  #-fsanitize=address -g3 #-std=c++98
RM			=	rm -rf
SRCS		=	main.cpp src/parseConfigFile.cpp src/parseLocation.cpp \
				src/utils.cpp src/setData.cpp src/setDataLocation.cpp \
				src/dataValidity.cpp src/multiplexing.cpp src/Request/Request.cpp \
				#src/Response/Response.cpp  #src/Response/Post.cpp 
				


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