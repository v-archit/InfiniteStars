#include "Renderer.h"
#include<iostream>

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

void GLClearError()
{
    while (glGetError());
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] " << error << " " << function << " " << file << " : " << line << std::endl;
        return false;
    }
    return true;
}



void Renderer::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib) const
{
    //shader.Bind();     //bind shader buffer
    va.Bind();        //bind vertex array, also binds vertex buffer
    ib.Bind();        //bind index buffer

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));      //draw call  //draw triangle from buffer with default intel shader and also checks error using GLCall

}


void Renderer::Submit(const std::shared_ptr<Shader>& shader, const VertexArray& vertArray, const glm::mat4& transform) const
{
    shader->Bind();
    shader->SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);           //view projection matrix
    shader->SetUniformMat4f("u_Transform", transform);                                       //model matrix 
    vertArray.Bind();

}


void Renderer::BeginScene(Camera& camera)
{
    m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();   
}

void Renderer::EndScene()
{
}


