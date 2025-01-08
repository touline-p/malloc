NAME=bad-zapple
BUILD_DIR=./build
TEST_DIR=./tests
TARGET=$(BUILD_DIR)/$(NAME).a
CMAKEFILE=$(BUILD_DIR)/Makefile

all: 
	make -C $(BUILD_DIR) all

.PHONY: all 

$(TARGET): $(CMAKEFILE)
	make -C $(BUILD_DIR) $(NAME)

clean:
	make clean -C $(NAME)

.PHONY: clean 

fclean:
	rm -rf $(BUILD_DIR)

.PHONY: fclean 

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_DIR):
	mkdir -p $(TEST_DIR)


$(CMAKEFILE): build/lib_sources.cmake build/tests_sources.cmake
	cmake -B $(BUILD_DIR)

.PHONY: bld

build/lib_sources.cmake: update-sources

update-sources: $(BUILD_DIR) $(TEST_DIR)
	@sh ./tools/list_sources.sh 

.PHONY: update-sources 

lsp: update-sources
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B $(BUILD_DIR)

.PHONY: lsp 

.PHONY: tests 

run-test: all
	ctest --test-dir build

install-tools:
	./tools/pre-commit-tester-installer.sh

.PHONY: run-tests
