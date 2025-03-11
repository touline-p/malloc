echo listing sources build/lib_sources.cmake;
echo sources found:;
echo set\(SOURCES > build/lib_sources.cmake;
find src/ -name '*.cpp' | sed 's/^/\t/' | tee -a build/lib_sources.cmake;
find src/ -name '*.c' | sed 's/^/\t/' | tee -a build/lib_sources.cmake;
find src/ -name '*.hpp' | sed 's/^/\t/' | tee -a build/lib_sources.cmake;
find src/ -name '*.h' | sed 's/^/\t/' | tee -a build/lib_sources.cmake;
echo \) >> build/lib_sources.cmake;

echo listing test sources in build/tests_sources.cmake;
echo set\(TEST_SOURCES > build/tests_sources.cmake;
find tests/ -name '*.cpp' | sed 's/^/\t/' | tee -a build/tests_sources.cmake;
find tests/ -name '*.c' | sed 's/^/\t/' | tee -a build/tests_sources.cmake;
find tests/ -name '*.hpp' | sed 's/^/\t/' | tee -a build/tests_sources.cmake;
find tests/ -name '*.h' | sed 's/^/\t/' | tee -a build/tests_sources.cmake;
echo \) >> build/tests_sources.cmake;

echo listing test sources in build/libft_sources.cmake;
echo set\(LIBFT_SOURCES > build/libft_sources.cmake;
find libft/ -name '*.cpp' | sed 's/^/\t/' | tee -a build/libft_sources.cmake;
find libft/ -name '*.c' | sed 's/^/\t/' | tee -a build/libft_sources.cmake;
find libft/ -name '*.hpp' | sed 's/^/\t/' | tee -a build/libft_sources.cmake;
find libft/ -name '*.h' | sed 's/^/\t/' | tee -a build/libft_sources.cmake;
echo \) >> build/libft_sources.cmake;

echo listing test sources in build/printf_sources.cmake;
echo set\(PRINTF_SOURCES > build/printf_sources.cmake;
find printf/ -name '*.cpp' | sed 's/^/\t/' | tee -a build/printf_sources.cmake;
find printf/ -name '*.c' | sed 's/^/\t/' | tee -a build/printf_sources.cmake;
find printf/ -name '*.hpp' | sed 's/^/\t/' | tee -a build/printf_sources.cmake;
find printf/ -name '*.h' | sed 's/^/\t/' | tee -a build/printf_sources.cmake;
echo \) >> build/printf_sources.cmake;
