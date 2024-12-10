# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -g -fprofile-arcs -ftest-coverage

# Include Paths
OATPP_INCLUDE_LIB = /usr/local/include/oatpp-1.3.0/oatpp
OATPP_INCLUDE = -I src -I $(OATPP_INCLUDE_LIB)

# Full Paths to Static Libraries
OATPP_LIB = /usr/local/lib/oatpp-1.3.0/liboatpp.a
OATPP_TEST_LIB = /usr/local/lib/oatpp-1.3.0/liboatpp-test.a

# Directories
SRC_DIR = src
SRC_DIR_SERVER = src/server
SRC_DIR_DTO = src/dto
SRC_DIR_CONTROLLER = src/controller
SRC_DIR_SERVICE = src/service
SRC_DIR_CLIENT = src/client
SRC_DIR_TESTS = src/test
SRC_DIR_TESTS_Selene = src/test/designpatterns
SRC_DIR_TESTS_VERSIONCONTROL = src/test/versioncontrol
SRC_DIR_TESTS_TESTING = src/test/testing
SRC_DIR_TESTS_SOFTWARE = src/test/software
SRC_DIR_TESTS_OPTIMUSPRIME = src/test/optimusprime
TEST_DIR = src/test

# Tools
GCOV = gcov
LCOV = lcov
COVERAGE_RESULTS = results.coverage
COVERAGE_DIR = coverage

STATIC_ANALYSIS = cppcheck
STYLE_CHECK = cpplint

PROGRAM_SERVER = seleneServer
TEST_SERVER = testServer

# Phony Targets
.PHONY: all clean clean-gcov docker start stop coverage docs static style

# Default Target
all: $(PROGRAM_SERVER) coverage docs static style

# Compilation Rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Targets
clean: clean-gcov
	rm -rf *~ \
		$(COVERAGE_RESULTS) \
		$(PROGRAM_SERVER) \
		$(TEST_SERVER) \
		$(COVERAGE_DIR) \
		doxygen/html \
		obj bin \
		$(shell find . -name '*.o' -print)

clean-gcov:
	rm -rf *.gcov *.gcda *.gcno

# Build Server Program
$(PROGRAM_SERVER): $(SRC_DIR_SERVER) $(SRC_DIR_SERVICE)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM_SERVER) $(OATPP_INCLUDE) \
		$(SRC_DIR_SERVER)/*.cpp \
		$(SRC_DIR_DTO)/*.hpp \
		$(SRC_DIR_CONTROLLER)/*.hpp \
		$(OATPP_LIB) \
		$(OATPP_TEST_LIB)

# Docker Targets
docker:
	docker build --pull --rm -f "Dockerfile" -t selene:latest "."

start:
	docker compose -f "compose.yml" up -d --build

stop:
	docker compose -f "compose.yml" down

# Build Test Program
$(TEST_SERVER): $(SRC_DIR_TESTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_SERVER) $(OATPP_INCLUDE) \
		$(SRC_DIR_TESTS_Selene)/*.cpp \
		$(SRC_DIR_TESTS_VERSIONCONTROL)/*.cpp \
		$(SRC_DIR_TESTS_TESTING)/*.cpp \
		$(SRC_DIR_TESTS_SOFTWARE)/*.cpp \
		$(SRC_DIR_TESTS_OPTIMUSPRIME)/*.cpp \
		$(SRC_DIR_TESTS)/*.cpp \
		$(OATPP_LIB) \
		$(OATPP_TEST_LIB)

# Static Analysis
static: $(SRC_DIR_SERVER) $(SRC_DIR_CLIENT) $(SRC_DIR_SERVICE) $(TEST_DIR)
	$(STATIC_ANALYSIS) --verbose --enable=all \
		$(SRC_DIR_SERVER) $(SRC_DIR_CLIENT) $(SRC_DIR_SERVICE) $(TEST_DIR) $(SRC_INCLUDE) \
		--suppress=missingInclude


style: ${SRC_DIR_SERVICE} ${SRC_INCLUDE}
	${STYLE_CHECK} src/controller/* src/dto/* src/server/* src/service/*/* src/test/*/* src/test/*.cpp


# Documentation
docs:
	doxygen Doxyfile

# Code Coverage
coverage: $(TEST_SERVER)
	./$(TEST_SERVER)
	$(LCOV) --capture --gcov-tool $(GCOV) --directory . --output-file $(COVERAGE_RESULTS) --rc lcov_branch_coverage=1
	$(LCOV) --extract $(COVERAGE_RESULTS) */*/*/$(SRC_DIR)/* -o $(COVERAGE_RESULTS)
	genhtml $(COVERAGE_RESULTS) --output-directory $(COVERAGE_DIR)
	make clean-gcov


