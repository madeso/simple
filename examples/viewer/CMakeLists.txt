set(src
    main.cc
    viewer.cc viewer.h
)

add_executable(viewer WIN32 MACOSX_BUNDLE ${src})
target_link_libraries(
    viewer
    PUBLIC engine imgui_filebrowser
    PRIVATE project_options project_warnings
)
target_include_directories(viewer PRIVATE ${EXAMPLE_FOLDER})

source_group("" FILES ${src})
