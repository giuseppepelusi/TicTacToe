CC = g++
SOURCE = tris.cpp
EXEC = tris

all: clear compile

compile:
	@$(CC) $(SOURCE) -o $(EXEC)
	@echo "Compiled $(SOURCE) to $(EXEC)"

clear:
	@rm -f $(EXEC)
	@echo "Removed executable"

run:
	@echo "Executed \"$(EXEC)\""
	@./$(EXEC)

.PHONY: all compile clear run
