CC=mpicc
SRC_DIR=src
BIN_DIR=bin
SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(SOURCES:%.c=%)
CFLAGS := -std=c99 -Wall

all: $(OBJS)
	@doxygen mpi.dox # Compilar documentación
	@echo Done\!

%:
	@if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi # Crear directorio 'bin'
	$(CC) $(CFLAGS) $(@).c -o $(BIN_DIR)/$(@F)

clean:
	@rm -rf $(BIN_DIR) # Eliminar directorio 'bin' y sus contenidos
	@echo Done\!
