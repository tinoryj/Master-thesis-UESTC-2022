#src/client


add_library(chunker-tz STATIC chunker.cpp)
add_library(fingerprinter-tz STATIC fingerprinter.cpp)
add_library(keyClient-tz STATIC keyClient.cpp)
add_library(encoder-tz STATIC encoder.cpp)
add_library(powClient-tz STATIC powClient.cpp)
add_library(sender-tz STATIC sender.cpp)
add_library(recvDecode-tz STATIC recvDecode.cpp)
add_library(retriever-tz STATIC retriever.cpp)
add_library(powCA-tz STATIC powTZ.cpp)
target_link_libraries (powCA-tz PRIVATE teec)


add_executable(client-tz clientMain.cpp)
install (TARGETS client-tz DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(opensslTest opensslTest.cpp)
install (TARGETS opensslTest DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(fileGen-tz fileGen.cpp)
install (TARGETS fileGen-tz DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(baselineTest-tz baselineTest.cpp)
install (TARGETS baselineTest-tz DESTINATION ${CMAKE_INSTALL_BINDIR})

target_link_libraries(client-tz ${CLIENT_OBJ} ${LINK_OBJ} teec)
target_link_libraries(opensslTest ${LINK_OBJ})