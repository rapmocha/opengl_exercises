// copyright 2017 rapmocha

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

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

GLuint load_program(const std::string vertex_file_path,
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
  glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

  GLuint program = load_program(
      "resources/simple_vertex_shader.vertexshader",
      "resources/simple_fragment_shader.fragmentshader");

  static const GLfloat vertex_buffer_data[] = {
    -0.5f,-0.5f,-0.5f, // 三角形1:開始
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // 三角形1:終了
     0.5f, 0.5f,-0.5f, // 三角形2:開始
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // 三角形2:終了
     0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
     0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
     0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
     0.5f,-0.5f, 0.5f
  };

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  GLuint color_buffer;
  glGenBuffers(1, &color_buffer);
  GLfloat color_buffer_data[12*3*3];
  for (int v = 0; v < 12*3 ; v++){
    color_buffer_data[3*v+0] = v / static_cast<double>(12*3);
    color_buffer_data[3*v+1] = v / static_cast<double>(12*3);
    color_buffer_data[3*v+2] = v / static_cast<double>(12*3);
  }
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data),
      color_buffer_data, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glm::mat4 model = glm::mat4();
  glm::mat4 view = glm::lookAt(
      glm::vec3(4, 3, -3),
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1, 0));
  glm::mat4 projection =
    glm::perspective(glm::radians(30.0), 4.0/3.0, 0.1, 10.0);
  glm::mat4 mvp = projection * view * model;
  GLuint mvp_id = glGetUniformLocation(program, "mvp");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  int timer = 0;

  // main loop
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);

    // vertex
    glEnableVertexAttribArray(0);
    timer++;
    if (timer > 6) {
        timer = 0;
        model = glm::rotate(glm::radians(5.0f), glm::vec3(0,1,0)) * model;
        mvp = projection * view * model;
    }
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
        vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // color
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3*12);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &vertex_buffer);
  glDeleteVertexArrays(1, &vertex_array_id);
}

