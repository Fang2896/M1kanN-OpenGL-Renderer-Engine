//
// Created by fangl on 2023/10/3.
//

#ifndef POST_PROCESS_SCREEN_HPP
#define POST_PROCESS_SCREEN_HPP

#include "gl_configure.hpp"


class PostProcessScreen {
   public:
    PostProcessScreen();
    ~PostProcessScreen();
    void init();
    void draw();

   private:
    GLFunctions_Core *glFunc;

    GLuint VAO;
    GLuint VBO;
};

#endif  //POST_PROCESS_SCREEN_HPP
