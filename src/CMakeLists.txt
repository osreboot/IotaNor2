target_sources(${PROJECT_NAME} PRIVATE
        display.h
        display.cpp
        painter.h
        painter.cpp
        quad.h
        quad.cpp
        texture.h
        texture.cpp
        texture_frame_buffer.h
        texture_frame_buffer.cpp
        shader.h
        shader.cpp
        )

add_subdirectory(game)