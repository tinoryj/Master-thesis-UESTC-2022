#src/server
add_library(dataSR STATIC dataSR.cpp)
add_library(dedupCore STATIC dedupCore.cpp)
add_library(storage STATIC storageCore.cpp)
add_library(powServer STATIC powServer.cpp)

set(SERVER_OBJ dataSR  dedupCore powServer storage kmServer) 

add_executable(server-sgx ServerMain.cpp)

target_link_libraries(server-sgx ${SERVER_OBJ} ${LINK_OBJ})
