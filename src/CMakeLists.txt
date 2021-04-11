add_executable(clientBinary keys.c keys.h database.c database.h client.c)
add_executable(serverBinary server.c)

target_link_libraries(serverBinary pthread)