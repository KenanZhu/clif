add_executable(CLIF_example main.cpp)

target_include_directories(CLIF_example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/funcs
)
target_link_libraries(CLIF_example PRIVATE CLIF)

install(TARGETS CLIF_example DESTINATION bin)