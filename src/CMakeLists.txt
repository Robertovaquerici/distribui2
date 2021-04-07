add_executable(clientBinary keys.h client.c)
add_executable(serverBinary server.c)

target_link_libraries(serverBinary pthread)