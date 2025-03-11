### Variables ###
TARGET = dining_philosophers_problem # Name of executable file

CXX = g++ #Compiler (g++)

CXXFLAGS = -Wall -std=c++20 -pthread # Compiler Flags

SRC = dining_ph_problem_P1.cpp # source file

OBJ = $(SRC:.cpp=.o) # object file translation from .cpp

### Compile rules ###
all: $(TARGET) # target is to compile the program

$(TARGET): $(OBJ)                               
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)  # Make executable file from obj file

%.o: %.c			         
	$(CXX) $(CXXFLAGS) -c $< -o $@   # Make obj file from src file
	
clean:
	rm -f $(OBJ) $(TARGET) # Delete obj and executable files
