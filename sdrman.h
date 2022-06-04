//
// shader manager
//

#pragma once
#ifndef SDRMAN_H
#define SDRMAN_H

#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

#define DEFAULT_VS_STR                                  \
    "#version 140 \n"                                   \
    "in vec3 in_Position;\n"                            \
    "in vec3 in_Color;\n"                               \
    "out vec3 fs_color;\n"                              \
    "void main(void) {\n"                               \
    "gl_Position = vec4(in_Position.xyz, 1.0);\n"       \
    "fs_color = in_Color;\n"                            \
    "}\n"

#define DEFAULT_FS_STR                          \
    "#version 140\n"                            \
    "in vec3 fs_color;\n"                       \
    "out vec4 out_Color;\n"                     \
    "void main(void) { \n"                      \
    "out_Color = vec4(fs_color, 1.0);\n"        \
    "}\n"


class sdrman {
public:
    sdrman();
    ~sdrman();

    void init(void);

    // set shader as file path name
    // OK return 0; error return < 0
    int setShaderFile(const char *pathname, GLenum shadertype);
    int setShaderStr(std::string shaderStr, GLenum shadertype);
    GLuint progId;


private:
    std::string _readfile(const char *filepath);
    GLuint vsId_;
    GLuint fsId_;
};




// Local Variables:
// mode: c++
// c-file-style: "cc-mode"
// End:

































#endif // SDRMAN_H

// Local Variables:
// mode: c++
// c-file-style: "cc-mode"
// End:
