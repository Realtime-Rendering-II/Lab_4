#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "../include/ImGui/imgui.h"
#include "../include/ImGui/imgui_impl_glfw.h"
#include "../include/ImGui/imgui_impl_opengl3.h"

#include "../include/obj_loader.h"
#include "../include/shader.h"
#include "../include/texture.h"
#include "../include/camera.h"
#include "../include/c3dobject.hpp"

int width = 800, height = 600;
float last_x, last_y, delta_time = 0.0f, last_frame_time = 0.0f;;
bool occlusion_culling = false, wireframe = false, first_mouse = false;

Camera *camera = new Camera{glm::vec3{3, 3.5, 15}, glm::vec3{0,1,0}};
GLFWwindow *window;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow *window, double x, double y);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void uploadMatrices(GLuint shader);

void initialize();

int main(int argc, const char *argv[]) {

    initialize();

    Shader normal_shader = Shader{"../shader/textured.vert", "../shader/textured.frag"};

    // TODO 1: fill this vector with C3DObject's that use the low_poly_sphere.obj and the earthmap1k texture
    // NOTE: the objects should be contained in a "cube".
    // The dimension variable should specify how many spheres are inside the cube (number of spheres = dimension^3)
    std::vector<C3DObject> c3dobjects;
    const GLuint dimension = 3;

    // TODO 2: generate a query for the occlusion test and a query to measure the elapsed time
    GLuint occlusion_query, time_query;

    // NOTE: use this variable to count how many sphere had to be drawn
    int total_visible = 0;

    // the main loop where the object are drawn
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        // calculate the time it took to compute the new frame to move the camera in a smoother way depending on the framerate
        delta_time = currentFrame - last_frame_time;
        last_frame_time = currentFrame;
        glfwGetFramebufferSize(window, &width, &height);

        // specify the background color
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        // clear color, depth and stencil buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        uploadMatrices(normal_shader.get_program());

        // TODO 3: compute for each object in the c3dobjects the distance to the camera
        // NOTE: the distance to the camera is the shortest distance between the bounding box vertices and the camera

        // TODO 4: sort the objects in the c3dobjects vector according to their distance to the camera

        // reset the variable to only consider draw calls from this frame
        total_visible = 0;

        // TODO 5: begin a query measuring the elapsed time

        // TODO 6: If occlusion culling is deactivated draw all c3dobjects vector.
        // TODO 6: If the occlusion culling is activated test for each sphere if its bounding box is visible before you draw it
        // Note: you can modify the value of the variable occlusion_culling by pressing the key "0"
        // Note: draw for each object first the bounding box and see if it is visible and afterwards draw the object itself if the bounding box was visible
        // Note: if you are in front of a sphere and no other sphere can be seen the number of draw calls should be 1

        // TODO 7: end the query measuring the time, receive the value and transform it into a float that resembles the time in ms

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize({350,100});
        ImGui::Begin("Statistics");
        // TODO 8: display the necessary information (is culling activated?, how many spheres are visible?, how much time did the draw calls take?) using ImGui::Text
        ImGui::Text("Example text with an example number %d", 10);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // unbinding shader program
    glUseProgram(NULL);
    // window is no longer required
    glfwDestroyWindow(window);
    // finish glfw
    glfwTerminate();
    return 0;
}

// this function is called when a key is pressed
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // if the escape key is pressed the window will close
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // if this key is pressed the cube should start rotating around the y-axis
    if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
        occlusion_culling = !occlusion_culling;
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (wireframe) {
            wireframe = false;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            wireframe = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }

    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(FORWARD, delta_time * 50);
        }

    }

    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(BACKWARD, delta_time * 50);
        }
    }
    //left and right
    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(RIGHT, delta_time * 50);
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            camera->ProcessKeyboard(LEFT, delta_time * 50);
        }
    }
}

void mouse_callback(GLFWwindow *window, double x, double y) {
    if (first_mouse) {
        last_x = x;
        last_y = y;
        first_mouse = false;
    }

    float xoffset = x - last_x;
    float yoffset = last_y - y; // reversed since y-coordinates go from bottom to top

    glfwSetCursorPos(window, last_x, last_y);
    camera->ProcessMouseMovement(xoffset, yoffset);
}

// this function is called when the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// this function uploads the model, view and projection matrix to the shader if they are defined in the shader
void uploadMatrices(GLuint shader) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), width / (float) height, 0.1f, 10000.0f);

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);
}

void initialize() {
    // initialize the GLFW library to be able create a window
    if (!glfwInit()) {
        throw std::runtime_error("Couldn't init GLFW");
    }

    // set the opengl version
    int major = 3;
    int minor = 3;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // create the window
    window = glfwCreateWindow(width, height, "Lab 4", NULL, NULL);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Couldn't create a window");
    }

    // set the window to the current context so that it is used
    glfwMakeContextCurrent(window);
    // set the frameBufferSizeCallback so that the window adjusts if it is scaled
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // set the keyCallback function so that keyboard input can be used
    glfwSetKeyCallback(window, key_callback);
    // set the mouse callback so that mouse input can be used
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // try to initialise glew to be able to use opengl commands
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error(
                std::string("Could initialize GLEW, error = ") + (const char *) glewGetErrorString(err));
    }

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;


    // opengl configuration
    glEnable(GL_DEPTH_TEST);        // enable depth-testing
    glDepthFunc(GL_LESS);           // depth-testing interprets a smaller value as "closer"
    glfwSwapInterval(false); // disables VSYNC

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    const char* glsl_version = "#version 130";
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}