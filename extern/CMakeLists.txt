set(
    LIBRARY_SOURCES
    "glad/glad.c"

)
set(
    LIBRARY_HEADERS
    "glad/glad.h"
    "stb/stb_image.h"
    "stb/stb_image_write.h"
    "stb/stb_include.h"
)
set(
    LIBRARY_INCLUDES
    "./glad"
    "./stb"
)
add_library(
    ${EXTERN_LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS}
)
target_include_directories(
    ${EXTERN_LIBRARY_NAME} PUBLIC
    ${LIBRARY_INCLUDES}
)
