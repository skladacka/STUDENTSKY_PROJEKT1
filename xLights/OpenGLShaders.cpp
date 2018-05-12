#include "wx/wx.h"

#ifndef __WXMAC__
#include <GL/gl.h>
#ifdef _MSC_VER
#include "GL\glext.h"
#else
#include <GL/glext.h>
#endif

#include "xlGLCanvas.h"

extern PFNGLCREATESHADERPROC      glCreateShader;
extern PFNGLSHADERSOURCEPROC      glShaderSource;
extern PFNGLCOMPILESHADERPROC     glCompileShader;
extern PFNGLGETSHADERIVPROC       glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC     glCreateProgram;
extern PFNGLATTACHSHADERPROC      glAttachShader;
extern PFNGLLINKPROGRAMPROC       glLinkProgram;
extern PFNGLDETACHSHADERPROC      glDetachShader;
extern PFNGLDELETESHADERPROC      glDeleteShader;
extern PFNGLDELETEPROGRAMPROC     glDeleteProgram;
extern PFNGLGETPROGRAMIVPROC      glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

static bool canUseShaders()
{
   return glCreateShader != nullptr
      && glShaderSource != nullptr
      && glCompileShader != nullptr
      && glGetShaderiv != nullptr
      && glGetShaderInfoLog != nullptr
      && glCreateProgram != nullptr
      && glAttachShader != nullptr
      && glLinkProgram != nullptr
      && glDetachShader != nullptr
      && glDeleteShader != nullptr
      && glDeleteProgram != nullptr
      && glGetProgramiv != nullptr
      && glGetProgramInfoLog != nullptr;
}
#else
#include "OpenGL/gl.h"

static bool canUseShaders()
{
   return true;
}
#endif

#include "OpenGLShaders.h"

#include "DrawGLUtils.h"

#include <log4cpp/Category.hh>

namespace
{
   bool shaderLinkSuceeded( GLuint programID )
   {
      GLint result = GL_FALSE;
      glGetProgramiv( programID, GL_LINK_STATUS, &result );
      if ( result == GL_FALSE )
      {
         int infoLogLength = 0;
         glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLogLength );
         if ( infoLogLength > 0 )
         {
            std::vector<char> errorMessage( infoLogLength + 1 );
            char*             messagePtr = &errorMessage[0];
            glGetProgramInfoLog( programID, infoLogLength, NULL, messagePtr );
            static log4cpp::Category &logger_opengl = log4cpp::Category::getInstance( std::string( "log_opengl" ) );
            logger_opengl.error( "shader-link failure: '%s'", messagePtr );
         }
      }
      return result == GL_TRUE;
   }

   static bool shaderCompileSuceeded( GLuint shaderID )
   {
      GLint result = GL_FALSE;
      glGetShaderiv( shaderID, GL_COMPILE_STATUS, &result );
      if ( result == GL_FALSE )
      {
         int infoLogLength = 0;
         glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
         if ( infoLogLength > 0 )
         {
            std::vector<char> errorMessage( infoLogLength + 1 );
            char*             messagePtr = &errorMessage[0];
            glGetShaderInfoLog( shaderID, infoLogLength, NULL, messagePtr );
            static log4cpp::Category &logger_opengl = log4cpp::Category::getInstance( std::string( "log_opengl" ) );
            logger_opengl.error( "shader-compile failure: '%s'", messagePtr );
         }
      }
      return result == GL_TRUE;
   }

   const char* mediaRenderVertexFromUniformSource = "    \n\
      in vec4 pos;                                       \n\
      in vec2 texCoordIn;                                \n\
      out vec2 texCoord;                                 \n\
      uniform mat4 MVP;                                  \n\
      void main( void )                                  \n\
      {                                                  \n\
         gl_Position = MVP * pos;                        \n\
         texCoord = texCoordIn;                          \n\
      }                                                  \n\
";
}

bool OpenGLShaders::HasShaderSupport()
{
   return canUseShaders();
}

unsigned OpenGLShaders::compile( const std::string& vertexSource, const std::string& fragmentSource )
{
   GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
   const GLchar* vertexShaders[] = { vertexSource.c_str() };
   glShaderSource( vertexShader, 1, vertexShaders, NULL );
   glCompileShader( vertexShader );
   if ( !shaderCompileSuceeded( vertexShader ) )
      return 0;

   GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
   const GLchar* fragmentShaders[] = { fragmentSource.c_str() };
   glShaderSource( fragmentShader, 1, fragmentShaders, NULL );
   glCompileShader( fragmentShader );
   bool fragCompileSuccess = shaderCompileSuceeded( fragmentShader );
   if ( !fragCompileSuccess )
   {
      glDeleteShader( vertexShader );
      return 0;
   }

   GLuint program = glCreateProgram();
   glAttachShader( program, vertexShader );
   glAttachShader( program, fragmentShader );
   glLinkProgram( program );
   bool linkSuccess = shaderLinkSuceeded( program );

   glDetachShader( program, vertexShader );
   glDetachShader( program, fragmentShader );

   glDeleteShader( vertexShader );
   glDeleteShader( fragmentShader );

   return linkSuccess ? program : 0;
}
