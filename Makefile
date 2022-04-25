
GPP = g++ -g
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXE = exe

all: $(OBJ)
	$(GPP) -iquote header -g -o $(EXE) main.cpp $(OBJ) `pkg-config gtkmm-3.0 --cflags --libs`
	rm src/*.o
	@echo "$(EXE) prêt!"


%.o: %.cpp
	$(GPP) -iquote header -g -c $< -o $@ `pkg-config gtkmm-3.0 --cflags --libs`
	
clean:
	rm -f $(EXE) src/*.o
	@echo "$(EXE) et *.o effacé !"