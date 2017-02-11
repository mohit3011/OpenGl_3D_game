#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include <fstream>
#include <vector>

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

COLOR grey = {168.0/255.0,168.0/255.0,168.0/255.0};
COLOR gold = {218.0/255.0,165.0/255.0,32.0/255.0};
COLOR red = {255.0/255.0,51.0/255.0,51.0/255.0};
COLOR coingold = {255.0/255.0,223.0/255.0,0.0/255.0};
COLOR lightgreen = {57/255.0,230/255.0,0/255.0};
COLOR darkgreen = {51/255.0,102/255.0,0/255.0};
COLOR black = {30/255.0,30/255.0,21/255.0};
COLOR blue = {0,0,1};
COLOR ball = {255/255.0,228/225.0,181/255.0};
COLOR darkbrown = {46/255.0,46/255.0,31/255.0};
COLOR lightbrown = {95/255.0,63/255.0,32/255.0};
COLOR brown1 = {117/255.0,78/255.0,40/255.0};
COLOR brown2 = {134/255.0,89/255.0,40/255.0};
COLOR brown3 = {46/255.0,46/255.0,31/255.0};
COLOR cratebrown = {153/255.0,102/255.0,0/255.0};
COLOR cratebrown1 = {121/255.0,85/255.0,0/255.0};
COLOR cratebrown2 = {102/255.0,68/255.0,0/255.0};
COLOR skyblue7 = {95/255.0,158/255.0,160/255.0};
COLOR skyblue6 = {175/255.0,238/255.0,238/255.0};
COLOR skyblue5 = {70/255.0,130/255.0,180/255.0};
COLOR skyblue4 = {30/255.0,144/255.0,255/255};
COLOR skyblue3 = {135/255.0,206/255.0,250/255};
COLOR skyblue2 = {0/255.0,0/255.0,125/255.0};
COLOR skyblue1 = {123/255.0,201/255.0,227/255.0};
COLOR skyblue = {132/255.0,217/255.0,245/255.0};
COLOR cloudwhite = {229/255.0,255/255.0,255/255.0};
COLOR cloudwhite1 = {204/255.0,255/255.0,255/255.0};
COLOR lightpink = {255/255.0,122/255.0,173/255.0};
COLOR darkpink = {255/255.0,51/255.0,119/255.0};
COLOR white = {255/255.0,255/255.0,255/255.0};
COLOR score = {117/255.0,78/255.0,40/255.0};
COLOR lightred = {117/255.0,78/255.0,71/255.0};
COLOR otherred = {178/255.0,34/255.0,34/255.0};

struct Game {
    string name;
    COLOR color;
    float x,y,z;
    VAO* object;
    float height,width,radius,depth;
    float angle;
    int status;
    float numx;
    float numy;
};
map <string, Game> tiles;
map <string, Game> background;
map <string, Game> cuboid;

int gamemap[10][10]={
    {1,1,1,1,1,0,1,1,0,0},
    {0,0,0,1,1,1,1,1,0,0},
    {0,0,1,1,1,1,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},
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

int do_rot, floor_rel;;
GLuint programID;
double last_update_time, current_time;
glm::vec3 rect_pos, floor_pos;
float rectangle_rotation = 0;

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

float rectangle_rot_dir = 1;
bool rectangle_rot_status = true;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
    case GLFW_KEY_C:
        rectangle_rot_status = !rectangle_rot_status;
        break;
    case GLFW_KEY_P:
        break;
    case GLFW_KEY_X:
        // do something ..
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
    rect_pos.x -= 0.1;
    break;
    case 'd':
    rect_pos.x += 0.1;
    break;
    case 'w':
    rect_pos.y += 0.1;
    break;
    case 's':
    rect_pos.y -= 0.1;
    break;
    case 'r':
    rect_pos.z -= 0.1;
    break;
    case 'f':
    rect_pos.z += 0.1;
    break;
    case 'e':
    rectangle_rotation += 1;
    break;
    case 'j':
    floor_pos.x -= 0.1;
    break;
    case 'l':
    floor_pos.x += 0.1;
    break;
    case 'i':
    floor_pos.y += 0.1;
    break;
    case 'k':
    floor_pos.y -= 0.1;
    break;
    case 'y':
    floor_pos.z -= 0.1;
    break;
    case 'h':
    floor_pos.z += 0.1;
    break;
    case 'g':
    floor_rel ^= 1;
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
    case GLFW_MOUSE_BUTTON_RIGHT:
    if (action == GLFW_RELEASE) {
        rectangle_rot_dir *= -1;
    }
    break;
    default:
    break;
    }
}


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

VAO *rectangle, *cam, *floor_vao;


void createRectangle (string name, float angle, COLOR color,COLOR color2,COLOR color3,COLOR color4,float x, float y,float z, float height, float width, float depth, string component,int status,float numx,float numy)
{
    // GL3 accepts only Triangles. Quads are not supported
    float w=width,h=height,d=depth;
    static const GLfloat vertex_buffer_data [] = {
    -w/2, h/2, d/2, 
    -w/2, -h/2, d/2, 
    w/2, -h/2, d/2,
    -w/2, h/2, d/2, 
    w/2, -h/2, d/2,
    w/2, h/2, d/2,

    w/2, h/2, d/2,
    w/2, -h/2, d/2,
    w/2, -h/2, -d/2,
    w/2, h/2, d/2,
    w/2, -h/2, -d/2,
    w/2, h/2, -d/2,

    w/2, h/2, -d/2,
    w/2, -h/2, -d/2,
    -w/2, -h/2, -d/2,
    w/2, h/2, -d/2,
    -w/2, -h/2, -d/2,
    -w/2, h/2, -d/2,

    -w/2, h/2, -d/2,
    -w/2, -h/2, -d/2,
    -w/2, -h/2, d/2, 
    -w/2, h/2, -d/2,
    -w/2, -h/2, d/2, 
    -w/2, h/2, d/2, 
    
    -w/2, h/2, -d/2,
    -w/2, h/2, d/2, 
    w/2, h/2, d/2,
    -w/2, h/2, -d/2,
    w/2, h/2, d/2,
    w/2, h/2, -d/2,
    
    -w/2, -h/2, d/2, 
    -w/2, -h/2, -d/2,
    w/2, -h/2, -d/2,
    -w/2, -h/2, d/2, 
    w/2, -h/2, -d/2,
    w/2, -h/2, d/2,
    
    };

    static const GLfloat color_buffer_data [] = {
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,


    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,


    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,


    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    rectangle = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color_buffer_data, GL_FILL);
    Game InstanceGame = {};
    InstanceGame.color = color;
    InstanceGame.name = name;
    InstanceGame.object = rectangle;
    InstanceGame.x=x;
    InstanceGame.y=y;
    InstanceGame.z=z;
    InstanceGame.height=height;
    InstanceGame.width=width;
    InstanceGame.depth=depth;
    InstanceGame.radius=(sqrt(height*height+width*width+depth*depth))/2;
    InstanceGame.angle=angle;
    InstanceGame.status=status;
    InstanceGame.numx=numx;
    InstanceGame.numy=numy;
    if(component=="tile")
        tiles[name]=InstanceGame;
    else if(component=="background")
        background[name]=InstanceGame;
    else if(component=="cuboid")
        cuboid[name]=InstanceGame;
}
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
        -w,-h,d, // vertex 1
        w,-h,d, // vertex 2
        -w,-h,-d, // vertex 3
        -w,-h,-d, // vertex 3
        w,-h,d, // vertex 4
        w,-h,-d  // vertex 1
    };

    GLfloat color_buffer_data [] = {
        color.r,color.g,color.b, // color 1
        color2.r,color2.g,color2.b, // color 2
        color3.r,color3.g,color3.b, // color 3

        color3.r,color3.g,color3.b, // color 4
        color4.r,color4.g,color4.b, // color 5
        color.r,color.g,color.b // color 6
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    VAO *tile = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
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
    InstanceGame.angle=angle;
    InstanceGame.status=status;
    InstanceGame.numx=numx;
    InstanceGame.numy=numy;
    if(component=="tile")
        tiles[name]=InstanceGame;
    else if(component=="background")
        background[name]=InstanceGame;
}

float camera_rotation_angle = 90;

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw (GLFWwindow* window,int doM, int doV, int doP)
{
    //int fbwidth, fbheight;
    //glfwGetFramebufferSize(window, &fbwidth, &fbheight);
    //glViewport((int)(x*fbwidth), (int)(y*fbheight), (int)(w*fbwidth), (int)(h*fbheight));


    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*sin(camera_rotation_angle*M_PI/180.0f), 3, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
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
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Load identity to model matrix
    Matrices.model = glm::mat4(1.0f);

    glm::mat4 translateRectangle = glm::translate (rect_pos);        // glTranslatef
    glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1));
    Matrices.model *= (translateRectangle * rotateRectangle);
    if(floor_rel)
    Matrices.model = Matrices.model * glm::translate(floor_pos);
    if(doM)
    MVP = VP * Matrices.model;
    else
    MVP = VP;

    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // draw3DObject draws the VAO given to it using current MVP matrix
    draw3DObject(rectangle);


    // Load identity to model matrix
    Matrices.model = glm::mat4(1.0f);

    glm::mat4 translateCam = glm::translate(eye);
    glm::mat4 rotateCam = glm::rotate((float)((90 - camera_rotation_angle)*M_PI/180.0f), glm::vec3(0,1,0));
    Matrices.model *= (translateCam * rotateCam);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // draw3DObject draws the VAO given to it using current MVP matrix
    draw3DObject(cam);
    Matrices.model = glm::translate(floor_pos);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // draw3DObject draws the VAO given to it using current MVP matrix
    //draw3DObject(floor_vao);

    for(map<string,Game>::iterator it=tiles.begin();it!=tiles.end();it++){
        string current = it->first; //The name of the current object
        glm::mat4 MVP;  // MVP = Projection * View * Model

        Matrices.model = glm::mat4(1.0f);

        glm::mat4 ObjectTransform;
        glm::mat4 translateObject = glm::translate (glm::vec3(tiles[current].x,tiles[current].y, tiles[current].z)); 
        glm::mat4 rotateTriangle = glm::rotate((float)((tiles[current].angle)*M_PI/180.0f), glm::vec3(0,0,1));// glTranslatef
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


    for(map<string,Game>::iterator it=cuboid.begin();it!=cuboid.end();it++){
        string current = it->first; //The name of the current object
        glm::mat4 MVP;  // MVP = Projection * View * Model

        Matrices.model = glm::mat4(1.0f);

        glm::mat4 ObjectTransform;
        glm::mat4 translateObject = glm::translate (glm::vec3(cuboid[current].x,cuboid[current].y, cuboid[current].z)); 
        glm::mat4 rotateTriangle = glm::rotate((float)((cuboid[current].angle)*M_PI/180.0f), glm::vec3(0,0,1));// glTranslatef
        ObjectTransform=translateObject*rotateTriangle;
        Matrices.model *= ObjectTransform;
        //MVP = VP * Matrices.model; // MVP = p * V * M
        if(doM)
            MVP = VP * Matrices.model;
        else
            MVP = VP;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

        draw3DObject(cuboid[current].object);
        //glPopMatrix (); 
    }

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
    //createRectangle ();
    createCam();

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    int k=0,i,j;
    for(i=0;i<10;i++)
    {

        for(j=0;j<10;j++)
        {
            if(gamemap[i][j]==1)
            {
    

                string c="tile";
                char d=k+'0';
                string e=c+d;
                if((i+j)%2==0) 
                    createtile(e,0,brown1,brown1,brown1,brown1,1.5*(i-3),0,1.5*(j-3),1,1.5,1.5,"tile",0,i,j);
                else
                    createtile(e,0,brown2,brown2,brown2,brown2,1.5*(i-3),0,1.5*(j-3),1,1.5,1.5,"tile",0,i,j);
                k++;
            }        
        }
    }

    createRectangle("cuboid",0,brown2,brown2,brown2,brown2,16,0.5,4,1.5,3,1.5,"cuboid",0,0,0);

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
    rect_pos = glm::vec3(0, 0, 0);
    floor_pos = glm::vec3(0, 0, 0);
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
    /*if(do_rot)
        camera_rotation_angle += 90*(current_time - last_update_time); // Simulating camera rotation
    if(camera_rotation_angle > 720)
        camera_rotation_angle -= 720;*/
    last_update_time = current_time;
    draw(window, 1, 1, 1);
    //draw(window, 0.5, 0, 0.5, 0.5, 0, 1, 1);
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
