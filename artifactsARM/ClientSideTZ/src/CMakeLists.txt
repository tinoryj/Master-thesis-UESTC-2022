#/

include_directories(${BOOST_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIRS} ../include ./trustzone/ta/include)
link_directories(${BOOST_LIBRARY_DIR} ${OPENSSL_LIBRARY_DIR} ./util)

set(BOOST_LIBRARY_OBJ ${BOOST_LIBRARY_DIR}/libboost_thread.a ${BOOST_LIBRARY_DIR}/libboost_system.a ${BOOST_LIBRARY_DIR}/libboost_serialization.a)
set(SYSTEM_LIBRARY_OBJ pthread rt dl)
set(OPENSSL_LIBRARY_OBJ ssl crypto)
set(UTIL_OBJ configure-tz cryptoPrimitive-tz SSL_TLS-tz)

set(CLIENT_OBJ chunker-tz fingerprinter-tz keyClient-tz encoder-tz powClient-tz sender-tz recvDecode-tz retriever-tz powCA-tz)
set(LINK_OBJ ${UTIL_OBJ} ${OPENSSL_LIBRARY_OBJ} ${BOOST_LIBRARY_OBJ} ${SYSTEM_LIBRARY_OBJ})

add_subdirectory(client)
add_subdirectory(util)