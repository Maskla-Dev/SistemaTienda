add_library(Utils OBJECT Messages.h Messages.c SemLogic.c SemLogic.h SharedMemory.c SharedMemory.h ImpresoraMensajes.c ImpresoraMensajes.h Utils.c Utils.h)
target_include_directories(Utils PUBLIC .)