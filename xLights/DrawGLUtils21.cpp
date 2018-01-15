
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __WXMAC__
#include "OpenGL/gl.h"
#else

#include "wx/wx.h"

#include <GL/gl.h>
#ifdef _MSC_VER
//#include "GL/glut.h"
#include "GL/glext.h"
#else
#include <GL/glext.h>
#endif
#endif

#include "DrawGLUtils.h"

#include <log4cpp/Category.hh>

#include <stack>

#ifndef __WXMAC__
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;
extern PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;
#ifndef LINUX
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
#endif
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLPOINTPARAMETERFPROC glPointParameterf;
#endif

extern void LoadGLFunctions();

class OpenGL21Cache : public DrawGLUtils::xlGLCacheInfo {
public:


    OpenGL21Cache() : matrix(nullptr) {
        LOG_GL_ERRORV(glEnable(GL_COLOR_MATERIAL));
        LOG_GL_ERRORV(glDisable(GL_TEXTURE_2D));

        GLuint VertexShaderIDc = CompileShader(GL_VERTEX_SHADER,
            "#version 120\n"
            "attribute vec2 vertexPosition_modelspace;\n"
            "attribute vec4 vertexColor;\n"
            "varying vec4 fragmentColor;\n"
            "uniform mat4 MVP;\n"
            "void main(){\n"
            "    gl_Position = MVP * vec4(vertexPosition_modelspace,0,1);\n"
            "    fragmentColor = vertexColor;\n"
            "}\n");
        GLuint VertexShaderIDsc = CompileShader(GL_VERTEX_SHADER,
           "#version 120\n"
           "attribute vec2 vertexPosition_modelspace;\n"
           "varying vec4 fragmentColor;\n"
           "uniform vec4 color;\n"
           "uniform mat4 MVP;\n"
           "void main(){\n"
           "    gl_Position = MVP * vec4(vertexPosition_modelspace,0,1);\n"
           "    fragmentColor = color;\n"
           "}\n");
        GLuint FragmentShaderID = CompileShader(GL_FRAGMENT_SHADER,
            "#version 120\n"
            "varying vec4 fragmentColor;\n"
            "uniform int RenderType = 0;\n"
            "uniform float PointSmoothMin = 0.5;\n"
            "uniform float PointSmoothMax = 0.75;\n"
            "void main(){\n"
            "    if (RenderType == 0) {\n"
            "        gl_FragColor = fragmentColor;\n"
            "    } else {\n"
            "        float dist = distance(gl_PointCoord, vec2(0.5));\n"
            "        float alpha = 1.0 - smoothstep(PointSmoothMin, PointSmoothMax, dist);\n"
            "        if (alpha == 0.0) discard;\n"
            "        alpha = alpha * fragmentColor.a;\n"
            "        gl_FragColor = vec4(fragmentColor.rgb, alpha);\n"
            "    }\n"
            "}\n");

        ProgramIDcolors = LinkProgram(VertexShaderIDc, FragmentShaderID);
        ProgramIDstaticColor = LinkProgram(VertexShaderIDsc, FragmentShaderID);

        GLuint VertexShaderIDtx = CompileShader(GL_VERTEX_SHADER,
            "#version 120\n"
            "attribute vec2 vertexPosition_modelspace;\n"
            "attribute vec2 texturePos;\n"
            "varying vec2 textCoord;\n"
            "uniform mat4 MVP;\n"
            "void main(){\n"
            "    gl_Position = MVP * vec4(vertexPosition_modelspace,0,1);\n"
            "    textCoord = texturePos;\n"
            "}\n");
        GLuint FragmentShaderIDtxt = CompileShader(GL_FRAGMENT_SHADER,
            "#version 120\n"
            "varying vec2 textCoord;\n"
            "uniform sampler2D tex;\n"
            "uniform vec4 inColor;\n"
            "void main(){\n"
            "    gl_FragColor = vec4(texture2D(tex, textCoord).rgb, texture2D(tex, textCoord).a * inColor.a);\n"
            "}\n");

        ProgramIDtexture = LinkProgram(VertexShaderIDtx, FragmentShaderIDtxt);
        
        LOG_GL_ERRORV(glDeleteShader(VertexShaderIDtx));
        LOG_GL_ERRORV(glDeleteShader(FragmentShaderIDtxt));
        LOG_GL_ERRORV(glDeleteShader(VertexShaderIDc));
        LOG_GL_ERRORV(glDeleteShader(VertexShaderIDsc));
        LOG_GL_ERRORV(glDeleteShader(FragmentShaderID));

        
        LOG_GL_ERRORV(glUseProgram(ProgramIDcolors));
        LOG_GL_ERRORV(MatrixIDc = glGetUniformLocation(ProgramIDcolors, "MVP"));
        LOG_GL_ERRORV(MatrixIDsc = glGetUniformLocation(ProgramIDstaticColor, "MVP"));
        LOG_GL_ERRORV(MatrixIDt = glGetUniformLocation(ProgramIDcolors, "MVP"));
        
        
        isIntel = wxString(glGetString(GL_VENDOR)).Contains("Intel");
        //isIntel = true;
    }
    
    GLuint CompileShader(GLenum type, const char *text) {
        GLuint shad = glCreateShader(type);
        GLint Result = GL_FALSE;
        int InfoLogLength;

        LOG_GL_ERRORV(glShaderSource(shad, 1, &text , nullptr));
        LOG_GL_ERRORV(glCompileShader(shad));
        
        glGetShaderiv(shad, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (!Result && InfoLogLength > 0 ) {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(shad, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            wxString l = &VertexShaderErrorMessage[0];
            l.Trim();
            if (l.length() > 0) {
                printf("Shader Log(2.1): %s\n", &VertexShaderErrorMessage[0]);
                static log4cpp::Category &logger_opengl = log4cpp::Category::getInstance(std::string("log_opengl"));
                logger_opengl.error(std::string(&VertexShaderErrorMessage[0]));
            }
        }
        return shad;
    }
    GLuint LinkProgram(GLuint VertexShaderID, GLuint FragmentShaderID) {
        GLint Result = GL_FALSE;
        int InfoLogLength;
        
        GLuint ProgramID = glCreateProgram();
        
        LOG_GL_ERRORV(glAttachShader(ProgramID, VertexShaderID));
        LOG_GL_ERRORV(glAttachShader(ProgramID, FragmentShaderID));
        
        LOG_GL_ERRORV(glLinkProgram(ProgramID));
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (!Result && InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            wxString l = &ProgramErrorMessage[0];
            l.Trim();
            if (l.length() > 0) {
                printf("Program Log(2.1): %s\n", &ProgramErrorMessage[0]);
                static log4cpp::Category &logger_opengl = log4cpp::Category::getInstance(std::string("log_opengl"));
                logger_opengl.error(std::string(&ProgramErrorMessage[0]));
            }
        }
        LOG_GL_ERRORV(glDetachShader(ProgramID, VertexShaderID));
        LOG_GL_ERRORV(glDetachShader(ProgramID, FragmentShaderID));
        return ProgramID;
    }
    
    ~OpenGL21Cache() {
        if (matrix) {
            delete matrix;
        }
        while (!matrixStack.empty()) {
            delete matrixStack.top();
            matrixStack.pop();
        }
        if (ProgramIDcolors != 0) {
            LOG_GL_ERRORV(glDeleteProgram(ProgramIDcolors));
        }
        if (ProgramIDstaticColor != 0) {
            LOG_GL_ERRORV(glDeleteProgram(ProgramIDstaticColor));
        }
        if (ProgramIDtexture != 0) {
            LOG_GL_ERRORV(glDeleteProgram(ProgramIDtexture));
        }
    }

    void Draw(DrawGLUtils::xlVertexAccumulator &va, const xlColor & color, int type, int enableCapability) override {
        if (va.count == 0) {
            return;
        }
        LOG_GL_ERRORV(glUseProgram(ProgramIDstaticColor));
        SetMVP(ProgramIDstaticColor);

        GLuint vattrib = glGetAttribLocation( ProgramIDstaticColor, "vertexPosition_modelspace" );
        LOG_GL_ERRORV(glEnableVertexAttribArray(vattrib));
        LOG_GL_ERRORV(glVertexAttribPointer(vattrib, 2, GL_FLOAT, false, 0, &va.vertices[0]));
        GLuint cid = glGetUniformLocation(ProgramIDstaticColor, "color");
        glUniform4f(cid,
                    ((float)color.Red())/255.0,
                    ((float)color.Green())/255.0,
                    ((float)color.Blue())/255.0,
                    ((float)color.Alpha())/255.0
                    );
        
        if (enableCapability != 0) {
            glEnable(enableCapability);
        }
        LOG_GL_ERRORV(glDrawArrays(type, 0, va.count));
        if (enableCapability > 0) {
            glDisable(enableCapability);
        }
        LOG_GL_ERRORV(glDisableVertexAttribArray(vattrib));
    }
    void CalcSmoothPointParams(float ps) {
        glPointSize(ps+1);
        float delta = 1.0 / (ps+1);
        float mid = 0.35 + 0.15 * ((ps - 1.0f)/25.0f);
        if (mid > 0.5) {
            mid = 0.5;
        }
        
        float min = std::max(0.0f, mid - delta);
        float max = std::min(1.0f, mid + delta);
        glUniform1f(glGetUniformLocation(ProgramIDcolors, "PointSmoothMin"), min);
        glUniform1f(glGetUniformLocation(ProgramIDcolors, "PointSmoothMax"), max);
    }
    float CalcSmoothPointParams() {
        float ps;
        glGetFloatv(GL_POINT_SIZE, &ps);
        CalcSmoothPointParams(ps);
        return ps;
    }
    
    
    void Draw(DrawGLUtils::xlAccumulator &va) override {
        if (va.count == 0) {
            return;
        }
        bool intelMapped = false;
        LOG_GL_ERRORV(glUseProgram(ProgramIDcolors));
        SetMVP(ProgramIDcolors);
        
        GLuint vattrib = glGetAttribLocation( ProgramIDcolors, "vertexPosition_modelspace" );
        LOG_GL_ERRORV(glEnableVertexAttribArray(vattrib));
        LOG_GL_ERRORV(glVertexAttribPointer(vattrib, 2, GL_FLOAT, false, 0, &va.vertices[0]));
        
        GLuint cattrib = glGetAttribLocation( ProgramIDcolors, "vertexColor" );
        LOG_GL_ERRORV(glEnableVertexAttribArray(cattrib));
        LOG_GL_ERRORV(glVertexAttribPointer(cattrib, 4, GL_UNSIGNED_BYTE, true, 0, &va.colors[0]));

        bool textBound = false;
        GLuint tattrib = 0;
        GLuint tvattrib = 0;

        for (auto it = va.types.begin(); it != va.types.end(); it++) {
            if (it->type == GL_POINTS) {
                LOG_GL_ERRORV(glPointSize(it->extra));
            } else if (it->type == GL_LINES || it->type == GL_LINE_LOOP || it->type == GL_LINE_STRIP) {
                DrawGLUtils::SetLineWidth(it->extra);
            }
            if (isIntel && it->enableCapability == GL_POINT_SMOOTH) {
                if (it->enableCapability != 0) {
                    glEnable(it->enableCapability);
                }
                LOG_GL_ERRORV(glDisableVertexAttribArray(cattrib));
                LOG_GL_ERRORV(glDisableVertexAttribArray(vattrib));

                LOG_GL_ERRORV(glEnableClientState(GL_VERTEX_ARRAY));
                LOG_GL_ERRORV(glEnableClientState(GL_COLOR_ARRAY));
                if (!intelMapped) {
                    intelMapped = true;
                    LOG_GL_ERRORV(glColorPointer(4, GL_UNSIGNED_BYTE, 0, &va.colors[0]));
                    LOG_GL_ERRORV(glVertexPointer(2, GL_FLOAT, 0, &va.vertices[0]));
                }
                LOG_GL_ERRORV(glUseProgram(0));
                LOG_GL_ERRORV(glPushMatrix());
                LOG_GL_ERRORV(glLoadMatrixf(glm::value_ptr(*matrix)));
                LOG_GL_ERRORV(glDrawArrays(it->type, it->start, it->count));
                LOG_GL_ERRORV(glPopMatrix());
                LOG_GL_ERRORV(glUseProgram(ProgramIDcolors));
                LOG_GL_ERRORV(glDisableClientState(GL_VERTEX_ARRAY));
                LOG_GL_ERRORV(glDisableClientState(GL_COLOR_ARRAY));

                if (it->enableCapability != 0) {
                    glDisable(it->enableCapability);
                }
                LOG_GL_ERRORV(glEnableVertexAttribArray(cattrib));
                LOG_GL_ERRORV(glEnableVertexAttribArray(vattrib));
            } else {
                int enableCapability = it->enableCapability;
                
                float ps = 2.0;
                if (it->textureId != -1) {
                    LOG_GL_ERRORV(glDisableVertexAttribArray(vattrib));
                    LOG_GL_ERRORV(glDisableVertexAttribArray(cattrib));
                    LOG_GL_ERRORV(glUseProgram(ProgramIDtexture));
                    if (!textBound) {
                        SetMVP(ProgramIDtexture);
                        tattrib = glGetAttribLocation( ProgramIDtexture, "texturePos" );
                        tvattrib = glGetAttribLocation( ProgramIDtexture, "vertexPosition_modelspace" );
                        LOG_GL_ERRORV(glUniform1i(glGetUniformLocation(ProgramIDtexture, "tex"), 0));
                    }
                    LOG_GL_ERRORV(glEnableVertexAttribArray(tvattrib));
                    if (vattrib != tvattrib) {
                        LOG_GL_ERRORV(glVertexAttribPointer(tvattrib, 2, GL_FLOAT, false, 0, va.vertices));
                    }
                    LOG_GL_ERRORV(glEnableVertexAttribArray(tattrib));
                    if (tattrib == cattrib || tattrib == vattrib) {
                        LOG_GL_ERRORV(glVertexAttribPointer(tattrib, 2, GL_FLOAT, true, 0, va.tvertices));
                    }
                    GLuint cid = glGetUniformLocation(ProgramIDtexture, "inColor");
                    glUniform4f(cid, 1.0, 1.0, 1.0, ((float)it->textureAlpha)/255.0);
                    
                    LOG_GL_ERRORV(glActiveTexture(GL_TEXTURE0)); //switch to texture image unit 0
                    LOG_GL_ERRORV(glBindTexture(GL_TEXTURE_2D, it->textureId));
                }
                if (enableCapability != 0) {
                    if (enableCapability == GL_POINT_SMOOTH) {
                        //LOG_GL_ERRORV(glEnable(enableCapability));
                        GLuint cid = glGetUniformLocation(ProgramIDcolors, "RenderType");
                        glUniform1i(cid, 1);
                        ps = CalcSmoothPointParams();
                        LOG_GL_ERRORV(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));
                        LOG_GL_ERRORV(glEnable(GL_POINT_SPRITE));
                        LOG_GL_ERRORV(glTexEnvi(GL_POINT_SPRITE_ARB,GL_COORD_REPLACE_ARB ,GL_FALSE));
                    } else {
                        LOG_GL_ERRORV(glEnable(enableCapability));
                    }
                }
                LOG_GL_ERRORV(glDrawArrays(it->type, it->start, it->count));
                if (it->textureId != -1) {
                    LOG_GL_ERRORV(glBindTexture(GL_TEXTURE_2D, 0));
                    LOG_GL_ERRORV(glDisableVertexAttribArray(tattrib));
                    LOG_GL_ERRORV(glDisableVertexAttribArray(tvattrib));
                    LOG_GL_ERRORV(glUseProgram(ProgramIDcolors));
                    LOG_GL_ERRORV(glEnableVertexAttribArray(vattrib));
                    if (tvattrib != vattrib) {
                        LOG_GL_ERRORV(glVertexAttribPointer(vattrib, 2, GL_FLOAT, false, 0, &va.vertices[0]));
                    }
                    LOG_GL_ERRORV(glEnableVertexAttribArray(cattrib));
                    if (tattrib == cattrib || tattrib == vattrib) {
                        LOG_GL_ERRORV(glVertexAttribPointer(cattrib, 4, GL_UNSIGNED_BYTE, true, 0, &va.colors[0]));
                    }
                }
                if (enableCapability > 0) {
                    if (enableCapability == GL_POINT_SMOOTH || enableCapability == GL_POINT_SPRITE) {
                        GLuint cid = glGetUniformLocation(ProgramIDcolors, "RenderType");
                        glUniform1i(cid, 0);
                        LOG_GL_ERRORV(glPointSize(ps));
                        LOG_GL_ERRORV(glDisable(GL_POINT_SPRITE));
                        LOG_GL_ERRORV(glDisable(GL_VERTEX_PROGRAM_POINT_SIZE));
                    } else {
                        glDisable(enableCapability);
                    }
                }
            }
        }
        
        LOG_GL_ERRORV(glDisableVertexAttribArray(cattrib));
        LOG_GL_ERRORV(glDisableVertexAttribArray(vattrib));
    }

    
    
    void Draw(DrawGLUtils::xlVertexColorAccumulator &va, int type, int enableCapability) override {
        if (va.count == 0) {
            return;
        }
        if (isIntel && enableCapability == GL_POINT_SMOOTH) {
            LOG_GL_ERRORV(glUseProgram(0));
            LOG_GL_ERRORV(glPushMatrix());
            LOG_GL_ERRORV(glLoadMatrixf(glm::value_ptr(*matrix)));
            LOG_GL_ERRORV(glEnable(enableCapability));
            LOG_GL_ERRORV(glEnableClientState(GL_VERTEX_ARRAY));
            LOG_GL_ERRORV(glEnableClientState(GL_COLOR_ARRAY));
            
            LOG_GL_ERRORV(glColorPointer(4, GL_UNSIGNED_BYTE, 0, &va.colors[0]));
            LOG_GL_ERRORV(glVertexPointer(2, GL_FLOAT, 0, &va.vertices[0]));
            LOG_GL_ERRORV(glDrawArrays(type, 0, va.count));
            
            LOG_GL_ERRORV(glDisableClientState(GL_VERTEX_ARRAY));
            LOG_GL_ERRORV(glDisableClientState(GL_COLOR_ARRAY));
            LOG_GL_ERRORV(glPopMatrix());
            LOG_GL_ERRORV(glDisable(enableCapability));
            return;
        }
        LOG_GL_ERRORV(glUseProgram(ProgramIDcolors));
        SetMVP(ProgramIDcolors);
        
        GLuint vattrib = glGetAttribLocation( ProgramIDcolors, "vertexPosition_modelspace" );
        LOG_GL_ERRORV(glEnableVertexAttribArray(vattrib));
        LOG_GL_ERRORV(glVertexAttribPointer(vattrib, 2, GL_FLOAT, false, 0, &va.vertices[0]));

        GLuint cattrib = glGetAttribLocation( ProgramIDcolors, "vertexColor" );
        LOG_GL_ERRORV(glEnableVertexAttribArray(cattrib));
        LOG_GL_ERRORV(glVertexAttribPointer(cattrib, 4, GL_UNSIGNED_BYTE, true, 0, &va.colors[0]));
        
        float ps = 2.0;
        if (enableCapability != 0) {
            if (enableCapability == GL_POINT_SMOOTH) {
                //LOG_GL_ERRORV(glEnable(enableCapability));
                GLuint cid = glGetUniformLocation(ProgramIDcolors, "RenderType");
                glUniform1i(cid, 1);
                ps = CalcSmoothPointParams();
                LOG_GL_ERRORV(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));
                LOG_GL_ERRORV(glEnable(GL_POINT_SPRITE));
                LOG_GL_ERRORV(glTexEnvi(GL_POINT_SPRITE_ARB,GL_COORD_REPLACE_ARB ,GL_FALSE));
            } else {
                LOG_GL_ERRORV(glEnable(enableCapability));
            }
        }
        LOG_GL_ERRORV(glDrawArrays(type, 0, va.count));
        if (enableCapability > 0) {
            if (enableCapability == GL_POINT_SMOOTH || enableCapability == GL_POINT_SPRITE) {
                GLuint cid = glGetUniformLocation(ProgramIDcolors, "RenderType");
                glUniform1i(cid, 0);
                LOG_GL_ERRORV(glPointSize(ps));
                LOG_GL_ERRORV(glDisable(GL_POINT_SPRITE));
                LOG_GL_ERRORV(glDisable(GL_VERTEX_PROGRAM_POINT_SIZE));
            } else {
                glDisable(enableCapability);
            }
        }
        LOG_GL_ERRORV(glDisableVertexAttribArray(cattrib));
        LOG_GL_ERRORV(glDisableVertexAttribArray(vattrib));
    }
    void Draw(DrawGLUtils::xlVertexTextureAccumulator &va, int type, int enableCapability) override {
        if (va.count == 0) {
            return;
        }
        LOG_GL_ERRORV(glUseProgram(ProgramIDtexture));
        SetMVP(ProgramIDtexture);
        
        GLuint vattrib = glGetAttribLocation( ProgramIDtexture, "vertexPosition_modelspace" );
        LOG_GL_ERRORV(glEnableVertexAttribArray(vattrib));
        LOG_GL_ERRORV(glVertexAttribPointer(vattrib, 2, GL_FLOAT, false, 0, va.vertices));
        
        GLuint cattrib = glGetAttribLocation( ProgramIDtexture, "texturePos" );
        LOG_GL_ERRORV(glEnableVertexAttribArray(cattrib));
        LOG_GL_ERRORV(glVertexAttribPointer(cattrib, 2, GL_FLOAT, true, 0, va.tvertices));
        
        LOG_GL_ERRORV(glUniform1i(glGetUniformLocation(ProgramIDtexture, "tex"), 0));

        GLuint cid = glGetUniformLocation(ProgramIDtexture, "inColor");
        glUniform4f(cid, 1.0, 1.0, 1.0, ((float)va.alpha)/255.0);

        
        LOG_GL_ERRORV(glActiveTexture(GL_TEXTURE0)); //switch to texture image unit 0
        LOG_GL_ERRORV(glBindTexture(GL_TEXTURE_2D, va.id));
        
        if (enableCapability != 0) {
            glEnable(enableCapability);
        }
        LOG_GL_ERRORV(glDrawArrays(type, 0, va.count));
        if (enableCapability > 0) {
            glDisable(enableCapability);
        }
        LOG_GL_ERRORV(glBindTexture(GL_TEXTURE_2D, 0));
        LOG_GL_ERRORV(glDisableVertexAttribArray(cattrib));
        LOG_GL_ERRORV(glDisableVertexAttribArray(vattrib));
    }

    
    virtual void addVertex(float x, float y, const xlColor &c) override {
        data.PreAlloc(1);
        data.AddVertex(x, y, c);
    }
    virtual void ensureSize(unsigned int s) override {
        data.PreAlloc(s);
    }
    virtual unsigned int vertexCount() override {
        return data.count;
    }

    void flush(int type, int enableCapability) override {
        Draw(data, type, enableCapability);
        data.Reset();
    }

    void DrawTexture(GLuint texture, float x, float y, float x2, float y2,
                     float tx, float ty, float tx2, float ty2) override {
        
        DrawGLUtils::xlVertexTextureAccumulator va(texture);
        va.PreAlloc(6);
        
        va.AddVertex(x - 0.4, y, tx, ty);
        va.AddVertex(x - 0.4, y2, tx, ty2);
        va.AddVertex(x2 - 0.4, y2, tx2, ty2);
        va.AddVertex(x2 - 0.4, y2, tx2, ty2);
        va.AddVertex(x2 - 0.4, y, tx2, ty);
        va.AddVertex(x - 0.4, y, tx, ty);
        
        Draw(va, GL_TRIANGLES, 0);
    }

    void SetMVP(GLuint programId) {
        glUniformMatrix4fv(glGetUniformLocation(programId, "MVP"), 1, GL_FALSE, glm::value_ptr(*matrix));
    }
    void Ortho(int topleft_x, int topleft_y, int bottomright_x, int bottomright_y) override {
        if (matrix) {
            delete matrix;
        }
        glm::mat4 m = glm::ortho((float)topleft_x, (float)bottomright_x, (float)bottomright_y, (float)topleft_y);
        matrix = new glm::mat4(m);
    }
    void PushMatrix() override {
        matrixStack.push(matrix);
        matrix = new glm::mat4(*matrix);
    }
    void PopMatrix() override {
        if (!matrixStack.empty()) {
            if (matrix != nullptr) {
                delete matrix;
            }
            matrix = matrixStack.top();
            matrixStack.pop();
        }
    }
    void Translate(float x, float y, float z) override {
        if (matrix) {
            glm::mat4 tm = glm::translate(*matrix, glm::vec3(x, y, z));
            delete matrix;
            matrix = new glm::mat4(tm);
        } else {
            glm::mat4 tm = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
            matrix = new glm::mat4(tm);
        }
    }
    void Rotate(float angle, float x, float y, float z) override {
        if (matrix) {
            angle = wxDegToRad(angle);
            glm::mat4 tm = glm::rotate(*matrix, angle, glm::vec3(x, y, z));
            delete matrix;
            matrix = new glm::mat4(tm);
        } else {
            angle = wxDegToRad(angle);
            glm::mat4 tm = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(x, y, z));
            matrix = new glm::mat4(tm);
        }
    }
    void Scale(float w, float h, float z) override {
        if (matrix) {
            glm::mat4 tm = glm::scale(*matrix, glm::vec3(w, h, z));
            delete matrix;
            matrix = new glm::mat4(tm);
        } else {
            glm::mat4 tm = glm::scale(glm::mat4(1.0f), glm::vec3(w, h, z));
            matrix = new glm::mat4(tm);
        }
    }


protected:
    DrawGLUtils::xlVertexColorAccumulator data;

    GLuint ProgramIDcolors;
    GLuint ProgramIDstaticColor;
    GLuint ProgramIDtexture;
    
    GLuint MatrixIDc;
    GLuint MatrixIDsc;
    GLuint MatrixIDt;

    std::stack<glm::mat4*> matrixStack;
    
    bool isIntel;

    glm::mat4 *matrix;
};



DrawGLUtils::xlGLCacheInfo *Create21Cache() {
    return new OpenGL21Cache();
}
