# Variabili del compilatore
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude  # -Iinclude è vitale per trovare gli header
LDFLAGS = -pthread                   # Necessario per la sezione Concurrency

# Percorsi
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Trova tutti i file .c in src/ e il main.c
SRCS = $(wildcard $(SRC_DIR)/*.c) main.c
# Genera i nomi dei file .o dentro la cartella obj/
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

# Nome dell'eseguibile
TARGET = astrostream

# Regola principale
all: $(TARGET)

# Linker: unisce i file oggetto per creare l'eseguibile
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compilatore: crea i file oggetto dai sorgenti .c
# Crea la cartella obj se non esiste
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia
clean:
	rm -rf $(OBJ_DIR) $(TARGET) compile_commands.json

.PHONY: all clean
