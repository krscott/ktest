add_library(ktest ktest.c)

target_include_directories(ktest PUBLIC ${PROJECT_SOURCE_DIR}/include)

install(TARGETS ktest)
