CC = gcc
ifeq ($(shell uname -s), Darwin)
CC = clang
endif
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
BUILD_DIR = ../build
TARGET = ../build/fixture_tests.exe

all: default noStdlibMalloc 32bits

default: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -D UNITY_SUPPORT_64
	@ echo "default build"
	./$(TARGET)

32bits: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -m32
	@ echo "32bits build"
	./$(TARGET)

noStdlibMalloc: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -D UNITY_EXCLUDE_STDLIB_MALLOC
	@ echo "build with noStdlibMalloc"
	./$(TARGET)

clang89: ../build/
	clang $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -m32 -std=c89 -Wno-comment
	clang $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -m32 \
		-D UNITY_EXCLUDE_STDLIB_MALLOC -std=c89 -Wno-comment ; ./$(TARGET)

clangEverything:
	clang $(CFLAGS) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET) -Weverything

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(TARGET) $(BUILD_DIR)/*.gc*

coverage: $(BUILD_DIR)
	cd $(BUILD_DIR) && \
	$(CC) $(DEFINES) $(foreach i, $(SRC), ../test/$(i)) $(INC_DIR) -o $(TARGET) -fprofile-arcs -ftest-coverage
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) > /dev/null ; ./$(TARGET) -v > /dev/null
	cd $(BUILD_DIR) && \
	gcov unity_fixture.c | head -3
	grep '###' $(BUILD_DIR)/unity_fixture.c.gcov -C2 || true # Show uncovered lines

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
CFLAGS += -Wno-error=undef  # Warning only, this should not stop the build
CFLAGS += -Wunreachable-code
CFLAGS += -Wunused
CFLAGS += -fstrict-aliasing
