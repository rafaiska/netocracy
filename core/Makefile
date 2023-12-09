GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include
PROJECT_INCLUDE = include

G++ = g++
G++_FLAGS = -c -Wall -I $(GOOGLE_TEST_INCLUDE) -I $(PROJECT_INCLUDE)
LD_FLAGS = -L /usr/local/lib -l $(GOOGLE_TEST_LIB) -l pthread

OBJECTS = obj/test_all.o obj/test_cn_timer.o obj/cn_timer.o
TARGET = test_all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) $(LD_FLAGS)

obj/%.o : src/test/%.cpp
	$(G++) -o $@ $(G++_FLAGS) $<

obj/%.o : src/%.cpp
	$(G++) -o $@ $(G++_FLAGS) $<

clean:
	rm -f $(TARGET) $(OBJECTS)
                    
.PHONY: all clean