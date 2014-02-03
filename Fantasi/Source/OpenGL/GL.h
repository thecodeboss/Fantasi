#ifndef GL_h__
#define GL_h__

#include <windows.h>

#define GL_GLEXT_PROTOTYPES
#define WGL_WGLEXT_PROTOTYPES

// GL/GLEW Libraries
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GLU.h>

// GLM Libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Typedefs
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;

#endif // GL_h__
