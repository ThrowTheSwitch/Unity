CC = gcc
CFLAGS += -Werror
CFLAGS += -std=c99
CFLAGS += -pedantic
CFLAGS += -Wundef
DEFINES = -D UNITY_OUTPUT_CHAR=UnityOutputCharSpy_OutputChar
SRC = ../src/unity_fixture.c \
      ../../../src/unity.c   \
      unity_fixture_Test.c   \
      unity_fixture_TestRunner.c \
      unity_output_Spy.c     \
      main/AllTests.c

INC_DIR = -I../src -I../../../src/
TARGET = fixture_tests.exe

all: default noStdlibMalloc 32bits

default:
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET)
	@ echo "default build"
	./$(TARGET)

32bits:
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -m32
	@ echo "32bits build"
	./$(TARGET)

noStdlibMalloc:
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -D UNITY_EXCLUDE_STDLIB_MALLOC
	@ echo "build with noStdlibMalloc"
	./$(TARGET)

clangEverything:
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -m64 -Weverything # || true #prevents make from failing
