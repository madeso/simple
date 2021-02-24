set(src
    main.cc
    demo.cc demo.h
)

add_executable(demo WIN32 MACOSX_BUNDLE ${src})
target_link_libraries(
    demo
    PUBLIC engine
    PRIVATE project_options project_warnings
)
target_include_directories(demo PRIVATE ${EXAMPLE_FOLDER})

source_group("" FILES ${src})
