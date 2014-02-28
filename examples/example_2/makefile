# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

UNITY_ROOT=../..
C_COMPILER=gcc
TARGET_BASE1=all_tests
ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif
TARGET1 = $(TARGET_BASE1)$(TARGET_EXTENSION)
SRC_FILES1=\
  $(UNITY_ROOT)/src/unity.c \
  $(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
  src/ProductionCode.c \
  src/ProductionCode2.c \
  test/TestProductionCode.c \
  test/TestProductionCode2.c \
  test/test_runners/TestProductionCode_Runner.c \
  test/test_runners/TestProductionCode2_Runner.c \
  test/test_runners/all_tests.c
INC_DIRS=-Isrc -I$(UNITY_ROOT)/src -I$(UNITY_ROOT)/extras/fixture/src
SYMBOLS=

ifeq ($(OS),Windows_NT)
	CLEANUP = del /F /Q $(TARGET1)
else
	CLEANUP = rm -f build/*.o ; rm -f $(TARGET1)
endif

all: clean default

default:
#	ruby auto/generate_test_runner.rb test/TestProductionCode.c  test/test_runners/TestProductionCode_Runner.c
#	ruby auto/generate_test_runner.rb test/TestProductionCode2.c test/test_runners/TestProductionCode2_Runner.c
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES1) -o $(TARGET1)
	./$(TARGET1)

clean:
	$(CLEANUP)

