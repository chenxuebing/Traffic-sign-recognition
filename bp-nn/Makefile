NAME = train

SRC = main.cpp database.cpp

OB = $(SRC:.cpp=.o)

FLAGS = -std=c++11 -O3

OPENCV = `pkg-config --cflags --libs opencv`

all: $(NAME)

$(NAME): $(OB)
		g++ -o $(NAME) $(OB) $(OPENCV)

%.o : %.cpp
		g++ $(FLAGS) -c -o  $@ $<

clean:
		rm -f $(OB)

fclean: clean
		rm -f $(NAME)

re: clean all
