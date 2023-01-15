add_subdirectory(Utils)
add_library(OperacionTienda OBJECT "Asistente de Compra.h" "Asistente de Compra.c" Cliente.c Cliente.h Guardia.c Guardia.h Supervisor.c Supervisor.h Vendedor.h Vendedor.c)
target_include_directories(OperacionTienda PUBLIC .)