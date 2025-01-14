CC = g++

CFLAGS  = -g -Wall -std=c++11 -O3 -I./include

TARGET = gfastats
BUILD_PATH = build/bin
SOURCE_PATH = src

LIBS += -lz


$(TARGET): $(SOURCE_PATH)/$(TARGET).cpp
	mkdir -p $(BUILD_PATH)
	$(CC) $(CFLAGS) -o $(BUILD_PATH)/$(TARGET) $(SOURCE_PATH)/$(TARGET).cpp $(LIBS)

clean:
	$(RM) -r build
