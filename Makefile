# Nome do executável final
PROG = broyden

# Objetos necessários
OBJS = main.o broyden.o utils.o

# Flags
AVX_FLAGS = -mavx2 -march=native -fopt-info-vec
CC = gcc
CFLAGS = -O3 $(AVX_FLAGS) -Wno-unused-result
LFLAGS = -lm

# Arquivos para o tar
DISTFILES = *.c *.h Makefile README.md
DISTDIR = login-dinf

.PHONY: all clean purge dist

# Regra principal: liga os objetos para criar o executável
all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

# Regra para transformar .c em .o
# O $< pega o arquivo .c e o $@ define o nome do .o
%.o: %.c broyden.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Limpando sujeira..."
	@rm -f *.o *~ *.bak core 

purge: clean
	@echo "Limpando tudo..."
	@rm -f $(PROG) a.out $(DISTDIR).tgz

dist: purge
	@echo "Gerando arquivo de distribuição ($(DISTDIR).tgz)..."
	@ln -s . $(DISTDIR)
	@tar -chvzf $(DISTDIR).tgz $(addprefix $(DISTDIR)/, $(DISTFILES))
	@rm -f $(DISTDIR)