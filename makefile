C_COMPILER=gcc
OUT_FILE=-o testunity
ifeq ($(OS),Windows_NT)
OUT_EXTENSION=.exe
else
OUT_EXTENSION=.out
endif
SRC_FILES=src/unity.c test/testunity.c test/testunity_Runner.c
INC_DIRS=-Isrc
SYMBOLS=-DTEST

default:
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) $(OUT_FILE)$(OUT_EXTENSION)
