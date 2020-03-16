CC = gcc
ifeq ($(shell uname -s), Darwin)
CC = clang
endif
ifeq ($(findstring clang, $(CC)), clang)
E = -Weverything
CFLAGS += $E -Wno-unknown-warning-option -Wno-missing-prototypes
CFLAGS += -Wno-unused-macros -Wno-padded -Wno-missing-noreturn
endif
CFLAGS += -std=c99 -pedantic -Wall -Wextra -Werror
#CFLAGS += -Wconversion #disabled because if falsely complains about the isinf and isnan macros
CFLAGS += -Wno-switch-enum -Wno-double-promotion
CFLAGS += -Wbad-function-cast -Wcast-qual -Wold-style-definition -Wshadow -Wstrict-overflow \
          -Wstrict-prototypes -Wswitch-default -Wundef
#DEBUG = -O0 -g
CFLAGS += $(DEBUG)
UNITY_SUPPORT_64 = -D UNITY_SUPPORT_64
UNITY_INCLUDE_DOUBLE = -D UNITY_INCLUDE_DOUBLE
DEFINES =  -D UNITY_OUTPUT_CHAR=putcharSpy
DEFINES += -D UNITY_OUTPUT_CHAR_HEADER_DECLARATION=putcharSpy\(int\)
DEFINES += -D UNITY_OUTPUT_FLUSH=flushSpy
DEFINES += -D UNITY_OUTPUT_FLUSH_HEADER_DECLARATION=flushSpy\(void\)
DEFINES += $(UNITY_SUPPORT_64) $(UNITY_INCLUDE_DOUBLE)
SRC1 = ../src/unity.c tests/test_unity_arrays.c build/test_unity_arraysRunner.c
SRC2 = ../src/unity.c tests/test_unity_core.c build/test_unity_coreRunner.c
SRC3 = ../src/unity.c tests/test_unity_doubles.c build/test_unity_doublesRunner.c
SRC4 = ../src/unity.c tests/test_unity_floats.c build/test_unity_floatsRunner.c
SRC5 = ../src/unity.c tests/test_unity_integers.c build/test_unity_integersRunner.c
SRC6 = ../src/unity.c tests/test_unity_integers_64.c build/test_unity_integers_64Runner.c
SRC7 = ../src/unity.c tests/test_unity_memory.c build/test_unity_memoryRunner.c
SRC8 = ../src/unity.c tests/test_unity_strings.c build/test_unity_stringsRunner.c
INC_DIR = -I ../src
COV_FLAGS = -fprofile-arcs -ftest-coverage -I ../../src
BUILD_DIR = build
TARGET = build/testunity-cov.exe

# To generate coverage, call 'make -s', the default target runs.
# For verbose output of all the tests, run 'make test'.
default: coverage
.PHONY: default coverage test clean
coverage: $(SRC1) $(SRC2) $(SRC3) $(SRC4) $(SRC5) $(SRC6) $(SRC7) $(SRC8)
	cd $(BUILD_DIR) && \
	$(CC) $(CFLAGS) $(DEFINES) $(foreach i,$(SRC1), ../$i) $(COV_FLAGS) -o ../$(TARGET)
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) | grep 'Tests\|]]]' -A1
	cd $(BUILD_DIR) && \
	gcov unity.c | head -3
	grep '###' $(BUILD_DIR)/unity.c.gcov -C2 || true
	cd $(BUILD_DIR) && \
	$(CC) $(CFLAGS) $(DEFINES) $(foreach i,$(SRC2), ../$i) $(COV_FLAGS) -o ../$(TARGET)
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) | grep 'Tests\|]]]' -A1
	cd $(BUILD_DIR) && \
	gcov unity.c | head -3
	grep '###' $(BUILD_DIR)/unity.c.gcov -C2 || true
	cd $(BUILD_DIR) && \
	$(CC) $(CFLAGS) $(DEFINES) $(foreach i,$(SRC3), ../$i) $(COV_FLAGS) -o ../$(TARGET)
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) | grep 'Tests\|]]]' -A1
	cd $(BUILD_DIR) && \
	gcov unity.c | head -3
	grep '###' $(BUILD_DIR)/unity.c.gcov -C2 || true
	cd $(BUILD_DIR) && \
	$(CC) $(CFLAGS) $(DEFINES) $(foreach i,$(SRC4), ../$i) $(COV_FLAGS) -o ../$(TARGET)
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) | grep 'Tests\|]]]' -A1
	cd $(BUILD_DIR) && \
	gcov unity.c | head -3
	grep '###' $(BUILD_DIR)/unity.c.gcov -C2 || true
	cd $(BUILD_DIR) && \
	$(CC) $(CFLAGS) $(DEFINES) $(foreach i,$(SRC5), ../$i) $(COV_FLAGS) -o ../$(TARGET)
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) | grep 'Tests\|]]]' -A1
	cd $(BUILD_DIR) && \
	gcov unity.c | head -3
	grep '###' $(BUILD_DIR)/unity.c.gcov -C2 || true
	cd $(BUILD_DIR) && \
	$(CC) $(CFLAGS) $(DEFINES) $(foreach i,$(SRC6), ../$i) $(COV_FLAGS) -o ../$(TARGET)
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) | grep 'Tests\|]]]' -A1
	cd $(BUILD_DIR) && \
	gcov unity.c | head -3
	grep '###' $(BUILD_DIR)/unity.c.gcov -C2 || true
	cd $(BUILD_DIR) && \
	$(CC) $(CFLAGS) $(DEFINES) $(foreach i,$(SRC7), ../$i) $(COV_FLAGS) -o ../$(TARGET)
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) | grep 'Tests\|]]]' -A1
	cd $(BUILD_DIR) && \
	gcov unity.c | head -3
	grep '###' $(BUILD_DIR)/unity.c.gcov -C2 || true
	cd $(BUILD_DIR) && \
	$(CC) $(CFLAGS) $(DEFINES) $(foreach i,$(SRC8), ../$i) $(COV_FLAGS) -o ../$(TARGET)
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) | grep 'Tests\|]]]' -A1
	cd $(BUILD_DIR) && \
	gcov unity.c | head -3
	grep '###' $(BUILD_DIR)/unity.c.gcov -C2 || true

test: $(SRC1) $(SRC2) $(SRC3) $(SRC4) $(SRC5) $(SRC6) $(SRC7) $(SRC8)
	$(CC) $(CFLAGS) $(DEFINES) $(INC_DIR) $(SRC1) -o $(TARGET)
	./$(TARGET)
	$(CC) $(CFLAGS) $(DEFINES) $(INC_DIR) $(SRC2) -o $(TARGET)
	./$(TARGET)
	$(CC) $(CFLAGS) $(DEFINES) $(INC_DIR) $(SRC3) -o $(TARGET)
	./$(TARGET)
	$(CC) $(CFLAGS) $(DEFINES) $(INC_DIR) $(SRC4) -o $(TARGET)
	./$(TARGET)
	$(CC) $(CFLAGS) $(DEFINES) $(INC_DIR) $(SRC5) -o $(TARGET)
	./$(TARGET)
	$(CC) $(CFLAGS) $(DEFINES) $(INC_DIR) $(SRC6) -o $(TARGET)
	./$(TARGET)
	$(CC) $(CFLAGS) $(DEFINES) $(INC_DIR) $(SRC7) -o $(TARGET)
	./$(TARGET)
	$(CC) $(CFLAGS) $(DEFINES) $(INC_DIR) $(SRC8) -o $(TARGET)
	./$(TARGET)

# Compile only, for testing that preprocessor detection works
UNITY_C_ONLY =-c ../src/unity.c -o $(BUILD_DIR)/unity.o
intDetection:
	$(CC) $(CFLAGS) $(INC_DIR) $(UNITY_C_ONLY) -D UNITY_EXCLUDE_STDINT_H
	$(CC) $(CFLAGS) $(INC_DIR) $(UNITY_C_ONLY) -D UNITY_EXCLUDE_LIMITS_H

$(BUILD_DIR)/test_unity_arraysRunner.c: tests/test_unity_arrays.c | $(BUILD_DIR)
	awk $(AWK_SCRIPT) tests/test_unity_arrays.c > $@

$(BUILD_DIR)/test_unity_coreRunner.c: tests/test_unity_core.c | $(BUILD_DIR)
	awk $(AWK_SCRIPT) tests/test_unity_core.c > $@

$(BUILD_DIR)/test_unity_doublesRunner.c: tests/test_unity_doubles.c | $(BUILD_DIR)
	awk $(AWK_SCRIPT) tests/test_unity_doubles.c > $@

$(BUILD_DIR)/test_unity_floatsRunner.c: tests/test_unity_floats.c | $(BUILD_DIR)
	awk $(AWK_SCRIPT) tests/test_unity_floats.c > $@

$(BUILD_DIR)/test_unity_integersRunner.c: tests/test_unity_integers.c | $(BUILD_DIR)
	awk $(AWK_SCRIPT) tests/test_unity_integers.c > $@

$(BUILD_DIR)/test_unity_integers_64Runner.c: tests/test_unity_integers_64.c | $(BUILD_DIR)
	awk $(AWK_SCRIPT) tests/test_unity_integers_64.c > $@

$(BUILD_DIR)/test_unity_memoryRunner.c: tests/test_unity_memory.c | $(BUILD_DIR)
	awk $(AWK_SCRIPT) tests/test_unity_memory.c > $@

$(BUILD_DIR)/test_unity_stringsRunner.c: tests/test_unity_strings.c | $(BUILD_DIR)
	awk $(AWK_SCRIPT) tests/test_unity_strings.c > $@

AWK_SCRIPT=\
  '/^void test/{ declarations[d++]=$$0; gsub(/\(?void\)? ?/,""); tests[t++]=$$0; line[u++]=NR } \
  END{ print "\#include \"unity.h\" /* Autogenerated by awk in Makefile */" ;                   \
       for (i=0; i<d; i++) { print declarations[i] ";" }                                        \
       print "int main(void)\n{\n    UnityBegin(\"" FILENAME "\");" ;                           \
       for (i=0; i<t; i++) { print "    RUN_TEST(" tests[i] ", " line[i] ");" }                 \
       print "    return UNITY_END();\n}" }'

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(TARGET) $(BUILD_DIR)/*.gc* $(BUILD_DIR)/test_unity_*Runner.c
