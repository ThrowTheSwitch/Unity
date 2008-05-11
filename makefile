C_COMPILER=gcc
TARGET_BASE = testunity
ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif
TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)
OUT_FILE=-o $(TARGET)
SRC_FILES=src/unity.c test/testunity.c test/testunity_Runner.c
INC_DIRS=-Isrc
SYMBOLS=-DTEST

ifeq ($(OS),Windows_NT)
	CLEANUP = del /F /Q bin\* && del /F /Q $(TARGET)
else
	CLEANUP = rm -f bin/*.o ; rm -f $(TARGET)
endif

all: clean default

default:
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) $(OUT_FILE)$(OUT_EXTENSION)
	$(TARGET)

clean:
	$(CLEANUP)
	
