#shader vertex
#version 330 core

layout(location = 0) in vec4 position;        //get the first index value from vertex buffer into position

uniform mat4 u_ViewProjection;           //view projection matrix 
uniform mat4 u_Transform;           //model matrix 
//uniform mat4 u_mvp;            //model view projection matrix

void main()
{
   gl_Position = u_ViewProjection * u_Transform * position;       //vertex positions (attribute)         //projects the positions into 4:3 ortho matric
};

#shader fragment 
#version 330 core

uniform vec4 u_Color;

layout(location = 0) out vec4 color;       //output the color

void main()
{
	color = u_Color;          //variable color
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};