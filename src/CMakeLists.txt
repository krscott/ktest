add_library(cstart cstart.c)

target_include_directories(cstart PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_sources(cstart PRIVATE strbuf.c)

install(TARGETS cstart)
