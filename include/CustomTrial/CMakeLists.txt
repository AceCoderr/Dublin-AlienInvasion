set(
LIBRARY_SOURCES
"helloworld.cpp"
"ParticleSystem.cpp"
"Helper.cpp"
"Particle.cpp"
"Renderer.cpp"
)
set(
LIBRARY_HEADERS
"helloworld.h"
"shader_m.h"
"Camera.h"
"filesystem.h"
"model.h"
"mesh.h"
"animation.h"
"animator.h"
"animdata.h"
"assimp_glm_helpers.h"
"bone.h"
"Helper.h"
"Particle.h"
"ParticleSystem.h"
"Renderer.h"
)
set(
LIBRARY_INCLUDES
"./"
)
# STATIC/DYNAMIC LINKING OF LIBRARIES
add_library(
${LIBRARY_NAME} STATIC
${LIBRARY_SOURCES}
${LIBRARY_HEADERS}
)
set(
    EXTERN_LIBS
    glm::glm
    assimp
    glfw
    Vulkan-Headers
)
# Including the current directory for libraries
target_include_directories(
${LIBRARY_NAME} PUBLIC
${LIBRARY_INCLUDES}
)

# Linking the external libraries that we got from FetchContent
# Linking the libraries to our custom library (LIBRARY_NAME)
# We use project_name::library_name (Both can be found in the CMakeLists.txt for the library)
target_link_libraries(
    ${LIBRARY_NAME} PUBLIC
    ${EXTERN_LIBS}
    ${EXTERN_LIBRARY_NAME}

)
