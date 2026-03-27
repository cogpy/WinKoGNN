# WinKoGNN Unified AGI-OS — Makefile
#
# Usage:
#   make                 — Build all targets
#   make test            — Build and run all tests
#   make cli             — Build CLI only
#   make clean           — Remove build artifacts
#
# Configuration:
#   COGPY_HPP=../cogpy-hpp   — Path to cogpy-hpp repo
#   COG0=../cog0             — Path to cog0 repo

COGPY_HPP ?= ../cogpy-hpp
COG0 ?= ../cog0

CC = gcc
CXX = g++
CFLAGS = -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable \
         -Wno-missing-field-initializers -Wno-multichar
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable \
           -Wno-missing-field-initializers -Wno-multichar
INCLUDES = -I include/nt_compat -I include/cogpy -I $(COGPY_HPP)/include

NT_STUBS = src/nt_compat/nt_stubs.c

.PHONY: all test cli clean

all: tests/test_nt_compat_bin tests/test_main_bin winkognn_cli

tests/test_nt_compat_bin: tests/test_nt_compat.c $(NT_STUBS)
	$(CC) $(CFLAGS) -I include/nt_compat -o $@ $^

tests/test_main_bin: tests/test_main.cpp $(NT_STUBS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

winkognn_cli: src/cli/main.cpp $(NT_STUBS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -O2 -o $@ $^

cli: winkognn_cli

test: tests/test_nt_compat_bin tests/test_main_bin
	@echo "=== NT Compatibility Tests ==="
	@./tests/test_nt_compat_bin
	@echo ""
	@echo "=== Unified AGI-OS Tests ==="
	@./tests/test_main_bin

clean:
	rm -f tests/test_nt_compat_bin tests/test_main_bin winkognn_cli
