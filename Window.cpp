#include "window.h"
#include "glm/gtx/vector_angle.hpp"
#include <time.h>


const char* window_title = "GLFW Starter Project";
GLint shaderProgram, skyboxShaderProgram, sphereShaderProgram, bezierShaderProgram, selectShaderProgram, envmapShaderProgram;


// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "./shader.vert"
#define FRAGMENT_SHADER_PATH "./shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 20.0f, 100.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

glm::vec3 cam_pos_backup(0.0f, 20.0f, 100.0f);
glm::vec3 cam_look_at_backup(0.0f, 0.0f, 0.0f);
glm::vec3 cam_up_backup(0.0f, 1.0f, 0.0f);

float cam_pos_scale = 1.0f;

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

//double Window::mouseX;
//double Window::mouseY;
glm::vec3 Window::lastPoint(0.0, 0.0, 0.0);
glm::vec3 lastPointTB(0.0, 0.0, 0.0);
bool Window::buttonPressed = false;
std::string Window::mode;
double Window::lastZ = 0.0f;

std::string target = "object";
std::string control = "object";

skybox* skybox;
Bezier* curve;
unsigned char pixel[4];
int ind = 0;
Sphere* sphereObj;
float velocity = 0.0;
bool isMove = true;

//double mouseX, mouseY;




void Window::initialize_objects()
{
    
    
//    // Skybox
//    std::vector<const GLchar*> faces;
//    faces.push_back("./skybox_texture/right.ppm");
//    faces.push_back("./skybox_texture/left.ppm");
//    faces.push_back("./skybox_texture/top.ppm");
//    faces.push_back("./skybox_texture/bottom.ppm");
//    faces.push_back("./skybox_texture/back.ppm");
//    faces.push_back("./skybox_texture/front.ppm");
//    skybox = new skybox::skybox(faces);
//    
//    // Bezier curve
//    curve = new Bezier();
//    
//    // Sphere ojbect
//    sphereObj = new Sphere(4, 12, 24);
//    glm::vec3 pos = curve->points[curve->t + 1];
//    sphereObj->toWorld = glm::translate(glm::mat4(1.0f), pos);
    
	// Load the shader program. Make sure you have the correct filepath up top
//	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
//    skyboxShaderProgram = LoadShaders("./skybox.vert", "./skybox.frag");
//    sphereShaderProgram = LoadShaders("./sphere.vert", "./sphere.frag");
//    bezierShaderProgram = LoadShaders("./bezier.vert", "./bezier.frag");
//    selectShaderProgram = LoadShaders("./selection.vert", "./selection.frag");
//    envmapShaderProgram = LoadShaders("./envMapping.vert", "./envMapping.frag");
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
    
    //frustumg = new FrustumG();
    
    
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);
    
    

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
    if(!isMove) return;
    
	// update position
    velocity += sqrt(curve->maxY + 0.1 - curve->points[velocity].y) / 3;
    if(velocity >= curve->points.size()) {
        velocity -= curve->points.size();
    }
    sphereObj->toWorld = glm::translate(glm::mat4(1.0f), curve->points[velocity]);
    
    
}

void Window::display_callback(GLFWwindow* window)
{
    
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Skybox
    //glUseProgram(skyboxShaderProgram);
    //skybox->draw(skyboxShaderProgram);
    
    // Bezier curve
    //glUseProgram(bezierShaderProgram);
    //curve->draw(bezierShaderProgram);
    
    // Use the shader of programID
	//glUseProgram(shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
    
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        
        // bear
        if (key == GLFW_KEY_F1)
        {
            //obj = bear;
        }
        // bunny
        if (key == GLFW_KEY_F2)
        {
            //obj = bunny;
        }
        // dragon
        if (key == GLFW_KEY_F3)
        {
            //obj = dragon;
        }
        
        // Move along x axis (x/X)
        if (key == GLFW_KEY_X && !(mods == GLFW_MOD_SHIFT))
        {
            
        }
        if (key == GLFW_KEY_X && mods == GLFW_MOD_SHIFT)
        {
            
        }
        
        // Move along y axis (y/Y)
        if (key == GLFW_KEY_Y && !(mods == GLFW_MOD_SHIFT))
        {
            
        }
        if (key == GLFW_KEY_Y && mods == GLFW_MOD_SHIFT)
        {
            
        }
        
        // Move along z axis (z/Z)
        if (key == GLFW_KEY_Z && !(mods == GLFW_MOD_SHIFT))
        {
            
        }
        if (key == GLFW_KEY_Z && mods == GLFW_MOD_SHIFT)
        {
            
        }
        
        // Scale (s/S)
        if (key == GLFW_KEY_S && !(mods == GLFW_MOD_SHIFT))
        {
            
        }
        if (key == GLFW_KEY_S && mods == GLFW_MOD_SHIFT)
        {
            
        }
        
        // Orbit about the z axis (o/O)
        if (key == GLFW_KEY_O && !(mods == GLFW_MOD_SHIFT))
        {
            
        }
        if (key == GLFW_KEY_O && mods == GLFW_MOD_SHIFT)
        {
            
        }
        
        // Reset position, orientation and size
        if (key == GLFW_KEY_R)
        {
            cam_pos = cam_pos_backup;
            cam_look_at = cam_look_at_backup;
            cam_up = cam_up_backup;
            Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
            velocity = curve->t + 1;
        }
        
        // Pause movement
        if (key == GLFW_KEY_P) {
            isMove = !isMove;
        }
        
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    double mouseX, mouseY;
    
    // Rotate
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        buttonPressed = true;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        //lastPoint = glm::vec3(mouseX, mouseY, 0.0f);
        lastPoint = trackBallMapping(mouseX, mouseY);
        mode = "left";
        //std::cout << "mouse pressed" << std::endl;
        
        //GLuint viewport[4];
        // Clear the color and depth buffers
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Skybox
        //glUseProgram(skyboxShaderProgram);
        //skybox->draw(skyboxShaderProgram);
        
        glUseProgram(selectShaderProgram);
        curve->draw(selectShaderProgram);
        //std::cout << mouseX << std::endl;
        //std::cout << height - mouseY << std::endl;
        glReadPixels(mouseX, height - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
        ind = pixel[0];
        //std::cout << ind << std::endl;
    }
    
    // Move
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        buttonPressed = true;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        lastPointTB = trackBallMapping(mouseX, mouseY);
        mode = "right";
        std::cout << "mouse pressed" << std::endl;
    }
    
    if (action == GLFW_RELEASE)
    {
        buttonPressed = false;
        mode = "default";
        std::cout << "mouse released" << std::endl;
        glUseProgram(bezierShaderProgram);
        curve->draw(bezierShaderProgram);
        ind = 0;
    }
}

void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    
}

void Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    if (yOffset < 0) {
        cam_pos_scale *= 0.9;
    }
    else {
        cam_pos_scale *= 1.1;
    }
    glm::vec3 cam_pos_tmp = cam_pos * cam_pos_scale;
    V = glm::lookAt(cam_pos * cam_pos_scale, cam_look_at, cam_up);
    //frustumg->setCamInternals(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
    //frustumg->setCamDef(cam_pos_tmp, cam_look_at, cam_up);
}

glm::vec3 Window::trackBallMapping(double x, double y) {
    glm::vec3 v;
    float d;
    v.x = (2.0f * x - width) / width;
    v.y = (height - 2.0f * y) / height;
    v.z = 0.0f;
    d = length(v);
    d = (d<1.0) ? d : 1.0;
    v.z = sqrtf(1.001 - d * d);
    v = glm::normalize(v); // Still need to normalize, since we only capped d, not v.
    return v;
}
