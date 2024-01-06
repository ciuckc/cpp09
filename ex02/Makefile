NAME = PMergeMe

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++23

SRC_DIR = src

SRC = main.cpp

OBJ_DIR = obj

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

INC = include

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -I $(INC) $^ -o $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I $(INC) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) -r $(NAME)

re: fclean
	$(MAKE) -j4

.PHONY: fclean clean re all
