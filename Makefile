CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include
SOURCES = main.cpp src/calendario.cpp src/console.cpp src/extrato.cpp src/compra.cpp src/investimento.cpp src/cartao.cpp src/conta.cpp src/pessoa.cpp src/carteira.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = bin/main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
