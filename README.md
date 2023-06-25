# InfiniteStars
Basic infinite stars dodging game made in OpenGL using ImGUI, GLFW, GLM, and GLEW.

-------------------------------------------------------------------------------------------
INSTRUCTIONS

Press Spacebar to give vertical velocity

If the Settings window disappears, delete the imgui file in the same directory

-------------------------------------------------------------------------------------------

Executable showing an Infinite Stars game where player has to dodge obstacles generated infinitely
Player can opt for a square or a triangle, chosen through ImGui button.
Camera follows the player.
Player velocity and gravity are simulated. (gravity only on y axis)
Player can change the color of their avatar, obstacles, and background.
Player also can set the horizontal velocity.
Collision detection is implemented using PointInTri function that checks for given vertex in a given triangle.
Camera position, rotation and translation implemented.
Timestep implemented to make translation independent of fps.
Vertex Arrays providing Vertex Buffer with VertexAttrib layout is used along with Index Buffer.
Abstraction is done for quick viewing of code.
Colored square is projected on the 4:3 window using projection matrix
Colored square is translated using mvp matrix (model->transform, view->camera, projection->screen)

GLFW used for console application (draw window)
GLEW used for dynamic utilization of OpenGL api to draw triangles using custom shader.
GLM used as a maths library to provide mvp matrices
Imgui used for dynamic UI integration

