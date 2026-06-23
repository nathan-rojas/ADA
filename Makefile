# Variables de configuración
CXX = g++
CXXFLAGS = -std=c++17 -I/mingw64/include
LDFLAGS = -L/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system

# Nombre del ejecutable final
TARGET = VrpSolver

# Archivos objeto
OBJS = main.o SRC/SavingsAlgorithm.o SRC/GuiApp.o

# Regla principal
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

SRC/SavingsAlgorithm.o: SRC/SavingsAlgorithm.cpp
	$(CXX) $(CXXFLAGS) -c SRC/SavingsAlgorithm.cpp -o SRC/SavingsAlgorithm.o

SRC/GuiApp.o: SRC/GuiApp.cpp
	$(CXX) $(CXXFLAGS) -c SRC/GuiApp.cpp -o SRC/GuiApp.o

# Regla para limpiar archivos temporales
clean:
	rm -f main.o SRC/SavingsAlgorithm.o SRC/GuiApp.o VrpSolver.exemingw32-make clean