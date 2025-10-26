# Nom de l'exécutable
EXEC = main

# Dossiers
SRC_DIR = .
TAD_DIR = ./tad

# Fichiers sources
SRC = $(SRC_DIR)/main.c \
      $(TAD_DIR)/city.c \
      $(TAD_DIR)/matrix.c \
      $(TAD_DIR)/tsp.c

# Fichiers objets (automatiquement dérivés)
OBJ = $(SRC:.c=.o)

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm

# Règle principale
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Compilation des .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	rm -f $(OBJ)

# Nettoyage complet (objets + exécutable)
fclean: clean
	rm -f $(EXEC)

# Recompilation complète
re: fclean $(EXEC)

# Règles non liées à des fichiers
.PHONY: clean fclean re