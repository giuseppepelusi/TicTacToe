CC = g++
SOURCE = TicTacToe.cpp
EXEC = TicTacToe
INSTALL_PATH = /usr/local/bin

RESET = \033[0m
YELLOW = \033[0;33m
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[1;34m

all: clear compile

compile:
	$(CC) $(SOURCE) -o $(EXEC)
	echo "$(YELLOW)•$(RESET) Compiled"

clear:
	rm -f $(EXEC)
	echo "$(YELLOW)•$(RESET) Removed executable"

install: all
	sudo cp $(EXEC) $(INSTALL_PATH)
	rm -f $(EXEC)
	echo "$(GREEN)•$(RESET) Installed \033[1;32m$(EXEC)$(RESET) to $(BLUE)$(INSTALL_PATH)$(RESET)"

uninstall:
	sudo rm -f $(INSTALL_PATH)/$(EXEC)
	echo "$(RED)•$(RESET) Uninstalled \033[1;32m$(EXEC)$(RESET) from $(BLUE)$(INSTALL_PATH)$(RESET)"

run:
	echo "$(YELLOW)•$(RESET) Executed"
	./$(EXEC)

.PHONY: all compile clear install uninstall run
.SILENT: all compile clear install uninstall run
