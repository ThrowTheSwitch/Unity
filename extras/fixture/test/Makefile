CC = gcc
DEFINES = -D UNITY_OUTPUT_CHAR=UnityOutputCharSpy_OutputChar
SRC = ../src/unity_fixture.c \
      ../../../src/unity.c   \
      unity_fixture_Test.c   \
      unity_fixture_TestRunner.c \
      unity_output_Spy.c     \
      main/AllTests.c

INC_DIR = -I../src -I../../../src/
TARGET = fixture_tests.exe

all:
	@ $(CC) $(DEFINES) $(SRC) $(INC_DIR) -o $(TARGET)
	@ ./$(TARGET)
