#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "OBJObject.h"
#include "skybox.h"
#include "Node.h"
#include "Group.h"
#include "Sphere.hpp"
#include "Geode.h"
#include "MatrixTransform.h"
#include "FrustumG.h"
#include "Bezier.h"
//#include <nanogui/nanogui.h>


class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
    //static double mouseX;
    //static double mouseY;
    static double lastZ;
    static glm::vec3 lastPoint;
    static bool buttonPressed;
    static std::string mode;
    static FrustumG* frustumg;
    static std::vector<Group*> army;
    static std::vector<glm::mat4> robotPos;
    
    
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow*, double, double);
    static glm::vec3 trackBallMapping(double, double);
    static void scroll_callback(GLFWwindow*, double, double);
    static void generateRobot(glm::mat4);
};

#endif
