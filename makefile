CC = g++
SOURCE = tris.cpp
EXEC = tris

all: clear compile run

compile:
	@$(CC) $(SOURCE) -o $(EXEC)
	@echo "Compiled $(SOURCE) to $(EXEC)"

clear:
	@rm -f $(EXEC)
	@echo "Removed executable"

run:
	@echo "Executed \"$(EXEC)\""
	@./$(EXEC)
