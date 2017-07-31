// copyright 2017 rapmocha

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

const GLuint load_shader(const std::string &file_path, GLenum shader_type) {
  // Read the Shader code from the file
  std::string source_str;
  std::ifstream ifs(file_path, std::ios::in);
  if (ifs.is_open()) {
    std::string line = "";
    while (getline(ifs, line)) source_str += "\n" + line;
    ifs.close();
  } else {
    std::cerr << "Failed to open the shader file: " << file_path << std::endl;
    return 0;
  }
  int info_log_length;
  const GLuint id = glCreateShader(shader_type);
  const char *src = source_str.c_str();
  glShaderSource(id, 1, &src , NULL);
  glCompileShader(id);
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::cout << "Failed to compile the shader: " << file_path << std::endl;
    std::vector<char> message(info_log_length+1);
    glGetShaderInfoLog(id, info_log_length, NULL, &message[0]);
    std::cout << &message[0] << std::endl;
  }
  return id;
}

GLuint create_program(const std::string vertex_file_path,
    const std::string fragment_file_path) {

  const GLuint vobj = load_shader(vertex_file_path, GL_VERTEX_SHADER);
  const GLuint fobj = load_shader(fragment_file_path, GL_FRAGMENT_SHADER);

  int info_log_length;
  // Link the program
  const GLuint program = glCreateProgram();
  glAttachShader(program, vobj);
  glAttachShader(program, fobj);
  glLinkProgram(program);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
  if ( info_log_length > 0 ) {
    std::vector<char> message(info_log_length+1);
    glGetProgramInfoLog(program, info_log_length, NULL, &message[0]);
    std::cout << &message[0] << std::endl;
  }

  glDetachShader(program, vobj);
  glDetachShader(program, fobj);

  glDeleteShader(vobj);
  glDeleteShader(fobj);

  return program;
}


int main() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return EXIT_FAILURE;
  }
  atexit(glfwTerminate);

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window;
  window = glfwCreateWindow(640, 480, "TITLE", NULL, NULL);
  if (!window) {
    std::cerr << "Falied to create GLFW window" << std::endl;
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Falled to initialize GLFW" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
  std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;
  std::cout << "GL_VENDER: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  GLuint program = create_program(
      "resources/simple_vertex_shader.vertexshader",
      "resources/simple_fragment_shader.fragmentshader");

  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  static const GLfloat vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
  };

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
      vertex_buffer_data, GL_STATIC_DRAW);

  // main loop
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &vertex_buffer);
  glDeleteVertexArrays(1, &vertex_array_id);
}

