#include <getopt.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#ifdef OS_DARWIN
#include <gl.h>
#else
#include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>

#include "sdrman.h"

std::string appTitle = "Mad Shader";

static struct option longopts[] = {
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
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
int win_width = 1024, win_height = 768;


int
main(int argc, char *argv[])
{
    int ch;
    GLFWwindow *mainwin;
    double thisFrameTimeStamp, lastFrameTimeStamp;
    uint64_t frameCount = 0;
    uint64_t iFrameCount = 0;
    sdrman sm;
    
    while ((ch = getopt_long(argc, argv, "w:h:v:f:t:m:", longopts, NULL)) != -1) {
        switch (ch) {
        case 'w':
            win_width = atoi(optarg);
            break;
        case 'h':
            win_height = atoi(optarg);
            break;
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

    mainwin = glfwCreateWindow(win_width, win_height, appTitle.c_str(), NULL, NULL);
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
    glfwSwapInterval(0);
    set_default_vertex_attr();
    glViewport(0, 0, win_width, win_height);
    glClearColor(0, 0, 0, 1);

    lastFrameTimeStamp = glfwGetTime();

    sm.init();
    
    // main loop
    do {
        frameCount++;
        iFrameCount++;
        thisFrameTimeStamp = glfwGetTime();
        if (thisFrameTimeStamp - lastFrameTimeStamp >= 1.0) {
            std::string t;
            lastFrameTimeStamp = thisFrameTimeStamp;
            t = appTitle + " ( ";
            t += std::to_string(frameCount);
            t += " )";
            glfwSetWindowTitle(mainwin, t.c_str());
            frameCount = 0;
        }
      
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClear(GL_COLOR_BUFFER_BIT);

        // iTime: this frame time stamp
        GLint iTime = glGetUniformLocation(sm.progId, "iTime");
        glUniform1f(iTime, (float) thisFrameTimeStamp);

        // iResolution: window width and height
        GLint iResolution = glGetUniformLocation(sm.progId, "iResolution");
        glfwGetFramebufferSize(mainwin, &win_width, &win_height);
        glUniform3f(iResolution, win_width, win_height, 0.0f);

        // iFrame: the frame counter from start
        GLint iFrame = glGetUniformLocation(sm.progId, "iFrame");
        glUniform1f(iFrame, (float) iFrameCount);
        
        
        glLinkProgram(sm.progId);
        glUseProgram(sm.progId);


        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


        
        glfwSwapBuffers(mainwin);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(mainwin));

    //Close OpenGL window and terminate GLFW
    glfwDestroyWindow(mainwin);
    //Finalize and clean up GLFW
    glfwTerminate();

    return 0;
}

// Local Variables:
// mode: c++
// c-file-style: "cc-mode"
// End:

