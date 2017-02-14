#include <bits/stdc++.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
};
typedef struct VAO VAO;

struct COLOR {
    float r;
    float g;
    float b;
};
typedef struct COLOR COLOR;

    COLOR grey = {165.0/255.0,165.0/255.0,165.0/255.0};
    COLOR dimgrey = {105.0/255.0,105.0/255.0,105.0/255.0};
    COLOR gold = {218.0/255.0,165.0/255.0,32.0/255.0};
    COLOR coingold = {255.0/255.0,223.0/255.0,0.0/255.0};
    COLOR red = {255.0/255.0,51.0/255.0,51.0/255.0};
    COLOR lightgreen = {57/255.0,230/255.0,0/255.0};
    COLOR darkgreen = {51/255.0,102/255.0,0/255.0};
    COLOR black = {30/255.0,30/255.0,21/255.0};
    COLOR blue = {0,0,1};
    COLOR darkbrown = {46/255.0,46/255.0,31/255.0};
    COLOR lightbrown = {95/255.0,63/255.0,32/255.0};
    COLOR brown1 = {117/255.0,78/255.0,40/255.0};
    COLOR brown2 = {134/255.0,89/255.0,40/255.0};
    COLOR brown3 = {46/255.0,46/255.0,31/255.0};
    COLOR cratebrown = {153/255.0,102/255.0,0/255.0};
    COLOR cratebrown1 = {121/255.0,85/255.0,0/255.0};
    COLOR cratebrown2 = {102/255.0,68/255.0,0/255.0};
    COLOR slateblue = {106/255.0, 90/255.0, 205/255.0};
    COLOR aliceblue = {240/255.0,248/255.0,255/255.0};
    COLOR skyblue1 = {123/255.0,201/255.0,227/255.0};
    COLOR skyblue = {132/255.0,217/255.0,245/255.0};
    COLOR cloudwhite = {229/255.0,255/255.0,255/255.0};
    COLOR cloudwhite1 = {204/255.0,255/255.0,255/255.0};
    COLOR lightpink = {255/255.0,122/255.0,173/255.0};
    COLOR darkpink = {255/255.0,51/255.0,119/255.0};
    COLOR white = {255/255.0,255/255.0,255/255.0};
    COLOR azure = {255/255.0,245/255.0,238/255.0};
    COLOR score = {117/255.0,78/255.0,40/255.0};
    COLOR chocolate = {210/255.0, 105/255.0, 30/255.0};
    COLOR indianred = {205/255.0, 92/255.0, 92/255.0};
    COLOR peach = {255/255.0, 218/255.0, 185/255.0};
    COLOR wheat = {244/255.0, 164/255.0, 96/255.0};

struct Game {
    string name;
    COLOR color;
    float x,y,z;
    VAO* object;
    float height,width,radius,depth;
    float angle_x;
    float angle_y;
    float angle_z;
    int status;
    float numx;
    float numy;
};
map <string, Game> tiles;
map <string, Game> background;
map <string, Game> cube;
map <string, Game> scoredisp;
map <string, Game> life;
map <string, Game> welcomeback;

int gamemap[10][10]={
    {1,1,1,1,25,0,1,1,0,0},
    {0,0,0,0,0,1,1,1,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,13,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},      // 0 means gap , 1 means normal tile , 2 means bridge , 3 mean lever
    {0,0,0,1,1,1,1,0,0,0},
    {0,0,0,0,0,1,1,0,0,0},
    {0,0,0,1,1,1,1,1,0,0},
    {0,0,1,1,1,1,0,0,0,0}
};
struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint MatrixID;
} Matrices;

int do_rot, floor_rel;
GLuint programID;
double last_update_time, current_time;
float rectangle_rotation = 0;



float rectangle_rot_dir = 1;
bool rectangle_rot_status = true;
int up1=0,down1=0,right1=0,left1=0;

int user_score = 0;
int user_life = 3;
int k_tile=0;

float eye_x_1,eye_y_1,eye_z_1;
float target_x_1,target_y_1,target_z_1;
int camera_follow=0;
int camera_follow_adjust_1=0;
int camera_top=0;
int camera_fps=0;
float camera_radius;
float camera_fov=1.3;
float fps_head_offset=0;
float fps_head_offset_x=0;
int camera_tower=1;
int camera_helicopter=0;
int camera_self=0;
int orient_right=0;
int orient_left=0;
int orient_forward=0;
int orient_backward=0;
int mouse_click=0,right_mouse_click=0;
int keyboard_press=0;
double mouse_pos_x, mouse_pos_y;
double prev_mouse_pos_x,prev_mouse_pos_y;



/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
	{
	    std::string Line = "";
	    while(getline(VertexShaderStream, Line))
		VertexShaderCode += "\n" + Line;
	    VertexShaderStream.close();
	}

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
	std::string Line = "";
	while(getline(FragmentShaderStream, Line))
	    FragmentShaderCode += "\n" + Line;
	FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    //    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    //    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    //    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    //    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    //    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    //    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO 
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
                          0,                  // attribute 0. Vertices
                          3,                  // size (x,y,z)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
                          1,                  // attribute 1. Color
                          3,                  // size (r,g,b)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

/**************************
 * Customizable functions *
 **************************/


void setStrokes(char val){
    scoredisp["top"].status=0;
    scoredisp["bottom"].status=0;
    scoredisp["middle"].status=0;
    scoredisp["left1"].status=0;
    scoredisp["left2"].status=0;
    scoredisp["right1"].status=0;
    scoredisp["right2"].status=0;
    scoredisp["middle1"].status=0;
    scoredisp["middle2"].status=0;
    scoredisp["diagonal"].status=0;
    if(val=='0' || val=='2' || val=='C' || val=='A' || val=='3' || val=='5' || val=='F' || val=='6'|| val=='R' || val=='7' || val=='8' || val=='9' || val=='P' || val=='I' || val=='O' || val=='N' || val=='T' || val=='S' || val=='E'){
        scoredisp["top"].status=1;
    }
    if(val=='2' || val=='3' || val=='4' || val=='5' || val=='A' || val=='6' || val=='R'  || val=='F'  || val=='8' || val=='9' || val=='P' || val=='S' || val=='Y' || val=='E'){

        scoredisp["middle"].status=1;
    }
    if(val=='0' || val=='2' || val=='3' || val=='C' || val=='5' || val=='6' || val=='8' || val=='9' || val=='O' || val=='S' || val=='I' || val=='Y' || val=='U' || val=='L' || val=='E' || val=='W'){
        scoredisp["bottom"].status=1;
    }
    if(val=='0' || val=='4' || val=='5' || val=='6' || val=='C' || val=='A' || val=='8' || val=='F'  || val=='9' || val=='P' || val=='R' || val=='O' || val=='N' || val=='S' || val=='Y' || val=='U' || val=='L' || val=='E' || val=='W'){
        scoredisp["left1"].status=1;
    }
    if(val=='0' || val=='2' || val=='6' || val=='8' || val=='P' || val=='C' || val=='A' || val=='F'  || val=='O' || val=='N' || val=='U' || val=='R' || val=='L' || val=='E' || val=='W'){
        scoredisp["left2"].status=1;
    }
    if(val=='0' || val=='2' || val=='3' || val=='4' || val=='7' || val=='8' || val=='A' || val=='R' || val=='9' || val=='P' || val=='O' || val=='N' || val=='Y' || val=='U' || val=='W'){
        scoredisp["right1"].status=1;
    }
    if(val=='0' || val=='3' || val=='4' || val=='5' || val=='6' || val=='7' || val=='8'  || val=='A' || val=='9' || val=='O' || val=='N' || val=='S' || val=='Y' || val=='U' || val=='W'){
        scoredisp["right2"].status=1;
    }
    if(val=='I' || val=='T' || val=='1'){
        scoredisp["middle1"].status=1;
    }
    if(val=='I' || val=='T' || val=='W' || val=='1'){
        scoredisp["middle2"].status=1;
    }

    if(val=='R')
    {
        scoredisp["diagonal"].status = 1;
    }
}


/*void createcircle (string name, float angle, COLOR color,float x, float y,float z, float height, float width,float depth, string component,int status,float numx,float numy)
{
    int parts = 1000;
    GLfloat vertex_buffer_data[parts*9];
    GLfloat color_buffer_data[parts*9];
    int i,j;
    float angle=(2*M_PI/parts);
    float current_angle = 0;
    for(i=0;i<parts;i++){
        for(j=0;j<3;j++){
            color_buffer_data[i*9+j*3]=color.r;
            color_buffer_data[i*9+j*3+1]=color.g;
            color_buffer_data[i*9+j*3+2]=color.b;
        }
        vertex_buffer_data[i*9]=0;
        vertex_buffer_data[i*9+1]=0;
        vertex_buffer_data[i*9+2]=0;
        vertex_buffer_data[i*9+3]=rx*cos(current_angle);
        vertex_buffer_data[i*9+4]=ry*sin(current_angle);
        vertex_buffer_data[i*9+5]=0;
        vertex_buffer_data[i*9+6]=rx*cos(current_angle+angle);
        vertex_buffer_data[i*9+7]=ry*sin(current_angle+angle);
        vertex_buffer_data[i*9+8]=0;
        current_angle+=angle;
    }
    VAO* circle;
    if(fill==1)
        circle = create3DObject(GL_TRIANGLES, (parts*9)/3, vertex_buffer_data, color_buffer_data, GL_FILL);
    else
        circle = create3DObject(GL_TRIANGLES, (parts*9)/3, vertex_buffer_data, color_buffer_data, GL_LINE);
    
    Game InstanceGame = {};
    InstanceGame.color = color;
    InstanceGame.name = name;
    InstanceGame.object = circle;
    InstanceGame.x=x;
    InstanceGame.y=y;
    InstanceGame.z=z;
    InstanceGame.height=2*height;
    InstanceGame.width=2*width;
    InstanceGame.depth=depth;
    InstanceGame.radius=height;
    InstanceGame.angle_x=angle;
    InstanceGame.status=status;
    InstanceGame.numx=numx;
    InstanceGame.numy=numy;

    if(component=="life")
        life[name]=InstanceGame;
    
}*/

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */

/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

    GLfloat fov = M_PI/2;

    // sets the viewport of openGL renderer
    glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

    // Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    Matrices.projection = glm::perspective(fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    //Matrices.projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 500.0f);
}
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
    case GLFW_KEY_UP:
        if(left1==0 && right1==0 && up1==0 && down1==0)
        {
            up1=1;
            down1=0;
            right1=0;
            left1=0;
            user_score += 1;
        }
        break;
    case GLFW_KEY_DOWN:
        if(left1==0 && right1==0  && up1==0  && down1==0)
        {
            down1=1;
            up1=0;
            right1=0;
            left1=0;
            user_score += 1;

        }
        break;
    case GLFW_KEY_LEFT:
        if(left1==0 && right1==0 && up1==0 && down1==0)
        {
            right1=0;
            left1=1;
            up1=0;
            down1=0;
            user_score += 1;

        }
        break;
    case GLFW_KEY_RIGHT:
        if(left1==0 && right1==0 && up1==0 && down1==0)
        {
            right1=1;
            left1=0;
            up1=0;
            down1=0;
            user_score += 1;

        }
        break;
    case GLFW_KEY_T:
        camera_top=1;
        camera_follow=0;
        camera_fps=0;
        camera_tower=0;
        camera_self=0;
        camera_helicopter=0;
        break;
    case GLFW_KEY_R:
        camera_top=0;
        camera_follow=0;
        camera_fps=0;
        camera_tower=1;
        camera_self=0;
        camera_helicopter=0;
        break;
    case GLFW_KEY_F:
        camera_top=0;
        camera_follow=1;
        camera_fps=0;
        camera_tower=0;
        camera_self=0;
        camera_helicopter=0;
        break;
    case GLFW_KEY_P:
        camera_top=0;
        camera_follow=0;
        camera_fps=0;
        camera_tower=0;
        camera_self=0;
        camera_helicopter=1;
        break;
 case GLFW_KEY_I:
    camera_top=0;
    camera_follow=0;
    camera_fps=0;
    camera_tower=0;
    camera_self=1;
    camera_helicopter=0;
    break;
	default:
	    break;
        }
    }
    else if (action == GLFW_PRESS) {
        switch (key) {
	case GLFW_KEY_ESCAPE:
	    quit(window);
	    break;
	default:
	    break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
    switch (key) {
    case 'Q':
    case 'q':
	quit(window);
	break;
    case 'a':
	   if(camera_follow==1 || camera_self==1)
       {
            orient_left=1;
            orient_right=0;
            orient_backward=0;
            orient_forward=0;
       }
	break;
    case 'd':
    	if(camera_follow==1 ||  camera_self==1)
           {
                orient_left=0;
                orient_right=1;
                orient_backward=0;
                orient_forward=0;
           }
	break;
    case 'w':
	   if(camera_follow==1 ||camera_self==1)
       {
            orient_left=0;
            orient_right=0;
            orient_backward=0;
            orient_forward=1;
       }
	break;
    case 's':
        if(camera_follow==1 || camera_self==1)
       {
            orient_left=0;
            orient_right=0;
            orient_backward=1;
            orient_forward=0;
       }
	break;
    case ' ':
	do_rot ^= 1;
	break;
    default:
	break;
    }
}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        if(action == GLFW_PRESS)
        {
            mouse_click=1;
            keyboard_press=0;
            right_mouse_click=0;
        }
        if(action == GLFW_RELEASE)
        {
            mouse_click=0;
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        if(action == GLFW_PRESS)
        {
            right_mouse_click=1;
            keyboard_press=0;
            mouse_click=0;
        }
	    if(action == GLFW_RELEASE) 
        {
	       right_mouse_click=0;
	    }
	   break;
    default:
	break;
    }
}

VAO *rectangle, *cam, *floor_vao;

// Creates the rectangle object used in this sample code
void createCam ()
{
    // GL3 accepts only Triangles. Quads are not supported
    static const GLfloat vertex_buffer_data [] = {
	-0.1, 0, 0,
	0.1, 0, 0, 
	0, 0.1, 0,
    };

    static const GLfloat color_buffer_data [] = {
	1, 1, 1,
	1, 1, 1,
	1, 1, 1,
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    cam = create3DObject(GL_TRIANGLES, 1*3, vertex_buffer_data, color_buffer_data, GL_LINE);
}

void createtile (string name, float angle, COLOR color,COLOR color2,COLOR color3,COLOR color4,float x, float y,float z, float height, float width,float depth, string component,int status,float numx,float numy)
{
    // GL3 accepts only Triangles. Quads are not supported
    float w=width/2,h=height/2,d=depth/2;
    GLfloat vertex_buffer_data [] = {
        /*-w,-h,d, // vertex 1
        w,-h,d, // vertex 2
        -w,-h,-d, // vertex 3
        -w,-h,-d, // vertex 3
        w,-h,d, // vertex 4
        w,-h,-d  // vertex 1*/
        -w, h, d,
        -w, -h, d, 
        w, -h, d,
        -w, h, d, 
        w, -h, d,
        w, h, d,
        w, h, d,
        w, -h, d,
        w, -h, -d,
        w, h, d,
        w, -h, -d,
        w, h, -d,
        w, h, -d,
        w, -h, -d,
        -w, -h, -d,
        w, h, -d,
        -w, -h, -d,
        -w, h, -d,
        -w, h, -d,
        -w, -h, -d,
        -w, -h, d, 
        -w, h, -d,
        -w, -h, d, 
        -w, h, d, 
        -w, h, -d,
        -w, h, d, 
        w, h, d,
        -w, h, -d,
        w, h, d,
        w, h, -d,
        -w, -h, d, 
        -w, -h, -d,
        w, -h, -d,
        -w, -h, d, 
        w, -h, -d,
        w, -h, d,
        -w, h, d,
        w, h, -d,
        w, h, -d,
    };

    GLfloat color_buffer_data [] = {
        color4.r,color4.g,color4.b, // color 1
        color4.r,color4.g,color4.b, // color 2
        color4.r,color4.g,color4.b, // color 3

        color4.r,color4.g,color4.b, // color 4
        color4.r,color4.g,color4.b, // color 5
        color4.r,color4.g,color4.b, // color 6

        color4.r,color4.g,color4.b, // color 1
        color4.r,color4.g,color4.b, // color 2
        color4.r,color4.g,color4.b, // color 3

        color4.r,color4.g,color4.b, // color 4
        color4.r,color4.g,color4.b, // color 5
        color4.r,color4.g,color4.b, // color 6

        color4.r,color4.g,color4.b, // color 1
        color4.r,color4.g,color4.b, // color 2
        color4.r,color4.g,color4.b, // color 3

        color4.r,color4.g,color4.b, // color 4
        color4.r,color4.g,color4.b, // color 5
        color4.r,color4.g,color4.b, // color 6

        color4.r,color4.g,color4.b, // color 1
        color4.r,color4.g,color4.b, // color 2
        color4.r,color4.g,color4.b, // color 3

        color4.r,color4.g,color4.b, // color 4
        color4.r,color4.g,color4.b, // color 5
        color4.r,color4.g,color4.b, // color 6

        color.r,color.g,color.b, // color 1
        color2.r,color2.g,color2.b, // color 2
        color3.r,color3.g,color3.b, // color 3

        color3.r,color3.g,color3.b, // color 4
        color4.r,color4.g,color4.b, // color 5
        color.r,color.g,color.b, // color 6

        color.r,color.g,color.b, // color 1
        color2.r,color2.g,color2.b, // color 2
        color3.r,color3.g,color3.b, // color 3

        color3.r,color3.g,color3.b, // color 4
        color4.r,color4.g,color4.b, // color 5
        color.r,color.g,color.b, // color 6
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    VAO *tile = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color_buffer_data, GL_FILL);
    Game InstanceGame = {};
    InstanceGame.color = color;
    InstanceGame.name = name;
    InstanceGame.object = tile;
    InstanceGame.x=x;
    InstanceGame.y=y;
    InstanceGame.z=z;
    InstanceGame.height=height;
    InstanceGame.width=width;
    InstanceGame.depth=depth;
    InstanceGame.radius=(sqrt(height*height+width*width+depth*depth))/2;
    InstanceGame.angle_x=angle;
    InstanceGame.status=status;
    InstanceGame.numx=numx;
    InstanceGame.numy=numy;

    if(component=="tile")
    {
        tiles[name]=InstanceGame;
    }
    else if(component=="cube")
    {
        cube[name]=InstanceGame;
    }
    if(component=="score")
    {        
        scoredisp[name] = InstanceGame;
        if(name=="diagonal")
        {
            scoredisp[name].angle_x = M_PI/4;
        }

    }
    
}

float camera_rotation_angle = 90;

/* Render the scene with openGL */
/* Edit this function according to your assignment */
float angle;
int rep=0,fl=0;
glm::mat4 rotateTriangle2;
glm::mat4 rotateTriangle1;
void draw (GLFWwindow* window,float x,float y,float w,float h,int doM, int doV, int doP)
{


    COLOR grey = {165.0/255.0,165.0/255.0,165.0/255.0};
    COLOR dimgrey = {105.0/255.0,105.0/255.0,105.0/255.0};
    COLOR gold = {218.0/255.0,165.0/255.0,32.0/255.0};
    COLOR coingold = {255.0/255.0,223.0/255.0,0.0/255.0};
    COLOR red = {255.0/255.0,51.0/255.0,51.0/255.0};
    COLOR lightgreen = {57/255.0,230/255.0,0/255.0};
    COLOR darkgreen = {51/255.0,102/255.0,0/255.0};
    COLOR black = {30/255.0,30/255.0,21/255.0};
    COLOR blue = {0,0,1};
    COLOR darkbrown = {46/255.0,46/255.0,31/255.0};
    COLOR lightbrown = {95/255.0,63/255.0,32/255.0};
    COLOR brown1 = {117/255.0,78/255.0,40/255.0};
    COLOR brown2 = {134/255.0,89/255.0,40/255.0};
    COLOR brown3 = {46/255.0,46/255.0,31/255.0};
    COLOR cratebrown = {153/255.0,102/255.0,0/255.0};
    COLOR cratebrown1 = {121/255.0,85/255.0,0/255.0};
    COLOR cratebrown2 = {102/255.0,68/255.0,0/255.0};
    COLOR slateblue = {106/255.0, 90/255.0, 205/255.0};
    COLOR aliceblue = {240/255.0,248/255.0,255/255.0};
    COLOR skyblue1 = {123/255.0,201/255.0,227/255.0};
    COLOR skyblue = {132/255.0,217/255.0,245/255.0};
    COLOR cloudwhite = {229/255.0,255/255.0,255/255.0};
    COLOR cloudwhite1 = {204/255.0,255/255.0,255/255.0};
    COLOR lightpink = {255/255.0,122/255.0,173/255.0};
    COLOR darkpink = {255/255.0,51/255.0,119/255.0};
    COLOR white = {255/255.0,255/255.0,255/255.0};
    COLOR azure = {255/255.0,245/255.0,238/255.0};
    COLOR score = {117/255.0,78/255.0,40/255.0};
    COLOR chocolate = {210/255.0, 105/255.0, 30/255.0};
    COLOR indianred = {205/255.0, 92/255.0, 92/255.0};
    COLOR peach = {255/255.0, 218/255.0, 185/255.0};
    COLOR wheat = {244/255.0, 164/255.0, 96/255.0};

    int fbwidth, fbheight;
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);
    glViewport((int)(x*fbwidth), (int)(y*fbheight), (int)(w*fbwidth), (int)(h*fbheight));


    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    /*camera_radius=1;
    angle=90;
    target_x_1=0;
    target_y_1=0;
    target_z_1=0;
    eye_x_1=-4;
    eye_y_1=12;
    eye_z_1=-4;*/
    float temp1,temp2,temp3;
    temp1= (cube["cube1"].x+cube["cube2"].x)/2;
    temp3 = (cube["cube1"].z+cube["cube2"].z)/2;
    temp2 = (cube["cube1"].y+cube["cube2"].y)/2;
    if(camera_top==1)
    {
        eye_x_1 = 0+cos(45*M_PI/180);
        eye_z_1 = 0;
        //+sin(45*M_PI/180);
        eye_y_1=20;
        target_x_1=0;
        target_y_1=0;
        target_z_1=0;
    }
    if(camera_tower==1)
    {
        eye_x_1 = 15,
        eye_y_1 = 15;
        eye_z_1 = 0;
        target_z_1 = 0;
        target_y_1 = 0;
        target_x_1 = 0;    
    }
    if(camera_follow==1)
    {
        if(orient_left==1)
        {
            eye_x_1 = temp1-5;
            eye_y_1 = temp2;
            eye_z_1 = temp3;
            target_x_1 = 1000;
            target_y_1 = temp2;
            target_z_1 = temp3;
        }
        else if(orient_right==1)
        {
            eye_x_1 = temp1+5;
            eye_y_1 = temp2;
            eye_z_1 = temp3;
            target_x_1 = -1000;
            target_y_1 = temp2;
            target_z_1 = temp3;
        }
        else if(orient_backward==1)
        {
            eye_x_1 = temp1;
            eye_y_1 = temp2;
            eye_z_1 = temp3+5;
            target_x_1 = temp1;
            target_y_1 = temp2;
            target_z_1 = -1000;
        }
        else
        {
            eye_x_1 = temp1;
            eye_y_1 = temp2;
            eye_z_1 = temp3-5;
            target_x_1 = temp1;
            target_y_1 = temp2;
            target_z_1 = 1000;
        }
    }
    if(camera_self==1)
    {
        if(orient_left==1)
        {
            eye_x_1 = temp1-3;
            eye_y_1 = temp2;
            eye_z_1 = temp3;
            target_x_1 = -1000;
            target_y_1 = temp2;
            target_z_1 = temp3;
        }
        else if(orient_right==1)
        {
            eye_x_1 = temp1+3;
            eye_y_1 = temp2;
            eye_z_1 = temp3;
            target_x_1 = 1000;
            target_y_1 = temp2;
            target_z_1 = temp3;
        }
        else if(orient_backward==1)
        {
            eye_x_1 = temp1;
            eye_y_1 = temp2;
            eye_z_1 = temp3-3;
            target_x_1 = temp1;
            target_y_1 = temp2;
            target_z_1 = -1000;
        }
        else
        {
            eye_x_1 = temp1;
            eye_y_1 = temp2;
            eye_z_1 = temp3+3;
            target_x_1 = temp1;
            target_y_1 = temp2;
            target_z_1 = 1000;
        }
    }
    glfwGetCursorPos(window, &mouse_pos_x, &mouse_pos_y);
    if(camera_helicopter==1)
    {
        if(mouse_click==1)
        {
            angle=(mouse_pos_x)*360/600;
            eye_x_1 = 20*cos(angle*M_PI/180);
            eye_z_1 = 20*sin(angle*M_PI/180);
            target_x_1 = 0;
            target_z_1 = 0;
            target_y_1 = 0;
        }
        if(right_mouse_click==1)
        {
            angle = 90-(mouse_pos_y)*90/600;
            eye_y_1 = 20*sin(angle*M_PI/180);
            target_x_1 = 0;
            target_z_1 = 0;
            target_y_1 = 0;
        }
    }
    prev_mouse_pos_x = mouse_pos_x;
    prev_mouse_pos_y = mouse_pos_y;
    if(camera_self==0 && camera_follow==0)
    {
        orient_forward=1;
        orient_right=0;
        orient_left=0;
        orient_backward=0;
    }
    glm::vec3 eye(eye_x_1,eye_y_1,eye_z_1);
    //glm::vec3 eye ( 8*sin(camera_rotation_angle*M_PI/180.0f), 3, 8*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (target_x_1,target_y_1,target_z_1);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    if(doV)
	Matrices.view = glm::lookAt(eye, target, up); // Fixed camera for 2D (ortho) in XY plane
    else
	Matrices.view = glm::mat4(1.0f);

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    glm::mat4 VP;
    if (doP)
	VP = Matrices.projection * Matrices.view;
    else
	VP = Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    glm::mat4 MVP;	// MVP = Projection * View * Model

    // Load identity to model matrix
    Matrices.model = glm::mat4(1.0f);
    string scored = "score";
        int ps_score = user_score;
        int ones = ps_score%10;
        ps_score = ps_score/10;
        int tenth = ps_score%10;
        ps_score = ps_score/10;
        int hundreth = ps_score%10;
        int k_s;
        for(k_s=0;k_s<=11;k_s++)
        {
            if(k_s==0)
            {
                char inp = ones + '0';
                setStrokes(inp);
            }

            if(k_s==1)
            {
                char inp = tenth + '0';
                setStrokes(inp);
            }

            if(k_s==2)
            {
                char inp = hundreth + '0';
                setStrokes(inp);
            }

            if(k_s==3)
            {
                char inp = 'S';
                setStrokes(inp);
            }

            if(k_s==4)
            {
                char inp = 'C';
                setStrokes(inp);
            }

            if(k_s==5)
            {
                char inp = 'O';
                setStrokes(inp);
            }

            if(k_s==6)
            {
                char inp = 'R';
                setStrokes(inp);
            }
            if(k_s==7)
            {
                char inp = 'E';
                setStrokes(inp);
            }
            if(k_s==8)
            {
                char inp = 'L';
                setStrokes(inp);
            }

            if(k_s==9)
            {
                char inp = 'I';
                setStrokes(inp);
            }
            if(k_s==10)
            {
                char inp = 'F';
                setStrokes(inp);
            }

            if(k_s==11)
            {
                char inp = 'E';
                setStrokes(inp);
            }

          


            for(map<string,Game>::iterator it=scoredisp.begin();it!=scoredisp.end();it++)
            {
                string current = it->first; //The name of the current object
                if(scoredisp[current].status==0)
                {
                    continue;
                }

                glm::mat4 MVP;  // MVP = Projection * View * Model

                Matrices.model = glm::mat4(1.0f);

                glm::mat4 ObjectTransform;
                
                if(k_s==0)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-15, scoredisp[current].y-150, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                
                }
                if(k_s==1)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-36-15, scoredisp[current].y-150, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }

                if(k_s==2)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-72-15, scoredisp[current].y-150, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }
                if(k_s==3)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-130, scoredisp[current].y+70-150, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }
                if(k_s==4)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-90, scoredisp[current].y+70-150, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }
                if(k_s==5)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-50, scoredisp[current].y+70-150, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }
                if(k_s==6)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-10, scoredisp[current].y+70-150, 0.0f)); // glTranslatef
                    glm::mat4 rotateObjectAct = glm::rotate((float)(scoredisp[current].angle_x), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
                    ObjectTransform=translateObject*rotateObjectAct;
                    
                }
                if(k_s==7)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x+30, scoredisp[current].y+70-150, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }
                if(k_s==8)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-100, scoredisp[current].y+200, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }

                if(k_s==9)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-60, scoredisp[current].y+200, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }

                if(k_s==10)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x-20, scoredisp[current].y+200, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }
                if(k_s==11)
                {
                    glm::mat4 translateObject = glm::translate (glm::vec3(scoredisp[current].x+20, scoredisp[current].y+200, 0.0f)); // glTranslatef
                    ObjectTransform=translateObject;
                }


                Matrices.model *= ObjectTransform;
                MVP = VP * Matrices.model; // MVP = p * V * M
                
                glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

                draw3DObject(scoredisp[current].object);
                //glPopMatrix (); 
            }
        }

    if(up1==1)
    {
        if(cube["cube1"].y==cube["cube2"].y)
        {
            fl=1;
        }
        if(fl==1)
        {
            if(cube["cube1"].x>cube["cube2"].x)
            {
                //cube["cube2"].y=2.5;
                //cube["cube2"].x=cube["cube1"].x;
                cube["cube2"].x-=0.2;
                cube["cube1"].x-=0.4;
                cube["cube1"].y+=0.2;
            }
            else if(cube["cube1"].x<cube["cube2"].x)
            {
                cube["cube2"].y+=0.2;
                //cube["cube1"].x=cube["cube2"].x;
                cube["cube1"].x-=0.2;
                cube["cube2"].x-=0.4;
            }
            else
            {
                cube["cube1"].x-=0.2;
                cube["cube2"].x-=0.2;
                cube["cube1"].x=cube["cube2"].x;
            }
        }
        else
        {
            if(cube["cube1"].y>cube["cube2"].y)
            {
                cube["cube1"].x-=0.4;
                cube["cube2"].x-=0.2;
                cube["cube1"].y-=0.2;
                //cube["cube1"].y=cube["cube2"].y;

            }
            else
            {
                cube["cube2"].x-=0.4;
                cube["cube1"].x-=0.2;
                cube["cube2"].y-=0.2;
                //cube["cube2"].y=cube["cube1"].y;
            }
        }
        cube["cube1"].angle_z+=9;
        cube["cube2"].angle_z+=9;
        rotateTriangle1 = glm::rotate((float)(((cube["cube1"].angle_z))*M_PI/180.0f), glm::vec3(0,0,1));
        //rotateTriangle2 = glm::rotate((float)(((cube["cube1"].angle_x))*M_PI/180.0f), glm::vec3(1,0,0));
        rep++;
        if(rep==10)
        {
            rep=0;
            up1=0;
            if(fl==0)
            {
                cube["cube1"].y=cube["cube2"].y;
            }
            if(fl==1)
            {
                cube["cube1"].x=cube["cube2"].x;
            }
            fl=0;
        }
    }

    if(down1==1)
    {
        float ch1,ch2;
        ch1=cube["cube1"].y;
        ch2=cube["cube2"].y;
        if(ch1==ch2)
        {
            fl=1;
        }
        if(fl==1)
        {
            if(cube["cube1"].x>cube["cube2"].x)
            {
    
                cube["cube2"].x+=0.4;
                cube["cube1"].x+=0.2;
                cube["cube2"].y+=0.2;
            }
            else if(cube["cube1"].x<cube["cube2"].x)
            {
                cube["cube1"].y+=0.2;
                cube["cube2"].x+=0.2;
                cube["cube1"].x+=0.4;
            }
            else
            {
                cube["cube1"].x+=0.2;
                cube["cube2"].x+=0.2;
                cube["cube1"].x=cube["cube2"].x;
            }
        }
        else
        {
            if(cube["cube1"].y>cube["cube2"].y)
            {
                cube["cube1"].x+=0.4;
                cube["cube2"].x+=0.2;
                cube["cube1"].y-=0.2;
                //cube["cube1"].y=cube["cube2"].y;

            }
            else
            {
                cube["cube2"].x+=0.4;
                cube["cube1"].x+=0.2;
                cube["cube2"].y-=0.2;
                //cube["cube2"].y=cube["cube1"].y;
            }
        }
        cube["cube1"].angle_z-=9;
        cube["cube2"].angle_z-=9;
        rotateTriangle1 = glm::rotate((float)(((cube["cube1"].angle_z))*M_PI/180.0f), glm::vec3(0,0,1));
        //rotateTriangle2 = glm::rotate((float)(((cube["cube1"].angle_x))*M_PI/180.0f), glm::vec3(1,0,0));
        rep++;
        if(rep==10)
        {
            rep=0;
            down1=0;
            if(fl==0)
            {
                cube["cube1"].y=cube["cube2"].y;
            }
            if(fl==1)
            {
                cube["cube1"].x=cube["cube2"].x;
            }
            fl=0;
        }
    }
    
    if(right1==1)
    {
        if(cube["cube1"].y==cube["cube2"].y)
        {
            fl=1;
        }
        if(fl==1)
        {
            if(cube["cube1"].z>cube["cube2"].z)
            {
                cube["cube1"].z-=0.4;
                cube["cube2"].z-=0.2;
                cube["cube1"].y+=0.2;
            }
            else if(cube["cube1"].z<cube["cube2"].z)
            {
                cube["cube2"].y+=0.2;
                //cube["cube1"].x=cube["cube2"].x;
                cube["cube2"].z-=0.4;
                cube["cube1"].z-=0.2;
            }
            else
            {
                cube["cube1"].z-=0.2;
                cube["cube2"].z-=0.2;
                cube["cube1"].z=cube["cube2"].z;
            }
        }
        else
        {
            if(cube["cube1"].y>cube["cube2"].y)
            {
                cube["cube1"].z-=0.4;
                cube["cube2"].z-=0.2;
                cube["cube1"].y-=0.2;
                //cube["cube1"].y=cube["cube2"].y;

            }
            else
            {
                cube["cube2"].z-=0.4;
                cube["cube1"].z-=0.2;
                cube["cube2"].y-=0.2;
                //cube["cube2"].y=cube["cube1"].y;
            }
        }
        cube["cube1"].angle_x-=9;
        cube["cube2"].angle_x-=9;
        rotateTriangle1 = glm::rotate((float)(((cube["cube1"].angle_x))*M_PI/180.0f), glm::vec3(1,0,0));
        //rotateTriangle2 = glm::rotate((float)(((cube["cube1"].angle_z))*M_PI/180.0f), glm::vec3(1,0,0));
        rep++;
        if(rep==10)
        {
            rep=0;
            right1=0;
            if(fl==0)
            {
                cube["cube1"].y=cube["cube2"].y;
            }
            if(fl==1)
            {
                cube["cube1"].z=cube["cube2"].z;
            }
            fl=0;
        }
    }



    if(left1==1)
    {
        if(cube["cube1"].y==cube["cube2"].y)
        {
            fl=1;
        }
        if(fl==1)
        {
            if(cube["cube1"].z>cube["cube2"].z)
            {
                //cube["cube2"].y=2.5;
                //cube["cube2"].x=cube["cube1"].x;
                cube["cube2"].z+=0.4;
                cube["cube1"].z+=0.2;
                cube["cube2"].y+=0.2;
            }
            else if(cube["cube1"].z<cube["cube2"].z)
            {
                cube["cube1"].y+=0.2;
                //cube["cube1"].x=cube["cube2"].x;
                cube["cube1"].z+=0.4;
                cube["cube2"].z+=0.2;
            }
            else
            {
                cube["cube1"].z+=0.2;
                cube["cube2"].z+=0.2;
                cube["cube1"].z=cube["cube2"].z;
            }
        }
        else
        {
            if(cube["cube1"].y>cube["cube2"].y)
            {
                cube["cube1"].z+=0.4;
                cube["cube2"].z+=0.2;
                cube["cube1"].y-=0.2;
                //cube["cube1"].y=cube["cube2"].y;

            }
            else
            {
                cube["cube2"].z+=0.4;
                cube["cube1"].z+=0.2;
                cube["cube2"].y-=0.2;
                //cube["cube2"].y=cube["cube1"].y;
            }
        }
        cube["cube1"].angle_x+=9;
        cube["cube2"].angle_x+=9;
        rotateTriangle1 = glm::rotate((float)(((cube["cube1"].angle_x))*M_PI/180.0f), glm::vec3(1,0,0));
        //rotateTriangle2 = glm::rotate((float)(((cube["cube1"].angle_z))*M_PI/180.0f), glm::vec3(1,0,0));
        rep++;
        if(rep==10)
        {
            rep=0;
            left1=0;
            if(fl==0)
            {
                cube["cube1"].y=cube["cube2"].y;
            }
            if(fl==1)
            {
                cube["cube1"].z=cube["cube2"].z;
            }
            fl=0;
        }
    }
    if(right1==1)
    {
        if(cube["cube1"].y==cube["cube2"].y)
        {
            fl=1;
        }
        if(fl==1)
        {
            if(cube["cube1"].z>cube["cube2"].z)
            {
                cube["cube1"].z-=0.4;
                cube["cube2"].z-=0.2;
                cube["cube1"].y+=0.2;
            }
            else if(cube["cube1"].z<cube["cube2"].z)
            {
                cube["cube2"].y+=0.2;
                //cube["cube1"].x=cube["cube2"].x;
                cube["cube2"].z-=0.4;
                cube["cube1"].z-=0.2;
            }
            else
            {
                cube["cube1"].z-=0.2;
                cube["cube2"].z-=0.2;
                cube["cube1"].z=cube["cube2"].z;
            }
        }
        else
        {
            if(cube["cube1"].y>cube["cube2"].y)
            {
                cube["cube1"].z-=0.4;
                cube["cube2"].z-=0.2;
                cube["cube1"].y-=0.2;
                //cube["cube1"].y=cube["cube2"].y;

            }
            else
            {
                cube["cube2"].z-=0.4;
                cube["cube1"].z-=0.2;
                cube["cube2"].y-=0.2;
                //cube["cube2"].y=cube["cube1"].y;
            }
        }
        cube["cube1"].angle_x-=9;
        cube["cube2"].angle_x-=9;
        rotateTriangle1 = glm::rotate((float)(((cube["cube1"].angle_x))*M_PI/180.0f), glm::vec3(1,0,0));
        //rotateTriangle2 = glm::rotate((float)(((cube["cube1"].angle_z))*M_PI/180.0f), glm::vec3(1,0,0));
        rep++;
        if(rep==10)
        {
            rep=0;
            right1=0;
            if(fl==0)
            {
                cube["cube1"].y=cube["cube2"].y;
            }
            if(fl==1)
            {
                cube["cube1"].z=cube["cube2"].z;
            }
            fl=0;
        }
    }


    int k=0,i,j;

    for(i=0;i<10;i++)
    {

        for(j=0;j<10;j++)
        {
            if(gamemap[i][j]>2 && cube["cube1"].x==cube["cube2"].x && cube["cube1"].x>2*(i-3)-1 && cube["cube1"].x<2*(i-3)+1 && cube["cube1"].z>2*(j-3)-1 && cube["cube1"].z<2*(j-3)+1)
            {
    

                string c="tile";
                char d=k_tile+'0';
                string e=c+d;
                int ones = gamemap[i][j]%10;
                int temp_num = gamemap[i][j]/10;
                int tens = temp_num%10;
                if(gamemap[tens][ones]==0)
                {
                    createtile(e,0,gold,gold,gold,gold,2*(tens-3),-2,2*(ones-3),1,2,2,"tile",0,i,j);
                    gamemap[tens][ones] = 1;
                    k_tile++;
                }
            }
        }
    }

    for(map<string,Game>::iterator it=tiles.begin();it!=tiles.end();it++){
        string current = it->first; //The name of the current object
        glm::mat4 MVP;  // MVP = Projection * View * Model

        Matrices.model = glm::mat4(1.0f);

        glm::mat4 ObjectTransform;
        glm::mat4 translateObject = glm::translate (glm::vec3(tiles[current].x,tiles[current].y, tiles[current].z)); 
        glm::mat4 rotateTriangle = glm::rotate((float)((tiles[current].angle_x)*M_PI/180.0f), glm::vec3(0,0,1));// glTranslatef
        ObjectTransform=translateObject*rotateTriangle;
        Matrices.model *= ObjectTransform;
        //MVP = VP * Matrices.model; // MVP = p * V * M
        if(doM)
            MVP = VP * Matrices.model;
        else
            MVP = VP;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

        draw3DObject(tiles[current].object);
        //glPopMatrix (); 
    }
    for(map<string,Game>::iterator it=cube.begin();it!=cube.end();it++){
        string current = it->first; //The name of the current object
        glm::mat4 MVP;  // MVP = Projection * View * Model
        Matrices.model = glm::mat4(1.0f);
        glm::mat4 ObjectTransform;
        glm::mat4 translateObject = glm::translate (glm::vec3(cube[current].x,cube[current].y, cube[current].z));
        ObjectTransform=translateObject*rotateTriangle1;
        Matrices.model *= ObjectTransform;
        //MVP = VP * Matrices.model; // MVP = p * V * M
        if(doM)
            MVP = VP * Matrices.model;
        else
            MVP = VP;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

        draw3DObject(cube[current].object);
    }
    /*Matrices.model = glm::translate(floor_pos);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // draw3DObject draws the VAO given to it using current MVP matrix
    draw3DObject(floor_vao);*/

}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height){
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

    if (!window) {
	exit(EXIT_FAILURE);
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);
    glfwSetWindowCloseCallback(window, quit);
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks

    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    //createtile ();
    //createCam();
    //createFloor();
	
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

	int k=0,i,j;
    //createtile("background",0,skyblue2,skyblue1,skyblue1,skyblue4,0,0,0,1000,1000,1000,"background",0,0,0);
    //createtile("background1",0,skyblue2,skyblue1,skyblue,skyblue4,300,0,0,height,0,width,"background",0,0,0);
    for(i=0;i<10;i++)
    {

        for(j=0;j<10;j++)
        {
            if(gamemap[i][j]==1)
            {
    

                string c="tile";
                char d=k_tile+'0';
                string e=c+d;
                if((i+j)%2==0)
                { 
                    createtile(e,0,brown1,brown1,brown1,brown1,2*(i-3),-2,2*(j-3),1,2,2,"tile",0,i,j);
                }
                else
                {
                    createtile(e,0,brown2,brown2,brown2,brown2,2*(i-3),-2,2*(j-3),1,2,2,"tile",0,i,j);
                }
                k_tile++;
            }

            else if(gamemap[i][j]>2)
            {
                string c="tile";
                char d=k_tile+'0';
                string e=c+d;

                createtile(e,0,blue,blue,blue,blue,2*(i-3),-2,2*(j-3),1,2,2,"tile",0,i,j);

                k_tile++;
            }

        }
    }
    createtile("cube1",0,red,red,red,red,12,0.25,-4,2,2,2,"cube",0,0,0);
    createtile("cube2",0,red,red,red,red,12,2.25,-4,2,2,2,"cube",0,0,0);


    createtile("top",0,chocolate,chocolate,chocolate,chocolate,0+550,24,0,8,24,0,"score",0,0,0);
    createtile("bottom",0,chocolate,chocolate,chocolate,chocolate,0+550,-24,0,8,24,0,"score",0,0,0);
    createtile("middle",0,chocolate,chocolate,chocolate,chocolate,0+550,0,0,8,24,0,"score",0,0,0);
    createtile("left1",0,chocolate,chocolate,chocolate,chocolate,-24/2+550,24/2,0,24,8,0,"score",0,0,0);
    createtile("left2",0,chocolate,chocolate,chocolate,chocolate,-24/2+550,-24/2,0,24,8,0,"score",0,0,0);
    createtile("right1",0,chocolate,chocolate,chocolate,chocolate,24/2+550,24/2,0,24,8,0,"score",0,0,0);
    createtile("right2",0,chocolate,chocolate,chocolate,chocolate,24/2+550,-24/2,0,24,8,0,"score",0,0,0);
    createtile("middle1",0,chocolate,chocolate,chocolate,chocolate,0+550,24/2,0,24,8,0,"score",0,0,0);
    createtile("middle2",0,chocolate,chocolate,chocolate,chocolate,0+550,-24/2,0,24,8,0,"score",0,0,0);
    createtile("diagonal",0,chocolate,chocolate,chocolate,chocolate,0+550,-24/2,0,24*sqrt(2),8,0,"score",0,0,0);
    
    /*createCircle("1",10000,indianred,450+8,132,20,20,1000,"life",1);
    createCircle("2",10000,indianred,500+8,132,20,20,1000,"life",1);
    createCircle("3",10000,indianred,550+8,132,20,20,1000,"life",1);*/




    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (0.3f, 0.3f, 0.3f, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    // cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    // cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    // cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    // cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main (int argc, char** argv)
{
    int width = 600;
    int height = 600;
    do_rot = 0;
    floor_rel = 1;

    GLFWwindow* window = initGLFW(width, height);
    initGL (window, width, height);

    last_update_time = glfwGetTime();
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {

	// clear the color and depth in the frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // OpenGL Draw commands
	current_time = glfwGetTime();
    angle=135;
	/*if(do_rot)
	    camera_rotation_angle += 90*(current_time - last_update_time); // Simulating camera rotation
	if(camera_rotation_angle > 720)
	    camera_rotation_angle -= 720;*/
	last_update_time = current_time;
	draw(window, 0, 0 ,1,1,1, 1, 1);
	//draw(window, 0.5,0, 0.5, 1, 0, 1, 1);
	//draw(window, 0, 0.5, 0.5, 0.5, 1, 0, 1);
	//draw(window, 0.5, 0.5, 0.5, 0.5, 0, 0, 1);

        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    glfwTerminate();
    //    exit(EXIT_SUCCESS);
}
