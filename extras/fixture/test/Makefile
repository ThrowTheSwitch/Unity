CC = gcc
#DEBUG = -O0 -g
CFLAGS += -std=c99
CFLAGS += -pedantic
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += $(DEBUG)
DEFINES = -D UNITY_OUTPUT_CHAR=UnityOutputCharSpy_OutputChar
SRC = ../src/unity_fixture.c \
      ../../../src/unity.c   \
      unity_fixture_Test.c   \
      unity_fixture_TestRunner.c \
      unity_output_Spy.c     \
      main/AllTests.c

INC_DIR = -I../src -I../../../src/
TARGET = ../build/fixture_tests.exe

all: default noStdlibMalloc 32bits

default: ../build/
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET)
	@ echo "default build"
	./$(TARGET)

32bits: ../build/
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -m32
	@ echo "32bits build"
	./$(TARGET)

noStdlibMalloc: ../build/
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -D UNITY_EXCLUDE_STDLIB_MALLOC
	@ echo "build with noStdlibMalloc"
	./$(TARGET)

clangEverything:
	clang $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -m64 -Weverything

../build :
	mkdir -p ../build

clean:
	rm -f $(TARGET)

# These extended flags DO get included before any target build runs
CFLAGS += -Wbad-function-cast
CFLAGS += -Wcast-qual
CFLAGS += -Wconversion
CFLAGS += -Wformat=2
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wold-style-definition
CFLAGS += -Wpointer-arith
CFLAGS += -Wshadow
CFLAGS += -Wstrict-overflow=5
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wswitch-default
CFLAGS += -Wundef
CFLAGS += -Wunused
CFLAGS += -fstrict-aliasing
