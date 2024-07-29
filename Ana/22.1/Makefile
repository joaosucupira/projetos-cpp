# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -g

# Nome do executável
EXEC = diagnosticador

# Arquivos fontes e objetos
SRCS = main.cpp hormonio.cpp endorfina.cpp serotonina.cpp reg_paciente.cpp diagnosticador.cpp
OBJS = $(SRCS:.cpp=.o)

# Regras
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
