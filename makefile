INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj
CXX = g++
CXX_FLAGS = -I$(INC_DIR)
CXX_FLAGS += -march=native -O2

SRC = $(wildcard $(SRC_DIR)/*.cpp)
DEP = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SRC))
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

PROGRAM = 6dj 

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CXX) $(CXX_FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -MMD -MP -o $@ -c $<

$(OBJ_DIR): 
	mkdir $@
 

-include $(DEP)

clean:
	rm -rf $(OBJ_DIR)
	rm $(PROGRAM)
	