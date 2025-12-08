CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wno-deprecated-declarations
CPPFLAGS = -I./src -I/usr/include/UnitTest++
# Для тестов добавляем флаг UNIT_TESTS
TEST_CPPFLAGS = $(CPPFLAGS) -DUNIT_TESTS

LDFLAGS = -lssl -lcrypto
TEST_LDFLAGS = -lssl -lcrypto -lUnitTest++

TARGET = server
TEST_TARGET = run_tests

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
TEST_BUILD_DIR = build/tests

# Исходные файлы
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Тестовые файлы
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(TEST_BUILD_DIR)/%.o,$(TEST_SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Правило для тестов
tests: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) $(filter-out $(BUILD_DIR)/main.o, $(OBJS))
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TEST_LDFLAGS)

# Ключевое изменение: используем TEST_CPPFLAGS для тестов
$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_CPPFLAGS) -I$(SRC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET) vcalc.log

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean run test tests