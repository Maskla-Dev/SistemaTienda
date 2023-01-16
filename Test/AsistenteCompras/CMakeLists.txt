add_executable(TestAsistente TestAsistente.c)
target_include_directories(TestAsistente PRIVATE ../../src)
target_link_libraries(TestGuardia PRIVATE OperacionTienda)
target_link_libraries(TestGuardia PRIVATE Utils)