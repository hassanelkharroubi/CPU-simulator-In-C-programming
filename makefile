# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g

# Fichiers source
SRCS = hashmap.c memory_handler.c test_memory.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Nom de l'exécutable final
EXEC = tests

# Règle par défaut : compiler l'exécutable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

# Règle pour compiler les fichiers .c en .o
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJS) $(EXEC)

# Règle pour recompiler tout depuis zéro
fclean: clean
	rm -f $(EXEC)

# Règle pour recompiler les tests
re: fclean $(EXEC)

# Règle pour exécuter les tests
run: $(EXEC)
	./$(EXEC)
