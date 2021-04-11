add_executable(clientBinary keys.c keys.h  client.c)
add_executable(serverBinary server.c database.c database.h)

target_link_libraries(serverBinary pthread)