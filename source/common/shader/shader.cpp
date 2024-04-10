#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

//Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const {
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCStr = sourceString.c_str();
    file.close();

    //DONE Complete this function
    //Note: The function "checkForShaderCompilationErrors" checks if there is
    // an error in the given shader. You should use it to check if there is a
    // compilation error and print it so that you can know what is wrong with
    // the shader. The returned string will be empty if there is no errors.

    //Create a shader object
    GLuint shader = glCreateShader(type);
    //Attach the GLSL code to the shader
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    //Compile the shader
    glCompileShader(shader);

    std::string error = checkForShaderCompilationErrors(shader);
    // Check if there is a compilation error and print it so that you can know what is wrong with the shader
    if (error != "") {
        std::cerr << "ERROR: Shader compilation failed with the following error: " << error << std::endl;
        return false;
    }

    //Attach the shader to the program
    glAttachShader(program, shader);
    //Delete the shader (it won't actually be deleted until the program that it's attached to has been destroyed)
    glDeleteShader(shader);
    //We return true if the compilation succeeded
    return true;
}



bool our::ShaderProgram::link() const {
    //DONE Complete this function
    //Note: The function "checkForLinkingErrors" checks if there is
    // an error in the given program. You should use it to check if there is a
    // linking error and print it so that you can know what is wrong with the
    // program. The returned string will be empty if there is no errors.

    //Link the program
    glLinkProgram(program);

    std::string error = checkForLinkingErrors(program);
    // Check if there is a linking error and print it so that you can know what is wrong with the program
    if (error != "") {
        std::cerr << "ERROR: Shader linking failed with the following error: " << error << std::endl;
        return false;
    }
    //We return true if the compilation succeeded
    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader){
     //Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program){
     //Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}