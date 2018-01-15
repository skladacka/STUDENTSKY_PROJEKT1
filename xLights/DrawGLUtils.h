
#ifndef __XL_DRAWGLUTILS
#define __XL_DRAWGLUTILS

#include <mutex>
#include <list>
#include <vector>
#include <map>
#include "wx/glcanvas.h"
#include "Color.h"

class xlGLCanvas;

namespace DrawGLUtils
{
    #define LOG_GL_ERROR() DrawGLUtils::LogGLError(__FILE__, __LINE__)
    #define LOG_GL_ERRORV(a) a; DrawGLUtils::LogGLError(__FILE__, __LINE__, #a)
    #define IGNORE_GL_ERRORV(a) a; glGetError()
    
    bool LoadGLFunctions();
    
    class xlVertexAccumulatorBase {
    public:
        virtual void Reset() {count = 0;}
        void PreAlloc(unsigned int i) {
            if ((count + i) > max) {
                DoRealloc(count + i);
                max = count + i;
            }
        };
        
        float *vertices;
        unsigned int count;
        unsigned int max;
        
    protected:
        virtual void DoRealloc(int newMax) {
            vertices = (float*)realloc(vertices, sizeof(float)*newMax*2);
        }
        xlVertexAccumulatorBase() : count(0), max(64) {
            vertices = (float*)malloc(sizeof(float)*max*2);
        }
        xlVertexAccumulatorBase(unsigned int m) : count(0), max(m) {
            vertices = (float*)malloc(sizeof(float)*max*2);
        }

        xlVertexAccumulatorBase(const xlVertexAccumulatorBase &mv) {
            count = mv.count;
            max = mv.max;
            vertices = (float*)malloc(sizeof(float)*max*2);
            memcpy(vertices, mv.vertices, count * sizeof(float) * 2);
        }
        xlVertexAccumulatorBase(xlVertexAccumulatorBase &&mv) {
            vertices = mv.vertices;
            mv.vertices = nullptr;
            count = mv.count;
            max = mv.max;
        }
        virtual ~xlVertexAccumulatorBase() {
            if (vertices != nullptr) free(vertices);
        }

    };
    class xlVertexAccumulator : public xlVertexAccumulatorBase {
    public:
        xlVertexAccumulator() : xlVertexAccumulatorBase() {}
        
        void AddVertex(float x, float y) {
            PreAlloc(1);
            int i = count*2;
            vertices[i++] = x;
            vertices[i] = y;
            count++;
        }
        void AddLinesRect(float x1, float y1, float x2, float y2) {
            PreAlloc(8);
            AddVertex(x1, y1);
            AddVertex(x1, y2);
            AddVertex(x1, y2);
            AddVertex(x2, y2);
            AddVertex(x2, y2);
            AddVertex(x2, y1);
            AddVertex(x2, y1);
            AddVertex(x1, y1);
        }
        void AddRect(float x1, float y1, float x2, float y2) {
            PreAlloc(6);
            AddVertex(x1, y1);
            AddVertex(x1, y2);
            AddVertex(x2, y2);
            AddVertex(x2, y2);
            AddVertex(x2, y1);
            AddVertex(x1, y1);
        }
    };
    class xlVertexColorAccumulator : public xlVertexAccumulatorBase {
    public:
        xlVertexColorAccumulator() : xlVertexAccumulatorBase() {
            colors = (uint8_t*)malloc(max*4);
        }
        xlVertexColorAccumulator(unsigned int m) : xlVertexAccumulatorBase(m) {
            colors = (uint8_t*)malloc(max*4);
        }
        xlVertexColorAccumulator(xlVertexColorAccumulator &&mv) : xlVertexAccumulatorBase(mv) {
            colors = mv.colors;
            mv.colors = nullptr;
        }
        xlVertexColorAccumulator(const xlVertexColorAccumulator &mv) : xlVertexAccumulatorBase(mv) {
            colors = (uint8_t*)malloc(max*4);
            memcpy(colors, mv.colors, count * 4);
        }

        virtual ~xlVertexColorAccumulator() {
            free(colors);
        }
    
        void AddVertex(float x, float y, const xlColor &c) {
            PreAlloc(1);
            int i = count*2;
            vertices[i++] = x;
            vertices[i] = y;
            i = count*4;
            colors[i++] = c.Red();
            colors[i++] = c.Green();
            colors[i++] = c.Blue();
            colors[i] = c.Alpha();
            count++;
        }
        void AddRect(float x1, float y1, float x2, float y2, const xlColor &c) {
            PreAlloc(6);
            AddVertex(x1, y1, c);
            AddVertex(x1, y2, c);
            AddVertex(x2, y2, c);
            AddVertex(x2, y2, c);
            AddVertex(x2, y1, c);
            AddVertex(x1, y1, c);
        }
        void AddLinesRect(float x1, float y1, float x2, float y2, const xlColor &c) {
            PreAlloc(8);
            AddVertex(x1, y1, c);
            AddVertex(x1, y2, c);
            AddVertex(x2, y2, c);
            AddVertex(x2, y1, c);
            
            AddVertex(x1, y2, c);
            AddVertex(x2, y2, c);
            AddVertex(x2, y1, c);
            AddVertex(x1, y1, c);
        }
        void AddDottedLinesRect(float x1, float y1, float x2, float y2, const xlColor &c);
        void AddHBlendedRectangle(const xlColorVector &colors, float x1, float y1,float x2, float y2, xlColor* colorMask, int offset = 0);
        void AddHBlendedRectangle(const xlColor &left, const xlColor &right, float x1, float y1, float x2, float y2);
        void AddTrianglesCircle(float x, float y, float radius, const xlColor &color);
        void AddTrianglesCircle(float x, float y, float radius, const xlColor &center, const xlColor &edge);
        
        uint8_t *colors;
    protected:
        virtual void DoRealloc(int newMax) {
            xlVertexAccumulatorBase::DoRealloc(newMax);
            colors = (uint8_t*)realloc(colors, newMax*4);
        }
    };
    
    class xlVertexTextureAccumulator : public xlVertexAccumulatorBase {
    public:
        xlVertexTextureAccumulator() : xlVertexAccumulatorBase(), id(0), alpha(255) {
            tvertices = (float*)malloc(sizeof(float)*max*2);
        }
        xlVertexTextureAccumulator(GLuint i) : xlVertexAccumulatorBase(), id(i), alpha(255) {
            tvertices = (float*)malloc(sizeof(float)*max*2);
        }
        xlVertexTextureAccumulator(GLuint i, uint8_t a) : xlVertexAccumulatorBase(), id(i), alpha(a) {
            tvertices = (float*)malloc(sizeof(float)*max*2);
        }
        xlVertexTextureAccumulator(xlVertexTextureAccumulator &&mv) : xlVertexAccumulatorBase(mv) {
            tvertices = mv.tvertices;
            mv.tvertices = nullptr;
            id = mv.id;
            alpha = mv.alpha;
        }
        xlVertexTextureAccumulator(const xlVertexTextureAccumulator &mv) : xlVertexAccumulatorBase(mv) {
            id = mv.id;
            alpha = mv.alpha;
            tvertices = (float*)malloc(sizeof(float)*max*2);
            memcpy(tvertices, mv.tvertices, count * sizeof(float) * 2);
        }

        virtual ~xlVertexTextureAccumulator() {
            free(tvertices);
        }
        
        void AddVertex(float x, float y, float tx, float ty) {
            PreAlloc(1);
            int i = count*2;
            vertices[i] = x;
            tvertices[i] = tx;
            i++;
            vertices[i] = y;
            tvertices[i] = ty;
            count++;
        }
        void AddFullTexture(float x, float y, float x2, float y2) {
            PreAlloc(6);
            AddVertex(x, y, 0, 0);
            AddVertex(x, y2, 0, 1);
            AddVertex(x2, y2, 1, 1);
            AddVertex(x2, y2, 1, 1);
            AddVertex(x2, y, 1, 0);
            AddVertex(x, y, 0, 0);
        }
        GLuint id;
        uint8_t alpha;
        float *tvertices;
    protected:
        virtual void DoRealloc(int newMax) {
            xlVertexAccumulatorBase::DoRealloc(newMax);
            tvertices = (float*)realloc(tvertices, sizeof(float)*newMax*2);
        }
    };
    class xlVertexTextAccumulator {
    public:
        xlVertexTextAccumulator() : count(0) {}
        ~xlVertexTextAccumulator() {}
        
        void PreAlloc(unsigned int i) {
            vertices.reserve(vertices.size() + i*2);
            text.reserve(text.size() + i);
        };
        void Reset() {count = 0; vertices.clear(); text.clear();}
        void AddVertex(float x, float y, const std::string &s) {
            vertices.push_back(x); vertices.push_back(y);
            text.push_back(s);
            count++;
        }
        std::vector<float> vertices;
        std::vector<std::string> text;
        unsigned int count;
    };
    
    class xlAccumulator : public xlVertexColorAccumulator {
    public:
        xlAccumulator() : xlVertexColorAccumulator(), tvertices(nullptr) { start = 0;}
        xlAccumulator(unsigned int max) : xlVertexColorAccumulator(max), tvertices(nullptr) { start = 0;}
        virtual ~xlAccumulator() {
            if (tvertices) {
                free(tvertices);
                tvertices = nullptr;
            }
        }
        virtual void Reset() override {
            xlVertexColorAccumulator::Reset();
            start = 0;
            types.clear();
            if (tvertices) {
                free(tvertices);
                tvertices = nullptr;
            }
        }
        
        virtual void DoRealloc(int newMax) override;

        bool HasMoreVertices() { return count != start; }
        void Finish(int type, int enableCapability = 0, float extra = 1);
        
        
        void PreAllocTexture(int i);
        void AddTextureVertex(float x, float y, float tx, float ty);
        void FinishTextures(int type, GLuint textureId, uint8_t alpha, int enableCapability = 0);
        
        void Load(const xlVertexColorAccumulator &ca);
        void Load(const xlVertexAccumulator &ca, const xlColor &c);
        void Load(const xlVertexTextureAccumulator &ca, int type, int enableCapability = 0);
        
        class BufferRangeType {
        public:
            BufferRangeType(int s, int c, int t, int ec, float ex) {
                start = s;
                count = c;
                type = t;
                enableCapability = ec;
                extra = ex;
                textureId = -1;
            }
            BufferRangeType(int s, int c, int t, int ec, GLuint tid, uint8_t alpha) {
                start = s;
                count = c;
                type = t;
                enableCapability = ec;
                extra = 0.0f;
                textureId = tid;
                textureAlpha = alpha;
            }
            int start;
            int count;
            int type;
            int enableCapability;
            float extra;
            GLuint textureId;
            uint8_t textureAlpha;
        };
        std::list<BufferRangeType> types;
        float *tvertices;
    private:
        int start;
    };

    class xlGLCacheInfo {
    public:
        xlGLCacheInfo();
        virtual ~xlGLCacheInfo();
        
        virtual bool IsCoreProfile() { return false;}
        virtual void SetCurrent();
        virtual void Draw(xlVertexAccumulator &va, const xlColor & color, int type, int enableCapability = 0) = 0;
        virtual void Draw(xlVertexColorAccumulator &va, int type, int enableCapability = 0) = 0;
        virtual void Draw(xlVertexTextureAccumulator &va, int type, int enableCapability = 0) = 0;
        virtual void Draw(xlAccumulator &va) = 0;
        
        virtual void addVertex(float x, float y, const xlColor &c) = 0;
        virtual unsigned int vertexCount() = 0;
        virtual void flush(int type, int enableCapability = 0) = 0;
        virtual void ensureSize(unsigned int i) = 0;
        
        virtual void Ortho(int topleft_x, int topleft_y, int bottomright_x, int bottomright_y) = 0;
        virtual void PushMatrix() = 0;
        virtual void PopMatrix() = 0;
        virtual void Translate(float x, float y, float z) = 0;
        virtual void Rotate(float angle, float x, float y, float z) = 0;
        virtual void Scale(float w, float h, float z) = 0;
        virtual void DrawTexture(GLuint texture,
                                 float x, float y, float x2, float y2,
                                 float tx = 0.0, float ty = 0.0, float tx2 = 1.0, float ty2 = 1.0) = 0;

        GLuint GetTextureId(int i);
        void SetTextureId(int i, GLuint id) {textures[i] = id;}
        bool HasTextureId(int i);
        void AddTextureToDelete(GLuint i) { deleteTextures.push_back(i); }
        protected:
            std::vector<GLuint> deleteTextures;
            std::map<int, GLuint> textures;
    };
    
    xlGLCacheInfo *CreateCache();
    void SetCurrentCache(xlGLCacheInfo *cache);
    void DestroyCache(xlGLCacheInfo *cache);
    
    void SetViewport(xlGLCanvas &win, int x1, int y1, int x2, int y2);
    void PushMatrix();
    void PopMatrix();
    void Translate(float x, float y, float z);
    void Rotate(float angle, float x, float y, float z);
    void Scale(float w, float h, float z);
    
    void LogGLError(const char *file, int line, const char *msg = nullptr);

    
    class DisplayListItem {
    public:
        DisplayListItem() : x(0.0), y(0.0) {};
        xlColor color;
        float x, y;
    };
    class xlDisplayList : public std::vector<DisplayListItem> {
    public:
        xlDisplayList() : iconSize(2) {};
        int iconSize;
        mutable std::recursive_mutex lock;
        
        void LockedClear() {
            std::unique_lock<std::recursive_mutex> locker(lock);
            clear();
        }
    };
   
    
    bool IsCoreProfile();
    int NextTextureIdx();
    
    void Draw(xlAccumulator &va);
    void Draw(xlVertexAccumulator &va, const xlColor & color, int type, int enableCapability = 0);
    void Draw(xlVertexColorAccumulator &va, int type, int enableCapability = 0);
    void Draw(xlVertexTextureAccumulator &va, int type, int enableCapability = 0);
    void Draw(xlVertexTextAccumulator &va, int size, float factor);

    
    void DrawText(double x, double y, double size, const wxString &text, double factor = 1.0);
    int GetTextWidth(double size, const wxString &text, double factor = 1.0);
    
    void SetLineWidth(float i);
    
    void DrawCircle(const xlColor &color, double x, double y, double r, int ctransparency = 0, int etransparency = 0);
    void DrawCircleUnfilled(const xlColor &color, double cx, double cy, double r, float width);

    /* Methods to hold vertex informaton (x, y, color) in an array until End is called where they are all
       draw out to the context in very few calls) */
    void AddVertex(double x, double y, const xlColor &c, int transparency = 0);
    int VertexCount();
    /* Add four vertices to the cache list, all with the given color */
    void PreAlloc(int verts);
    void AddRect(double x1, double y1,
                 double x2, double y2,
                 const xlColor &c, int transparency = 0);
    void AddRectAsTriangles(double x1, double y1,
                            double x2, double y2,
                            const xlColor &c, int transparency = 0);
    void End(int type, int enableCapability = 0);

    void DrawLine(const xlColor &color, wxByte alpha,int x1, int y1,int x2, int y2,float width);
    void DrawRectangle(const xlColor &color, bool dashed, int x1, int y1,int x2, int y2);
    void DrawFillRectangle(const xlColor &color, wxByte alpha, int x, int y,int width, int height);

    void CreateOrUpdateTexture(const wxBitmap &bmp48,    //will scale to 64x64 for base
                               const wxBitmap &bmp32,
                               const wxBitmap &bmp16,
                               GLuint *texture);
    void DrawTexture(GLuint texture,
                     float x, float y, float x2, float y2,
                     float tx = 0.0, float ty = 0.0, float tx2 = 1.0, float ty2 = 1.0);
    
    void UpdateTexturePixel(GLuint texture,double x, double y, xlColor& color, bool hasAlpha);


    void DrawDisplayList(float xOffset, float yOffset,
                         float width, float height,
                         const xlDisplayList & dl,
                         xlVertexColorAccumulator &bg);
}

#endif
