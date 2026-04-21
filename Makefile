# Makefile v7
ARDUINO_CLI ?= arduino-cli
CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Werror -pedantic

LIBRARY_DIR ?= .
TCP1819_DIR ?=
EXAMPLE ?= examples/ReaderAlive
FQBN ?= arduino:renesas_uno:unor4wifi
BUILD_DIR ?= build
HOST_BUILD_DIR ?= build/host

ALL_FQBNS := \
	arduino:renesas_uno:unor4wifi

HOST_TEST_SOURCES := $(sort $(wildcard tests/host/test_*.cpp))
HOST_TEST_SUPPORT_SOURCES := $(sort $(wildcard tests/host/test_support/*.cpp))
HOST_LIBRARY_SOURCES := $(sort $(wildcard src/*.cpp))
HOST_TEST_BINS := $(patsubst tests/host/%.cpp,$(HOST_BUILD_DIR)/%,$(HOST_TEST_SOURCES))

.PHONY: help compile compile-all test clean core-update core-install-renesas

help:
	@echo "Targets:"
	@echo "  make compile            Compile EXAMPLE for FQBN=$(FQBN)"
	@echo "  make compile-all        Compile the example for all default boards"
	@echo "  make test               Build and run host tests"
	@echo "  make clean              Remove build artifacts"
	@echo "  make core-update        Run arduino-cli core update-index"
	@echo "  make core-install-renesas Install arduino:renesas_uno"
	@echo
	@echo "Variables:"
	@echo "  EXAMPLE=<path>          default: examples/ReaderAlive"
	@echo "  FQBN=<fqbn>             default: arduino:renesas_uno:unor4wifi"
	@echo "  TCP1819_DIR=<path>      optional extra library path"

compile:
	@mkdir -p "$(BUILD_DIR)"
	@extra_lib_flags=''; \
	if [ -n "$(TCP1819_DIR)" ]; then \
		extra_lib_flags="--library $(TCP1819_DIR)"; \
	fi; \
	$(ARDUINO_CLI) compile \
		--fqbn "$(FQBN)" \
		--library "$(LIBRARY_DIR)" \
		$$extra_lib_flags \
		--build-path "$(BUILD_DIR)/$$(echo '$(FQBN)' | tr ':' '_')" \
		"$(EXAMPLE)"

compile-all:
	@set -e; \
	for fqbn in $(ALL_FQBNS); do \
		echo "Compiling $(EXAMPLE) for $$fqbn"; \
		$(MAKE) --no-print-directory compile FQBN=$$fqbn TCP1819_DIR="$(TCP1819_DIR)"; \
	done

test: $(HOST_TEST_BINS)
	@set -e; \
	for test_bin in $(HOST_TEST_BINS); do \
		echo "Running $$(basename "$$test_bin")"; \
		"$$test_bin"; \
	done

$(HOST_BUILD_DIR)/test_%: tests/host/test_%.cpp $(HOST_LIBRARY_SOURCES) $(HOST_TEST_SUPPORT_SOURCES)
	@mkdir -p "$(HOST_BUILD_DIR)"
	@$(CXX) $(CXXFLAGS) \
		-I./src \
		-I./tests/host \
		$^ \
		-o "$@"

clean:
	rm -rf "$(BUILD_DIR)"

core-update:
	$(ARDUINO_CLI) core update-index

core-install-renesas:
	$(ARDUINO_CLI) core install arduino:renesas_uno
# Makefile v7