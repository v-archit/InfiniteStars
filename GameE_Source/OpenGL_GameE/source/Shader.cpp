#include"Shader.h"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include"Renderer.h"


ShaderProgramSource Shader::ParseShader(const std::string& filepath)             //function to decode the shader file
{
    std::ifstream stream(filepath);         //open file

    enum class ShaderType                //define shader types
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];                  //take 2 string streams 
    ShaderType type = ShaderType::NONE;         //default

    while (getline(stream, line))       //get the data line by line in string format
    {
        if (line.find("#shader") != std::string::npos)        //npos-> invalid string position
        {
            if (line.find("vertex") != std::string::npos)    //set the type acc to the shader
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';    //add line to the read string stream
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, std::string& source)   //compile shader program
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();    //pointer to the beginning
    glShaderSource(id, 1, &src, nullptr);      //specifies the source of the shader i.e id
    glCompileShader(id);      //compile the shader

    int result;       //result for error handling
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);   //query to get the result of compiling i-> integer v->vector
    if (result == GL_FALSE)      //if result is 0
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);    //get the log length   
        char* message = (char*)alloca(length * sizeof(char));    //allocate bytes for char message 
        glGetShaderInfoLog(id, length, &length, message);     //convert to char message
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader " << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);      //delete the uncompiled shader
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(std::string& vertexShader, std::string& fragmentShader)           //create and compile shader program using shader source code (strings)
{
    unsigned int program = glCreateProgram();             //create shader program
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);    //create vertex shader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);     //create fragment shader

    glAttachShader(program, vs);     //attach shaders to the program
    glAttachShader(program, fs);
    glLinkProgram(program);          //link the program
    glValidateProgram(program);      //validate the program and get the status

    glDeleteShader(vs);     //delete intermediate shaders after attaching
    glDeleteShader(fs);

    return program;
}

Shader::Shader(const std::string& filepath)
	:m_Filepath(filepath), m_RendererID(0)
{
    //shader program

    ShaderProgramSource source = ParseShader(filepath);      //for inside visual studio compilation
    //ShaderProgramSource source = ParseShader("ColorShader.shader");                         //for exe compilation
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);                //bind the shader
}

void Shader::Unbind() const
{
    glUseProgram(0);                //bind the shader
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& color)
{
    glUniform4f(GetUniformLocation(name), color.x, color.y, color.z, color.w);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const std::string& name)                   //retrieve the location of color uniform
{

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())       //if uniform location already exists
    {
        return m_UniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "Warning: Uniform " << name << " doesn't exist" << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location;
}