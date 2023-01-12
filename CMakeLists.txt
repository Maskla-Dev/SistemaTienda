cmake_minimum_required(VERSION 3.24)
project(SistemaTienda C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(Utils)

add_executable(Supervisor Supervisor.c Supervisor.h Guardia.h Guardia.c "Asistente de Compra.c" "Asistente de Compra.h" Vendedor.c Vendedor.h)
add_executable(Cliente Cliente.c Cliente.h)
target_link_libraries(Cliente PRIVATE Utils)
target_link_libraries(Supervisor PRIVATE Utils)