CPP  = g++
CC   = gcc
OBJ  = main.o "texture_manager/bmp.o" "texture_manager/texture.o" volumes.o snake.o $(RES)
LINKOBJ  = main.o "texture_manager/bmp.o" "texture_manager/texture.o" volumes.o snake.o $(RES)
LIBS = -lglut
INCS = 
CXXINCS =  
BIN  = Snake3D
CXXFLAGS =  -pg -Wall -pedantic $(CXXINCS)
CFLAGS = -pg -Wall -pedantic $(INCS)
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before Snake3D all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "Snake3D" $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

"texture_manager/bmp.o": texture_manager/bmp.cpp
	$(CPP) -c "texture_manager/bmp.cpp" -o "texture_manager/bmp.o" $(CXXFLAGS)

"texture_manager/texture.o": texture_manager/texture.cpp
	$(CPP) -c "texture_manager/texture.cpp" -o "texture_manager/texture.o" $(CXXFLAGS)

snake.o: snake.cpp
	$(CPP) -c snake.cpp -o snake.o $(CXXFLAGS)

volumes.o: volumes.cpp
	$(CPP) -c volumes.cpp -o volumes.o $(CXXFLAGS)
