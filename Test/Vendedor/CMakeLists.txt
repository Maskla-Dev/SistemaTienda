add_executable(TestVendedor TestVendedor.c)
target_include_directories(TestVendedor PRIVATE ../../src)
target_link_libraries(TestVendedor PRIVATE OperacionTienda)
target_link_libraries(TestVendedor PRIVATE Utils)

add_test(NAME ObtenerInventario COMMAND TestVendedor 1)
add_test(NAME BuscarArticulo COMMAND TestVendedor 2)