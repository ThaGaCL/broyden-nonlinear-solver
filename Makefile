PROG = broyden
OBJ_DIR = objs
HEADERS_DIR = headers
SRC_DIR = srcs

TEST_PROG = tests
TEST_DIR = unittests
TEST_OBJ_DIR = $(TEST_DIR)/objs

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TEST_SRCS = $(TEST_DIR)/main.cpp $(wildcard $(TEST_DIR)/test_*.cpp)
TEST_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(patsubst $(TEST_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SRCS))

# Compilers
CC = gcc
CXX = g++

# Controle da macro de entrega:
# make            -> compila com -DENTREGA
# make ENTREGA=0  -> compila sem -DENTREGA
ENTREGA ?= 1
ENTREGA_FLAG := $(if $(filter 1,$(ENTREGA)),-DENTREGA)

# Flags
AVX_FLAGS = -mavx -march=native -fopt-info-vec
CFLAGS = -O3 $(AVX_FLAGS) -Wno-unused-result -Wall -Wextra -I$(HEADERS_DIR) -std=gnu11 $(ENTREGA_FLAG)
CXXFLAGS = -O3 $(AVX_FLAGS) -Wno-unused-result -Wall -Wextra -I$(HEADERS_DIR) -std=gnu++11
LFLAGS = -lm

# Arquivos para o tar
DISTFILES = *.c *.h Makefile README.md
DISTDIR = nigr21-tgcl21

.PHONY: all clean purge dist test

# Regra principal: liga os objetos para criar o executável
all: $(PROG)

# Regra para criar o executável principal
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

# Regra para gerar arquivos objeto a partir dos arquivos fonte
# O $< pega o arquivo .c e o $@ define o nome do .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Regra para criar o executável de testes
$(TEST_PROG): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

# Regra para gerar arquivos objeto de teste a partir dos arquivos fonte de teste
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJ_DIR):
	@mkdir -p $(TEST_OBJ_DIR)

# Regra para compilar os testes
test: $(TEST_PROG)

# Regra para rodar o programa principal
run: $(PROG)
	@./$(PROG)

# Regra para rodar os testes
run-tests: $(TEST_PROG)
	@./$(TEST_PROG)

clean:
	@echo "Limpando sujeira..."
	@rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR) *~ *.bak core $(PROG) $(TEST_PROG) a.out

purge: clean
	@echo "Limpando tudo..."
	@rm -f $(DISTDIR).tgz

dist: purge
	@echo "Gerando arquivo de distribuição ($(DISTDIR).tgz)..."
	@ln -s . $(DISTDIR)
	@tar -chvzf $(DISTDIR).tgz $(addprefix $(DISTDIR)/, $(DISTFILES))
	@rm -f $(DISTDIR)