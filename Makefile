# thanks to chat gpt for this
TARGET = gbdollop

SRC_DIR = source
INC_DIR = include

# lista di sorgenti, ovvero tutti i file .c dentro source
SRCS = $(wildcard $(SRC_DIR)/*.c)

# a partire dai sorgeti crea la lista di file in output
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(SRC_DIR)/%.o)

# dipendenze
DEPS = $(OBJS:.o=.d)

# seleziona il compilatore e le sue flag
CC = gcc
CFLAGS = -I$(INC_DIR) -Wall -Wextra -MMD -MP -g -Wno-unused-parameter -Wno-unused-function

# le librerie da compilare
LIBS = -lm

# definisci come compilare e effettuare il link
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

# regole per la pulizia
clean:
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.d $(TARGET)

# compila tutti i target
all: $(TARGET)

# Phony targets per evitare conflitti con file aventi lo stesso nome
# .PHONY: clean all