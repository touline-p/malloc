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
