set(src
    main.cc
    viewer.cc
)

add_executable(viewer WIN32 MACOSX_BUNDLE ${src})
target_link_libraries(
    viewer
    PUBLIC engine
    PRIVATE project_options project_warnings
)

source_group("" FILES ${src})
