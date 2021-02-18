set(src
    engine/angle.cc engine/angle.h
    engine/main.cc engine/main.h
)

add_library(engine STATIC ${src})
target_link_libraries(engine
    PUBLIC sdl2 opengl imgui fmt::fmt stb glm
    PRIVATE project_options project_warnings
)
target_include_directories(engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

source_group("" FILES ${src})

set(src_test
    test/testangle.cc
    ../external/catch/main.cc
)
add_executable(test ${src_test})
target_link_libraries(test
    PUBLIC catch engine catchy
    PRIVATE project_options project_warnings
)
