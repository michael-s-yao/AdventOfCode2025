CXX = g++
CXXFLAGS = -std=c++14 -Wall
SRCDIR = src
BUILD_DIR = bin
SRCS := $(wildcard $(SRCDIR)/*.cpp)
EXES := $(patsubst $(SRCDIR)/%.cpp,$(BUILD_DIR)/%,$(SRCS))
NAMES := $(notdir $(basename $(SRCS)))

all: $(EXES)

$(BUILD_DIR)/%: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(NAMES): %: $(BUILD_DIR)/%
	@true

clean:
	rm -rf $(BUILD_DIR) && mkdir -p $(BUILD_DIR)
