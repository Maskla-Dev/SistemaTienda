add_executable(Test TestTienda.c)
target_include_directories(Test PRIVATE ../src)
target_link_libraries(Test PRIVATE OperacionTienda)
target_link_libraries(Test PRIVATE Utils)

add_test(NAME Supervisor-IniciarRecursos COMMAND Test 1)
add_test(NAME Supervisor-DireccionarCliente COMMAND Test 2)
add_test(NAME Guardia-ExtraerDatosUsuario COMMAND Test 3)
add_test(NAME Guardia-DescomponerPeticion COMMAND Test 4)