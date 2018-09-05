//
//  main.c
//  gltut4
//
//  Created by Adenike Adeyemi on 9/3/18.
//  Copyright © 2018 Adenike Adeyemi. All rights reserved.
//
/**/
#define GLFW_INCLUDE_GLCOREARB
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include <glmc.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

void gl_sanity_test(void);

int main(int argc, const char * argv[]) {
    
    GLFWwindow *window;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint program;
    
    
    GLfloat square[] = {
        //Position          //Color
        -0.5, -0.5, 0.0,    1.0, 0.0, 0.0,
        -0.5,  0.5, 0.0,    0.0, 1.0, 0.0,
         0.5, -0.5, 0.0,    0.0, 0.0, 1.0,
         0.5,  0.5, 0.0,    0.0, 0.0, 0.0,
    };
    
    GLuint index[] = {
        0, 1, 2,
        1, 2 ,3,
    };
    
    if(!glfwInit()){
        printf("GLFW could not initialize.\n");
        exit(EXIT_FAILURE);
    }
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    window = glfwCreateWindow(400, 400, "Transformation", NULL, NULL);
    
    if(!window){
        printf("Window could not open.\n");
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("GLAD failed to load.\n");
        exit(EXIT_FAILURE);
    }
    
    //
    // insert code here...
    program = initShaderFiles("vshader.vsh", "fshader.fsh");
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    printf("2\n");
    gl_sanity_test();
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
    printf("1\n");
    gl_sanity_test();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    printf("3\n");
    gl_sanity_test();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glUseProgram(program);
    while (!glfwWindowShouldClose(window)) {
        
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void gl_sanity_test(){
    GLenum err;
    while((err = glGetError())!= GL_NO_ERROR){
        printf("Something broke. OpenGL err: %x\n", err);
    }
}
