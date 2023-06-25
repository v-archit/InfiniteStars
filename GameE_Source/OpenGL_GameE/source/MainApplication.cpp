#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include"Renderer.h"

#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"VertexArray.h"
#include"VertexBufferLayout.h"
#include"Shader.h"

#include"Camera.h"
#include"Level.h"
#include"Random.h"

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

using namespace std;

enum class GameState
{   
    Play = 0, MainMenu = 1, GameOver = 2
};

GameState m_State = GameState::MainMenu;

Renderer renderer;
Level m_Level;
Player player;
ImFont* m_FontA;
ImFont* m_FontB;
float m_Time = 0.0f;
float xVelocity = 8.0f;
bool startSimulation = false;
bool playerShape = false;                      //false->square true->triangle
glm::vec4 playerColor = { 1.0f, 0.3f, 0.8f, 1.0f };
glm::vec4 backgroundColor = { 0.0f, 0.0f, 0.0f, 0.0f };
glm::vec4 obstacleColor = { 0.3f, 0.5f, 1.0f, 1.0f };

int main(void)
{
    GLFWwindow* window;


    if (!glfwInit())         //library initialization
        return -1;

    window = glfwCreateWindow(1920.0f, 1080.0f, "Infinite Stars", glfwGetPrimaryMonitor(), NULL);       //create a full screen window


    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);         //current windows context

    if (glewInit() != GLEW_OK)          //GLEW initialization
    {
        cout << "Error" << endl;
    }

    m_Level.Init();              //Initialize the level -> create obstacles
    player = m_Level.GetPlayer();       //get player reference

    cout << glGetString(GL_VERSION) << endl;


    //shader program

    std::shared_ptr<Shader> shader;                                      //declare shared pointer to shader
    shader.reset(new Shader("resource/shaders/ColorShader.shader"));           //with shader pointer
    //shader.reset(new Shader("ColorShader.shader"));           //with shader pointer  //exe



    //Camera camera(-160.0f, 160.0f, -90.0f, 90.0f);           //creates camera with vp matrix [320*180 camera with origin at center]
    Camera camera(-16.0f, 16.0f, -9.0f, 9.0f);           //creates camera with vp matrix [32*18 camera with origin at center]
   

    //Square geometry

    float positionsSquare[] = {           //square vertices
       -0.5f, -0.5f,    //0
       -0.5f, 0.5f,     //1
        0.5f, -0.5f,    //2
        0.5f, 0.5f,     //3
    };

    unsigned int indicesSquare[] = {         //use square vertice indices to draw 2 triangles
        0, 1, 2,
        2, 3, 1
    };

    VertexArray va_Square;
    VertexBuffer vb_Square(positionsSquare, 4 * 2 * sizeof(float));            // 4 vertex positions and 2 floats each

    VertexBufferLayout layout_Square;
    layout_Square.Push<float>(2);              //pass 2 as element count
    va_Square.AddBuffer(vb_Square, layout_Square);

    IndexBuffer ib_Square(indicesSquare, 6);            //6 indices

    //Triangle geometry

    float positionsTriangle[] = {           //triangle vertices
       -0.5f, -0.5f, //0
        0.5f, -0.5f, //1
        0.0f, 0.5f, //2
    };

    unsigned int indicesTriangle[] = {         //use triangle vertex indices to draw 1 triangle
        0, 1, 2,
    };

    VertexArray va_Triangle;
    VertexBuffer vb_Triangle(positionsTriangle, 3 * 2 * sizeof(float));            // 4 vertex positions and 2 floats each

    VertexBufferLayout layout_Triangle;
    layout_Triangle.Push<float>(2);              //pass 2 as element count
    va_Triangle.AddBuffer(vb_Triangle, layout_Triangle);

    IndexBuffer ib_Triangle(indicesTriangle, 3);            //6 indices


    //imgui attach
    IMGUI_CHECKVERSION();
    ImGuiContext* iContext;
    iContext = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();


    m_FontB = io.Fonts->AddFontDefault();                   //add default font
    m_FontA = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 120.0f);           //set the font of imgui

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       //enable keyboard navigation
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           //enable docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         //enable viewports in imgui

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);              //Init imgui
    ImGui_ImplOpenGL3_Init("#version 330");


    float m_LastFrame = 0.0f;                   //render time of last frame

    while (!glfwWindowShouldClose(window))           //render loop
    {
        //Timestep calculation
        float time = (float)glfwGetTime();
        float timeStep = time - m_LastFrame;
        m_LastFrame = time;


        //// Render

        renderer.SetClearColor({ 1.0f,1.0f,1.0f,1.0f });    //clear screen with color
        renderer.Clear();                              //renderer clear screen

        renderer.BeginScene(camera);           //set VP matrix


        m_Time += timeStep;


        if (m_Level.IsGameOver())
            m_State = GameState::GameOver;


        switch (m_State)
        {
            case GameState::Play:
            {
                m_Level.OnUpdate(timeStep);             //Update the level and player
                break;
            }
        }

        const auto& playerPos = m_Level.GetPlayer().GetPosition();
        const auto& obstacleArray = m_Level.GetObstacles();

        camera.SetPosition({ playerPos.x, playerPos.y, 0.0f });

        //imgui begin

        ImGui_ImplOpenGL3_NewFrame();              
        ImGui_ImplGlfw_NewFrame();              //new frame before imgui code
        ImGui::NewFrame();


        // Background
        glm::mat4 levelTransform = glm::translate(glm::mat4(1.0f), { playerPos.x, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), { 50.0f, 50.0f, 1.0f });        //translate the model (drawn vertices) to right and above  //transform

        renderer.Submit(shader, va_Square, levelTransform);
        shader->SetUniform4f("u_Color", backgroundColor);       //Set color uniform in shader
        renderer.Draw(va_Square, ib_Square);            //using the renderer, draw the bound vertex array, index buffer, and shader

        // Floor and Ceiling

        levelTransform = glm::translate(glm::mat4(1.0f), { playerPos.x,  34.0f, 0.0f }) * glm::scale(glm::mat4(1.0f), { 50.0f, 50.0f, 1.0f });        //translate the model (drawn vertices) to right and above  //transform

        renderer.Submit(shader, va_Square, levelTransform);
        shader->SetUniform4f("u_Color", obstacleColor);
        renderer.Draw(va_Square, ib_Square);            //using the renderer, draw the bound vertex array, index buffer, and shader

        levelTransform = glm::translate(glm::mat4(1.0f), { playerPos.x,  -34.0f, 0.0f }) * glm::scale(glm::mat4(1.0f), { 50.0f, 50.0f, 1.0f });        //translate the model (drawn vertices) to right and above  //transform

        renderer.Submit(shader, va_Square, levelTransform);
        renderer.Draw(va_Square, ib_Square);            //using the renderer, draw the bound vertex array, index buffer, and shader


        //Obstacles 

        for (auto& obstacle : obstacleArray)
        {
            levelTransform = glm::translate(glm::mat4(1.0f), obstacle.TopPosition) * glm::scale(glm::mat4(1.0f), { obstacle.TopScale.x, obstacle.TopScale.y, 1.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0f, 0.0f, 1.0f });        //translate the model (drawn vertices) to right and above  //transform  //model matrix

            renderer.Submit(shader, va_Triangle, levelTransform);
            shader->SetUniform4f("u_Color", obstacleColor);
            renderer.Draw(va_Triangle, ib_Triangle);            //using the renderer, draw the bound vertex array, index buffer, and shader

            levelTransform = glm::translate(glm::mat4(1.0f), obstacle.BottomPosition) * glm::scale(glm::mat4(1.0f), { obstacle.BottomScale.x, obstacle.BottomScale.y, 1.0f });        //translate the model (drawn vertices) to right and above  //transform

            renderer.Submit(shader, va_Triangle, levelTransform);
            renderer.Draw(va_Triangle, ib_Triangle);            //using the renderer, draw the bound vertex array, index buffer, and shader


        }


        //Player
        //playerColor = { 1.0f, 0.3f, 0.8f, 1.0f };
            //mvp
        if (!playerShape)          //square
        {
            glm::mat4 playerTransform = glm::translate(glm::mat4(1.0f), { playerPos.x, playerPos.y, 0.5f }) * glm::scale(glm::mat4(1.0f), { 1.5f, 1.5f, 1.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Level.GetPlayer().GetRotation()), { 0.0f, 0.0f, 1.0f });        //translate the model (drawn vertices) to right and above  //transform

            renderer.Submit(shader, va_Square, playerTransform);
            shader->SetUniform4f("u_Color", playerColor);
            renderer.Draw(va_Square, ib_Square);            //using the renderer, draw the bound vertex array, index buffer, and shader
        }
        else           //triangle
        {

            glm::mat4 playerTransform = glm::translate(glm::mat4(1.0f), { playerPos.x, playerPos.y, 0.5f }) * glm::scale(glm::mat4(1.0f), { 1.5f, 1.5f, 1.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Level.GetPlayer().GetRotation()), { 0.0f, 0.0f, 1.0f });        //translate the model (drawn vertices) to right and above  //transform

            renderer.Submit(shader, va_Triangle, playerTransform);
            shader->SetUniform4f("u_Color", playerColor);
            renderer.Draw(va_Triangle, ib_Triangle);            //using the renderer, draw the bound vertex array, index buffer, and shader
        }


        //ImGui Customization viewport

        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x, main_viewport->GetWorkPos().y), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 250), ImGuiCond_FirstUseEver);

        ImGui::Begin("Customization", (bool*)true, 0);

        ImGui::ColorEdit4("Background Color", glm::value_ptr(backgroundColor));
        ImGui::ColorEdit4("Player Color", glm::value_ptr(playerColor));
        ImGui::ColorEdit4("Obstacle Color", glm::value_ptr(obstacleColor));

        if (ImGui::SliderFloat("Player Speed", &xVelocity, 0.0f, 20.0f))
        {
            m_Level.GetPlayer().SetVelocityX(xVelocity);
        }

        if (ImGui::Button("Square Player"))
        {
            playerShape = false;
            m_Level.SetPlayerShape(false);
        }
        if (ImGui::Button("Triangle Player"))
        {
            playerShape = true;
            m_Level.SetPlayerShape(true);
        }

        if (ImGui::Button("Start"))
        {
            startSimulation = true;
            if (m_State == GameState::GameOver)
                m_Level.Reset();
            m_State = GameState::Play;
        }

        if (ImGui::Button("Exit"))
        {
            ImGui::End();
            glfwTerminate();
            return 1;
        }


        ImGui::End();                 //viewport ends


        //Game state manager

        switch (m_State)
        {
        case GameState::Play:
        {
            uint32_t playerScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_FontA, 48.0f, { ImGui::GetWindowPos().x + 400, ImGui::GetWindowPos().y }, 0xffffffff, scoreStr.c_str());      //draw text using imgui
            break;
        }
        case GameState::MainMenu:
        {
            auto pos = ImGui::GetWindowPos();
            auto width = 1280.0f;
            auto height = 720.0f;
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;


            ImGui::GetForegroundDrawList()->AddText(m_FontA, 120.0f, pos, 0xffffffff, "Click Start to Play!");
            break;
        }
        case GameState::GameOver:
        {
            auto pos = ImGui::GetWindowPos();
            auto width = 1280.0f;
            auto height = 720.0f;
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;

            ImGui::GetForegroundDrawList()->AddText(m_FontA, 120.0f, pos, 0xffffffff, "Click Start to Play!");

            pos.x += 200.0f;
            pos.y += 150.0f;
            uint32_t playerScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_FontA, 48.0f, pos, 0xffffffff, scoreStr.c_str());

            startSimulation = false;
            break;
        }
        }


        //imgui render


        ImGui::Render();                           //render imgui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());        //draw imgui data


        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)             //handler for viewports docking
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }


        glfwSwapBuffers(window);            //vsync
        glfwPollEvents();           //events polling


    }


    //imgui detach
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    glfwTerminate();        //terminate opengl
    return 0;
}

