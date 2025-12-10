add_executable(ktest-app main.c)

target_link_libraries(ktest-app ktest libkcli.a)

install(TARGETS ktest-app)
