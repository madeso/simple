set(src_math
    engine/angle.cc engine/angle.h
    engine/axisangle.cc engine/axisangle.h
    engine/vec2.cc engine/vec2.h
    engine/vec3.cc engine/vec3.h
    engine/mat33.cc engine/mat33.h
    engine/mat44.cc engine/mat44.h
    engine/matrixhelper.cc engine/matrixhelper.h
    engine/math1.cc engine/math1.h
    engine/quat.cc engine/quat.h
    engine/arcball.cc engine/arcball.h
    engine/rect.cc engine/rect.h
    engine/rotation.cc engine/rotation.h
)

 set(src_core
    engine/cpp.cc engine/cpp.h
    engine/filesystem.cc engine/filesystem.h
    engine/window.cc engine/window.h
    engine/strings.cc engine/strings.h
    engine/stringcounter.cc engine/stringcounter.h
    engine/xml.cc engine/xml.h
    engine/userexception.cc engine/userexception.h
    engine/fileutil.cc engine/fileutil.h
    engine/ckey.cc engine/ckey.h
    engine/stringseperator.cc engine/stringseperator.h
    engine/pool.cc engine/pool.h
    engine/key.cc engine/key.h
    engine/main.cc engine/main.h
    engine/opengl.debug.cc engine/opengl.debug.h
 )

 set(src_root
    engine/meshdef.cc engine/meshdef.h
    engine/pushedmatrix.cc engine/pushedmatrix.h
    engine/media.cc engine/media.h
    engine/medialoader.cc engine/medialoader.h
    engine/texture.cc engine/texture.h
    engine/material.cc engine/material.h
    engine/poseable.cc engine/poseable.h
    engine/animation.cc engine/animation.h
    engine/meshpart.cc engine/meshpart.h
    engine/renderable.cc engine/renderable.h
    engine/renderlist.cc engine/renderlist.h
    engine/compiledmesh.cc engine/compiledmesh.h
    engine/actor.cc engine/actor.h
    engine/camera.cc engine/camera.h
    engine/renderablegrid.cc engine/renderablegrid.h
    engine/actorfile.cc engine/actorfile.h
    engine/world.cc engine/world.h
    engine/simpleworld.cc engine/simpleworld.h
    engine/entity.cc engine/entity.h
    engine/meshinstance.cc engine/meshinstance.h
    engine/mesh.cc engine/mesh.h
    engine/shader.cc engine/shader.h
    engine/fbo.cc engine/fbo.h
    engine/fullscreenquad.cc engine/fullscreenquad.h
    engine/setup.cc engine/setup.h
)

 set(src_load
    engine/meshfile.cc engine/meshfile.h
    engine/animationfile.cc engine/animationfile.h
    engine/load/milkshape.common.cc engine/load/milkshape.common.h
    engine/load/milkshape.ascii.cc engine/load/milkshape.ascii.h
    engine/load/milkshape.binary.cc engine/load/milkshape.binary.h
)

 set(src_fse
    engine/fse/renderargs.cc engine/fse/renderargs.h
    engine/fse/map.cc engine/fse/map.h
    engine/fse/bufferreference.cc engine/fse/bufferreference.h
    engine/fse/binder.cc engine/fse/binder.h
    engine/fse/target.cc engine/fse/target.h
    engine/fse/command.cc engine/fse/command.h
    engine/fse/provider.cc engine/fse/provider.h
    engine/fse/pipeline.cc engine/fse/pipeline.h
    engine/fse/linker.cc engine/fse/linker.h
    engine/fse/command.setvec2uniform.cc engine/fse/command.setvec2uniform.h
    engine/fse/command.bindbuffer.cc engine/fse/command.bindbuffer.h
    engine/fse/command.commands.cc engine/fse/command.commands.h
    engine/fse/provider.renderworld.cc engine/fse/provider.renderworld.h
    engine/fse/provider.renderfullscreen.cc engine/fse/provider.renderfullscreen.h
    engine/fse/provider.providers.cc engine/fse/provider.providers.h
    engine/fse/target.buffer.cc engine/fse/target.buffer.h
    engine/fse/target.form.cc engine/fse/target.form.h
    engine/fse/target.targets.cc engine/fse/target.targets.h
)

set(src ${src_fse} ${src_load} ${src_core} ${src_root} ${src_math})

find_package(Filesystem REQUIRED)

add_library(engine STATIC ${src})
target_link_libraries(engine
    PUBLIC sdl2 opengl imgui fmt::fmt stb glm glad
    PRIVATE project_options project_warnings std::filesystem tinyxml2
)
target_include_directories(engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

source_group("" FILES ${src_root})
source_group("math" FILES ${src_math})
source_group("load" FILES ${src_load})
source_group("core" FILES ${src_core})
source_group("fse" FILES ${src_fse})

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
