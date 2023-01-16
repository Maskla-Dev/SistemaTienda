add_executable(TestSupervisor TestSupervisor.c)
target_include_directories(TestSupervisor PRIVATE ../../src)
target_link_libraries(TestSupervisor PRIVATE OperacionTienda)
target_link_libraries(TestSupervisor PRIVATE Utils)

add_test(NAME IniciarRecursos COMMAND TestSupervisor 1)
add_test(NAME DireccionarCliente COMMAND TestSupervisor 2)