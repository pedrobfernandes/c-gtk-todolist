# Escolhe o compilador
ifeq ($(shell command -v gcc 2> /dev/null),)
	# Verificar se Clang está disponível
	ifeq ($(shell command -v clang 2> /dev/null),)
		$(error "GCC ou Clang não encontrados no sistema.")
	else
		CC := clang
	endif
else
	CC := gcc
endif


# Variáveis e flags de compilação
CFLAGS := -Wall -Wextra --pedantic -std=c99
LIBS := $(shell pkg-config --cflags --libs gtk+-3.0) -lsqlite3
# LDFLAGS += -rdynamic


# Diretórios de instalação
PREFIX := /usr/local
BINDIR := $(PREFIX)/bin
INCDIR := $(PREFIX)/include


# Comando de instalação
INSTALL := install


# Diretorio dos fontes e do executavel
SRC_DIR := src
BUILD_DIR := build


# Pega os fontes. whildcard pega todos os .c
# patsubst substitui os .c por .o, ou seja,
# coloca em BUILD_DIR tasks.o se em SRCS tiver tasks.c
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))


# Cabeçalhos
HEADERS := $(wildcard $(SRC_DIR)/*.h)


# Executável(eis)
TARGETS := tasks


# Comando para compilar. addprefix pega $(BUILD_DIR) que é
# o diretorio build e concatena o executável da vez
# ficando pro exemplo: build/tasks
$(addprefix $(BUILD_DIR)/, $(TARGETS)): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

# Cria os arquivos .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS) $(LIBS)


# Cria BUILD_DIR se não existir
ifeq ($(wildcard $(BUILD_DIR)),)
	@$(shell mkdir -p $(BUILD_DIR))
endif


# Instala o executável(eis) e os cabeçalhos
#install: $(addprefix $(BUILD_DIR)/, $(TARGETS))
#	$(INSTALL) -D $^ $(BINDIR)
#	$(INSTALL) -D $(HEADERS) $(INCDIR)


# Limpa os arquivos gerados
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: install clean
