add_executable(TestCliente TestCliente.c)
target_include_directories(TestCliente PRIVATE ../../src)
target_link_libraries(TestGuardia PRIVATE OperacionTienda)
target_link_libraries(TestGuardia PRIVATE Utils)