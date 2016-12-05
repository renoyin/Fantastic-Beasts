#include "window.h"
#include "glm/gtx/vector_angle.hpp"
#include <time.h>

using namespace std;

const char* window_title = "GLFW Starter Project";
GLint shaderProgram, skyboxShaderProgram, sphereShaderProgram, bezierShaderProgram, selectShaderProgram, envmapShaderProgram, gameboxShaderProgram, lightShaderProgram, depthShaderProgram;


// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "./shader.vert"
#define FRAGMENT_SHADER_PATH "./shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 70.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);         // up | What orientation "up" is
glm::vec3 cam_front(0.0f, 0.0f, -1.0f);

glm::vec3 cam_pos_backup(0.0f, 0.0f, 70.0f);
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
FrustumG* walls;
skybox* skybox;
Bezier* curve;
Cube* gameBox;
Cube* lightBox;
//Cube* cubeObj;
//Sphere* outBound;
vector<Geode*> obstacleList;
vector<Cube*> outBoundList;
vector<vec3> obstaclePosList;
Group* cubeGroup;
Group* boundGroup;
unsigned char pixel[4];
int ind = 0;
Sphere* sphereObj;
float velocity = 0.0;
bool isMove = true;

//double mouseX, mouseY;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// For mouse movement
bool firstMove = true;
GLfloat cam_yaw = 0.0f, cam_pitch = 0.0f;
GLfloat lastX = Window::width / 2, lastY = Window::height / 2;
GLfloat aspect = 45.0f;
bool keys[1024];

// light
glm::vec3 dirLightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
glm::vec3 pointLightPosition = glm::vec3(0.0f,  5.0f,  0.0f);

// Shadow mapping
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
GLuint depthMapFBO;
GLuint depthMap;
GLuint planeVAO, planeVBO;

//sphereObj move
vec3 Window::direction(-1,-2, 4);
//vec3 direction = Window::randomPos();
mat4 translateM = mat4(1.0f); //glm::translate(mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)) * ;
vec3 spherePos(0,0,0);
float sphereRadius = 4;
float obstacleRadius = 2;
float speed = 1.0f;
bool eliminate = false;
int lastHitWall = -1;

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
    // FrustumG object six planes of gameBox
    walls = new FrustumG();
    walls->setCubePlanes(40);
    // Sphere ojbect
    sphereObj = new Sphere(sphereRadius, 12, 24);
    
    // Game box
    gameBox = new Cube();
    
    // light
    lightBox = new Cube();
    
    //cube object
    for(int i=0; i<5; i++) {
        Cube* cubeObj = new Cube();
        Sphere* obstacle= new Sphere(obstacleRadius, 12, 24);
        vec3 newpos = randomPos();
        obstacle->toWorld = translate(mat4(1.0f), newpos);
        obstacle->solid = true;
        obstacle->ifDraw = true;
        obstacleList.push_back(obstacle);
        outBoundList.push_back(cubeObj);
        obstaclePosList.push_back(newpos);
    }
    
	// Load the shader program. Make sure you have the correct filepath up top
    skyboxShaderProgram = LoadShaders("./skybox.vert", "./skybox.frag");
    sphereShaderProgram = LoadShaders("./sphere.vert", "./sphere.frag");
    lightShaderProgram = LoadShaders("./light.vert", "./light.frag");
    gameboxShaderProgram = LoadShaders("./gamebox.vert", "./gamebox.frag");
    depthShaderProgram = LoadShaders("./depthShader.vert", "./depthShader.frag");

    
    
    // Plane
    GLfloat planeVertices[] = {
        // Positions          // Normals         // Texture Coords
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
        -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
        - 25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
    };
    // Setup plane VAO
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glBindVertexArray(0);
    
    
    // Create depth buffer
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
    delete(sphereObj);
    delete(skybox);
    delete(curve);
    delete(walls);
    delete(gameBox);
    delete(lightBox);
    glDeleteProgram(skyboxShaderProgram);
	glDeleteProgram(sphereShaderProgram);
    glDeleteProgram(gameboxShaderProgram);
    glDeleteProgram(lightShaderProgram);
    glDeleteProgram(depthShaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
    Window::width = width;
    Window::height = height;
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
		V = glm::lookAt(cam_pos, cam_front + cam_pos, cam_up);
	}
}

void Window::idle_callback()
{

}

void Window::display_callback(GLFWwindow* window)
{
    do_movement();
    moveSphereObj();
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
	// Clear the color and depth buffers
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Skybox
    //glUseProgram(skyboxShaderProgram);
    //skybox->draw(skyboxShaderProgram);
    
    // Bezier curve
    //glUseProgram(bezierShaderProgram);
    //curve->draw(bezierShaderProgram);
    
    // Sphere
    glUseProgram(lightShaderProgram);
    //viewpos
    glUniform3f(glGetUniformLocation(lightShaderProgram, "viewPos"), cam_pos.x, cam_pos.y, cam_pos.z);
    //dirLight
    glUniform3f(glGetUniformLocation(lightShaderProgram, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "dirLight.specular"), 0.2f, 0.5f, 0.9f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "dirLight.direction"), dirLightDirection.x, dirLightDirection.y, dirLightDirection.z);
    //pointLight
    glUniform3f(glGetUniformLocation(lightShaderProgram, "pointLight.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "pointLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "pointLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(lightShaderProgram, "pointLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightShaderProgram, "pointLight.linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightShaderProgram, "pointLight.quadratic"), 0.032);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "pointLight.position"), pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);
    // gold
    glUniform1f(glGetUniformLocation(lightShaderProgram, "material.shininess"), 128.0f * 0.4f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "material.ambient"), 0.24725f, 0.1995f, 0.0745f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "material.diffuse"), 0.75164f, 0.60648f, 0.22648f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "material.specular"), 0.628281f, 0.555802f, 0.366065f);
    // Mode -> point light
    glUniform1i(glGetUniformLocation(lightShaderProgram, "mode"), 2);
    
    //glUniform3fv(glGetUniformLocation(envmapShaderProgram, "cameraPos"), 1, &cam_pos[0]);
    //glUniform1i(glGetUniformLocation(envmapShaderProgram, "skybox"), 0);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);
    sphereObj->draw(lightShaderProgram, glm::mat4(1.0f));
    //glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    glUseProgram(lightShaderProgram);
    //cube color
    glUniform1f(glGetUniformLocation(lightShaderProgram, "material.shininess"), 0.6f*128);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "material.ambient"), 0.1745f, 0.01175f, 0.01175f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "material.diffuse"), 0.61424f, 0.04136f, 0.04136f);
    glUniform3f(glGetUniformLocation(lightShaderProgram, "material.specular"), 0.727811f, 0.626959f, 0.626959f);
    glUniform1i(glGetUniformLocation(lightShaderProgram, "mode"), 2);

    //check collision and delete collide cube, then generate a new one in random position
    unordered_set<int> collisionList = checkCollision();
   

    //draw obstacle
    for(int i=0; i<obstaclePosList.size(); i++) {
        //cout<< cubePosList[i].x <<","<<cubePosList[i].y<<","<<cubePosList[i].z << endl;
        //outBoundList[i]->color =vec3(1.0f,0.0f,0.0f);
        obstacleList[i]->draw(lightShaderProgram, mat4(1.0f));
    }
    
    //draw the wire frame
   // glUseProgram(sphereShaderProgram);
    glUseProgram(gameboxShaderProgram);
    for(int i=0; i<obstaclePosList.size(); i++) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(collisionList.find(i)== collisionList.end()) {
            glm::vec3 frameColor = glm::vec3(0.0,0.0,0.0);
            glUniform3fv(glGetUniformLocation(gameboxShaderProgram, "Color"), 1, &frameColor.x);
            outBoundList[i]->drawFrame(gameboxShaderProgram, translate(mat4(1.0f),obstaclePosList[i]));
        }
        else {
            glm::vec3 frameColor = glm::vec3(1.0f,0.0f,0.0f);
            glUniform3fv(glGetUniformLocation(gameboxShaderProgram, "Color"), 1, &frameColor.x);
            outBoundList[i]->drawFrame(gameboxShaderProgram, translate(mat4(1.0f),obstaclePosList[i]));
        }
    }
    
    //eliminate mode
    if(eliminate) {
        for (auto itr = collisionList.begin(); itr != collisionList.end(); ++itr) {
            //cout<< *itr << endl;
            vec3 newpos = randomPos();
            obstacleList[*itr]->toWorld = translate(mat4(1.0f), newpos);
            obstaclePosList[*itr] = newpos;
    
        }
    }
    // Shadow mapping
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    GLfloat near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(pointLightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    // - render scene from light's point of view
    glUseProgram(depthShaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(depthShaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    //RenderScene(depthShaderProgram);
    // Floor
    glm::mat4 model;
    glUniformMatrix4fv(glGetUniformLocation(depthShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Reset viewport
    glViewport(0, 0, Window::width, Window::height);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    
    // Light box at (0,22,0)
    glUseProgram(gameboxShaderProgram);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glUniform3fv(glGetUniformLocation(gameboxShaderProgram, "Color"), 1, &lightColor.x);
    gameBox->draw(gameboxShaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 22.0f, 0.0f)));
    
    
    // Game box
    glUseProgram(gameboxShaderProgram);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::vec3 frameColor = glm::vec3(0.196f, 0.714f, 0.576f);
    glUniform3fv(glGetUniformLocation(gameboxShaderProgram, "Color"), 1, &frameColor.x);
    gameBox->drawFrame(gameboxShaderProgram, glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
    
    //std::cout << glm::to_string(cam_pos) << std::endl;
    V = glm::lookAt(cam_pos, cam_front + cam_pos, cam_up);
    
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if(action == GLFW_PRESS) {
        keys[key] = true;
        if(key == GLFW_KEY_B) {
            for(int i=0; i<outBoundList.size(); i++) {
                outBoundList[i]->ifDraw = !outBoundList[i]->ifDraw;
            }
        }
        if(key == GLFW_KEY_C) {
            direction = Window::randomPos();
        }
        if(key == GLFW_KEY_E) {
            eliminate = !eliminate;
        }
    }
    
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    
}

void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    
    if(firstMove) {
        lastX = xpos;
        lastY = ypos;
        firstMove = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    GLfloat sensitivity = 0.5;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    cam_yaw += xoffset;
    cam_pitch +=yoffset;
    
    if(cam_pitch > 89.0f) cam_pitch = 89.0f;
    if(cam_pitch < -89.0f) cam_pitch = -89.0f;
    
    glm::vec3 front;
    front.x = sin(glm::radians(cam_yaw)) * cos(glm::radians(cam_pitch));
    front.y = sin(glm::radians(cam_pitch));
    front.z = -cos(glm::radians(cam_yaw)) * cos(glm::radians(cam_pitch));
    cam_front = glm::normalize(front);
    V = glm::lookAt(cam_pos, cam_front + cam_pos, cam_up);
}

void Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{

}

void Window::do_movement() {
    GLfloat cameraSpeed = 20.0f * deltaTime;
    if(keys[GLFW_KEY_W])
        cam_pos += cameraSpeed * cam_front;
    else if(keys[GLFW_KEY_S])
        cam_pos -= cameraSpeed * cam_front;
    else if(keys[GLFW_KEY_A])
        cam_pos -= glm::normalize(glm::cross(cam_front, cam_up)) * cameraSpeed;
    else if(keys[GLFW_KEY_D])
        cam_pos += glm::normalize(glm::cross(cam_front, cam_up)) * cameraSpeed;
    
}

unordered_set<int> Window::checkCollision() {
    unordered_set<int> res;
    for(int i=0; i<obstaclePosList.size(); i++) {
        float r = sphereRadius + obstacleRadius;
        float dis = length(spherePos - obstaclePosList[i]);
        //float angle = acos(dot(normalize(direction), pl[curWall].normal))/3.14*180;
        float angle = acos(dot(normalize(direction), normalize(spherePos-obstaclePosList[i])))/3.14*180;
        if (dis < r && angle>90) {
            
                vec3 norm = spherePos - obstaclePosList[i];
                vec3 I = normalize(direction);
                vec3 R = reflect(I, normalize(norm));
                direction = R;
                res.insert(i);
        }
    }
    return res;
    
}
vec3 Window::randomPos() {
    vec3 res(0);
    vector<double> ran;
    for(int k=0; k<3 ;k++) {
        double i = 0, d = 0;
        i = rand() % 40 - 20; //Gives a number between -20 and +20;
        d = i*16/20;
        ran.push_back(d);
    }
    res.x = ran[0];
    res.y = ran[1];
    res.z = ran[2];
    
    return res;
}

void Window::moveSphereObj() {
    int hitWall = walls->ballHitWall(spherePos, sphereRadius, direction);
    //int threshold = 3.0f;
    
    if(hitWall != -1) {
        cout<< "current hit" << hitWall << endl;
        direction = walls->reflection(direction, hitWall);
    }
    
    translateM = glm::translate(mat4(1.0f), normalize(direction)*speed);
    sphereObj->toWorld = translateM * sphereObj->toWorld;
    vec4 result = translateM * vec4(spherePos,1.0);
    spherePos = vec3(result.x, result.y, result.z);
    
    //follow ball view angle
//    vec4 cam_result = translateM*vec4(cam_pos,1.0);
//    cam_pos = normalize(spherePos);
//    vec3 z = normalize(cam_pos);
//    vec3 x = cross(normalize(cam_up), z);
//    cam_up = cross(z,x);
//    cam_look_at = spherePos + direction;
//    Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    
    
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
