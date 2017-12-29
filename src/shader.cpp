#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "shader.h"

Shader::Shader (const char *vshader, const char *fshader) {
  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vshader, std::ios::in);
  if (VertexShaderStream.is_open()) {
    std::string Line = "";
    while (getline(VertexShaderStream, Line)) {
      VertexShaderCode += "\n" + Line;
    }

    VertexShaderStream.close();
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fshader, std::ios::in);
  if (FragmentShaderStream.is_open()) {
    std::string Line = "";
    while (getline(FragmentShaderStream, Line)) {
      FragmentShaderCode += "\n" + Line;
    }

    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  char const *VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0){
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }

  // Compile Fragment Shader
  char const *FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }

  // Link the program
  programId = glCreateProgram();
  glAttachShader(programId, VertexShaderID);
  glAttachShader(programId, FragmentShaderID);
  glLinkProgram(programId);

  // Check the program
  glGetProgramiv(programId, GL_LINK_STATUS, &Result);
  glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }

  glDetachShader(programId, VertexShaderID);
  glDetachShader(programId, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  initUniforms();
}

Shader::~Shader () {
  glDeleteProgram(programId);
  programId = 0;
}

void Shader::addUniform (const char *name) {
  auto result = glGetUniformLocation(programId, name);
  if (result == -1) {
    result = 0;
    std::cout << "Error: " << glGetError() << std::endl;
  }

  uniforms[name] = result;
}

void Shader::initUniforms () {
  uniforms["MVP"] = glGetUniformLocation(programId, "MVP");
  uniforms["Model"] = glGetUniformLocation(programId, "Model");
  uniforms["View"] = glGetUniformLocation(programId, "View");
  uniforms["LightPosition"] = glGetUniformLocation(programId, "LightPosition");
  uniforms["textureSampler"] = glGetUniformLocation(programId, "textureSampler");
}

void Shader::use () {
  glUseProgram(programId);
}

void Shader::setMVP (const glm::mat4 &data) {
  glUniformMatrix4fv(uniforms["MVP"], 1, GL_FALSE, &data[0][0]);
}

void Shader::setModel (const glm::mat4 &data) {
  glUniformMatrix4fv(uniforms["Model"], 1, GL_FALSE, &data[0][0]);
}

void Shader::setView (const glm::mat4 &data) {
  glUniformMatrix4fv(uniforms["View"], 1, GL_FALSE, &data[0][0]);
}

void Shader::setLightPosition (const glm::vec3 &data) {
  glUniform3f(uniforms["LightPosition"], data.x, data.y, data.z);
}

void Shader::setTexture (const Texture *texture) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture->id());
  glUniform1i(uniforms["textureSampler"], 0);
}
