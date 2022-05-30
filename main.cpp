#include <getopt.h>
#include <string>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <gl.h>
#include <GLFW/glfw3.h>


static struct option longopts[] = {
    {"vs", required_argument, NULL, 'v'}, // vertex shader file
    {"fs", required_argument, NULL, 'f'}, // fragment shader file
    {"tex", required_argument, NULL, 't'}, // texture file (image file)
    {"mod", required_argument, NULL, 'm'}, // model file
    

    
};

static void glfw_error_callback(int error, const char* desc)
{
    std::cerr << desc << std::endl;
}

static void win_resize_callback(GLFWwindow *win, int w, int h)
{
    glViewport(0, 0, w, h);
}

static void set_default_vertex_attr()
{
    GLuint VBO[2];
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    const GLfloat posBuffer[] = {
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f,  -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    // Color data
    const GLfloat colBuffer[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
    };

    glGenBuffers(2, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posBuffer), posBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colBuffer), colBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

}

std::vector<std::string> fs_shader;
std::vector<std::string> tex_images;

int main(int argc, char *argv[])
{
    int ch;
    GLFWwindow *mainwin;
    
    while ((ch = getopt_long(argc, argv, "v:f:t:m:", longopts, NULL)) != -1) {
        switch (ch) {
        case 'f':
            break;
        case 't':
            break;
        default:
            break;
        }
    }

    // if (fs_shader.size() == 0) {
    //     std::cerr << "need --fs [fragment shaders]" << std::endl;
    //     return -1;
    // }

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "GLFW3 init error!" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef OS_DARWIN
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mainwin = glfwCreateWindow(1024, 768, "Mad Shader", NULL, NULL);
	if (!mainwin) {
		std::cerr << "can't create GLFW window!" << std::endl;
		return -1;
	}
	
    glfwMakeContextCurrent(mainwin);
    // glfwSetKeyCallback(window, key_callback);
    // glfwSetCursorPosCallback(window, cursor_position_callback);
    // glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(mainwin, win_resize_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "can't init GLEW!" << std::endl;
        return -1;
    }

    set_default_vertex_attr();
    glViewport(0, 0, 1024, 768);
    glClearColor(0, 0, 0, 1);
    // main loop
    do {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(mainwin);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(mainwin));

    //Close OpenGL window and terminate GLFW
    glfwDestroyWindow(mainwin);
    //Finalize and clean up GLFW
    glfwTerminate();

    return 0;
}
