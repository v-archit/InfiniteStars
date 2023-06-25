#pragma once

#include<string>
#include<unordered_map>
#include"glm/glm.hpp"

struct ShaderProgramSource             //struct for multiple return objects
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_Filepath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, std::string& source);
	unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader);

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, const glm::vec4& color);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	int GetUniformLocation(const std::string& name);
};
