project(bezier)

add_executable(${PROJECT_NAME} AMain.cpp)

target_sources(${PROJECT_NAME}
        PUBLIC
        Camera.cpp
        Curve.cpp
        DrawScene.cpp
        Surface.cpp
        Vector3D.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SAFESEH:NO")

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdparty
)

set(OPENGL_LIBRARIES
        ${CMAKE_SOURCE_DIR}/3rdparty/GL/GLAUX.LIB
        ${CMAKE_SOURCE_DIR}/3rdparty/GL/GLU32.lib
        ${CMAKE_SOURCE_DIR}/3rdparty/GL/GLUT32.lib
        ${CMAKE_SOURCE_DIR}/3rdparty/GL/OpenGL32.lib
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${OPENGL_LIBRARIES}
)