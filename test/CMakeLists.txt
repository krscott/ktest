file(
  GLOB TEST_SRCS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "*.c")

foreach(src IN LISTS TEST_SRCS)
  get_filename_component(name ${src} NAME_WE) # filename without extension

  add_executable(${name} ${src})
  add_test(NAME ${name} COMMAND ${name})
  set_tests_properties(
    ${name}
    PROPERTIES ENVIRONMENT "KTEST_NAME=;KTEST_STOP_ON_FAIL=;KTEST_LIST=;KTEST_VERBOSE=")
  target_include_directories(${name} PUBLIC ${PROJECT_SOURCE_DIR}/include)
endforeach()
