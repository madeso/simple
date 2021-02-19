set(src
    engine/angle.cc engine/angle.h
    engine/axisangle.cc engine/axisangle.h
    engine/vec2.cc engine/vec2.h
    engine/vec3.cc engine/vec3.h
    engine/mat33.cc engine/mat33.h
    engine/mat44.cc engine/mat44.h
    engine/matrixhelper.cc engine/matrixhelper.h
    engine/math1.cc engine/math1.h
    engine/quat.cc engine/quat.h

    engine/meshdef.cc engine/meshdef.h
    engine/pushedmatrix.cc engine/pushedmatrix.h
    engine/cpp.cc engine/cpp.h
    engine/filesystem.cc engine/filesystem.h
    engine/media.cc engine/media.h
    engine/medialoader.cc engine/medialoader.h

    engine/texture.cc engine/texture.h
    engine/material.cc engine/material.h

    engine/poseable.cc engine/poseable.h
    engine/animation.cc engine/animation.h

    engine/meshpart.cc engine/meshpart.h
    engine/renderable.cc engine/renderable.h
    engine/renderlist.cc engine/renderlist.h

    engine/main.cc engine/main.h
)

find_package(Filesystem REQUIRED)

add_library(engine STATIC ${src})
target_link_libraries(engine
    PUBLIC sdl2 opengl imgui fmt::fmt stb glm glad
    PRIVATE project_options project_warnings std::filesystem
)
target_include_directories(engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

source_group("" FILES ${src})

set(src_test
    test/test.cc test/test.h
    test/testangle.cc
    test/testvec.cc
    test/testquat.cc
    test/testmat33.cc
    test/testmat44.cc
    ../external/catch/main.cc
)
add_executable(test ${src_test})
target_link_libraries(test
    PUBLIC catch engine catchy
    PRIVATE project_options project_warnings
)
