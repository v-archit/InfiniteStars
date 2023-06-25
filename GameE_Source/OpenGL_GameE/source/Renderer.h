#pragma once

#include<GL/glew.h>
#include"VertexArray.h"
#include"IndexBuffer.h"
#include"Shader.h"
#include"Camera.h"
#include<memory>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))    


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void SetClearColor(const glm::vec4& color);
    void Draw(const VertexArray& va, const IndexBuffer& ib) const;
    void Submit(const std::shared_ptr<Shader>& shader, const VertexArray& vertArr, const glm::mat4& transform = glm::mat4(1.0f)) const;
    static void BeginScene(Camera& camera);
    static void EndScene();

private:
    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* m_SceneData;
};