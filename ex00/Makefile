NAME = btc

CXX = clang++

CXXFLAGS = -Wall -Wextra -Werror -Wshadow -Wpedantic -std=c++20

INCLUDE = inc

SRC_DIR = src

SRC = main.cpp PriceHistory.cpp InputData.cpp

OBJ_DIR = obj

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE) $^ -o $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean
	$(MAKE) -j 4
