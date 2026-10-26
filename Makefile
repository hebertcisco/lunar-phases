.PHONY: all build test sanitize clean install

BUILD_DIR ?= build

all: build

build:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

sanitize:
	cmake -S . -B build-sanitize -DLUNAR_ENABLE_SANITIZERS=ON
	cmake --build build-sanitize
	ctest --test-dir build-sanitize --output-on-failure

install: build
	cmake --install $(BUILD_DIR)

clean:
	cmake --build $(BUILD_DIR) --target clean
