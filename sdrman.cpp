#include "sdrman.h"

#include <iostream>
#include <fstream>
#include <sstream>

sdrman::sdrman():
    progId(0),vsId_(0),fsId_(0)
{    
}

sdrman::~sdrman()
{
    glDeleteProgram(progId);
    glDeleteShader(vsId_);
    glDeleteShader(fsId_);
}

void sdrman::init()
{
    progId = glCreateProgram();
    setShaderStr(std::string(DEFAULT_VS_STR), GL_VERTEX_SHADER);
    setShaderStr(std::string(DEFAULT_FS_STR), GL_FRAGMENT_SHADER);
}

int sdrman::setShaderStr(std::string shaderStr, GLenum shadertype)
{
    GLuint activeId = 0;
    int logLength;
    GLint result = GL_FALSE;

    if (shaderStr.empty())
        return -1;

    switch (shadertype) {
    case GL_VERTEX_SHADER: {
        glDeleteShader(vsId_);
        vsId_ = glCreateShader(GL_VERTEX_SHADER);
        const char *s = shaderStr.c_str();
        glShaderSource(vsId_, 1, &s, NULL);
        glCompileShader(vsId_);
        glAttachShader(progId, vsId_);
        activeId = vsId_;
    }
        break;
    case GL_FRAGMENT_SHADER: {
        glDeleteShader(fsId_);
        fsId_ = glCreateShader(GL_FRAGMENT_SHADER);
        const char *s = shaderStr.c_str();
        glShaderSource(fsId_, 1, &s, NULL);
        glCompileShader(fsId_);
        glAttachShader(progId, fsId_);
        activeId = fsId_;
    }
        break;
    default:
        std::cerr << "unknow shader type" << std::endl;
        return -1;
    }
    
    glGetShaderiv(activeId, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        glGetShaderiv(activeId, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar* strInfoLog = new GLchar[logLength + 1];
            glGetShaderInfoLog(activeId, logLength, NULL, strInfoLog);
            std::cerr << strInfoLog << std::endl;
            delete[] strInfoLog;
        }
        return -1;
    }
    return 0;
}

int sdrman::setShaderFile(const char *pathname, GLenum shadertype)
{
    std::string shaderSrc = _readfile(pathname);
    if (shaderSrc.empty())
        return -1;

    return setShaderStr(shaderSrc, shadertype);
}


std::string sdrman::_readfile(const char *filepath)
{
    std::ostringstream os;
    std::ifstream input_file((std::string(filepath)));

    if (!input_file.is_open()) {
        std::cerr << "Can't open file: " << filepath << std::endl;
        return std::string("");
    }

    os << input_file.rdbuf();
    return os.str();
}



// Local Variables:
// mode: c++
// c-file-style: "cc-mode"
// End:
