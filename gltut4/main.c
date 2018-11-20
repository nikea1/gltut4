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

#include <glmc.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glmc.h>

#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//create a function that converts double matrix into float matrix.
void toFloatMat4(double[], float[]);
void gl_sanity_test(void);
void mat4_x_vec4(t_mat4, t_vec4, t_vec4);
void mat4_x_mat4(t_mat4, t_mat4, t_mat4);
void print_4x4_matrix(t_mat4 matrix);
double degree_to_radian(double angle_in_degrees);
void test(void);
void quat_mat4(t_vec3, double, t_mat4);
void copyMat4(t_mat4 in, t_mat4 out);

char * VSHADER = "#version 330\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
""
"out vec3 myColor;\n"
"out vec2 myTexCoord;\n"
"uniform mat4 transform;\n"
"void main(){\n"
"    gl_Position =  transform * vec4(aPos, 1.0f);\n"
"    myColor = aColor;\n"
"    myTexCoord = aTexCoord;\n"
"}\n";

char * FSHADER ="#version 330\n"

"out vec4 FragColor;\n"

"in vec3 myColor;\n"
"in vec2 myTexCoord;\n"

"uniform sampler2D texture0;\n"
"uniform sampler2D texture1;\n"

"void main(){\n"
    //FragColor = vec4(myColor, 1.0);
    //FragColor = texture(texture0, myTexCoord);
"    FragColor = mix(texture(texture0, myTexCoord), texture(texture1, myTexCoord), 0.2);\n"
"}\n";

int main(int argc, const char * argv[]) {
    
    GLFWwindow *window;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint tex, tex1;
    
    unsigned int transLoc;
    
    //matrix variables
   
    
    
    unsigned char *img;
    int width, height, nrChannel;
    GLuint program;
    
    float out[16]; //output to shader
    t_mat4 temp; //temp matrix
    t_vec4 vec ={1.0, 0.0, 0.0, 1.0};
    t_vec4 v_out;
    t_vec3 vec2 = {1.0, 1.0, 0.0};
    t_vec3 axis = {0.0, 0.0, 1.0};
    t_vec3 scale = {0.5, 0.5, 0.5};
    t_mat4 trans;
    glmc_identity(trans);
    glmc_translate(trans, vec2, temp);
    copyMat4(temp, trans);
    mat4_x_vec4(trans, vec, v_out);
    
    
    printf("X: %lf, Y: %lf, Z: %lf, W: %lf\n", v_out[0], v_out[1], v_out[2], v_out[3]);
    
    glmc_identity(trans);
    glmc_rotate(trans, degree_to_radian(90.0), axis, temp);
    copyMat4(temp, trans);
    glmc_scale(trans, scale, temp);
    copyMat4(temp, trans);
    
    toFloatMat4(trans, out);
    
    
    GLfloat square[] = {
        //Position          //Color         //Texture
        -0.5, -0.5, 0.0,    1.0, 0.0, 0.0,  0.0, 0.0,
        -0.5,  0.5, 0.0,    0.0, 1.0, 0.0,  0.0, 1.0,
         0.5, -0.5, 0.0,    0.0, 0.0, 1.0,  1.0, 0.0,
         0.5,  0.5, 0.0,    0.0, 0.0, 0.0,  1.0, 1.0
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
    //program = initShaderFiles("vshader.vsh", "fshader.fsh");
    program = initShaders(VSHADER, FSHADER);
    //-----------------------------------------------------//
    //Matrix Transformations                               //
    //-----------------------------------------------------//
    
    glUseProgram(program); //must call this before getting any uniform location
    
    
    transLoc = glGetUniformLocation(program, "transform");
    
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, out);
    printf("1\n");
    gl_sanity_test();
    //-----------------------------------------------------//
    //Texture Data                                         //
    //-----------------------------------------------------//
    
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    img = stbi_load("container.jpg", &width, &height, &nrChannel, 0);
    if(img){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(img);
    }
    else{
        printf("No image.\n");
        exit(EXIT_FAILURE);
    }
    
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &tex1);
    
    glBindTexture(GL_TEXTURE_2D, tex1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(1);
    img = stbi_load("awesomeface.png", &width, &height, &nrChannel, 0);
    if(img){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(img);
    }
    else{
        printf("No image.\n");
        exit(EXIT_FAILURE);
    }
    
    //------------------------
    //Buffers
    //------------------------
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
    
    //----------------------------
    //Setting Attribute locations
    //----------------------------
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    
    //glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "texture0"), 0);
    
    glUniform1i(glGetUniformLocation(program, "texture1"), 1);
    printf("3\n");
    gl_sanity_test();
    while (!glfwWindowShouldClose(window)) {
        
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex1);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}

void mat4_x_vec4(t_mat4 mat, t_vec4 vec, t_vec4 out){

    out[X] = mat[0]*vec[X] + mat[4]*vec[Y] + mat[8]*vec[Z] + mat[12]*vec[W];
    out[Y] = mat[1]*vec[X] + mat[5]*vec[Y] + mat[9]*vec[Z] + mat[13]*vec[W];
    out[Z] = mat[2]*vec[X] + mat[6]*vec[Y] + mat[10]*vec[Z] + mat[14]*vec[W];
    out[W] = mat[3]*vec[X] + mat[7]*vec[Y] + mat[11]*vec[Z] + mat[15]*vec[W];
    
}

void mat4_x_mat4(t_mat4 m, t_mat4 n, t_mat4 out){
 
    /*
        0  1  2  3   0  1  2  3
        4  5  6  7   4  5  6  7
        8  9 10 11   8  9 10 11
       12 13 14 15  12 13 14 15
     
     
     */
    
    out[0] = m[0]*n[0] + m[4]*n[1] + m[8]*n[2] + m[12]*n[3];
    out[1] = m[1]*n[0] + m[5]*n[1] + m[9]*n[2] + m[13]*n[3];
    out[2] = m[2]*n[0] + m[6]*n[1] + m[10]*n[2] + m[14]*n[3];
    out[3] = m[3]*n[0] + m[7]*n[1] + m[11]*n[2] + m[15]*n[3];
    out[4] = m[0]*n[4] + m[4]*n[5] + m[8]*n[6] + m[12]*n[7];
    out[5] = m[1]*n[4] + m[5]*n[5] + m[9]*n[6] + m[13]*n[7];
    out[6] = m[2]*n[4] + m[6]*n[5] + m[10]*n[6] + m[14]*n[7];
    out[7] = m[3]*n[4] + m[7]*n[5] + m[11]*n[6] + m[15]*n[7];
    out[8] = m[0]*n[8] + m[4]*n[9] + m[8]*n[10] + m[12]*n[11];
    out[9] = m[1]*n[8] + m[5]*n[9] + m[9]*n[10] + m[13]*n[11];
    out[10] = m[2]*n[8] + m[6]*n[9] + m[10]*n[10] + m[14]*n[11];
    out[11] = m[3]*n[8] + m[7]*n[9] + m[11]*n[10] + m[15]*n[11];
    out[12] = m[0]*n[12] + m[4]*n[13] + m[8]*n[14] + m[12]*n[15];
    out[13] = m[1]*n[12] + m[5]*n[13] + m[9]*n[14] + m[13]*n[15];
    out[14] = m[2]*n[12] + m[6]*n[13] + m[10]*n[14] + m[14]*n[15];
    out[15] = m[3]*n[12] + m[7]*n[13] + m[11]*n[14] + m[15]*n[15];
   
}

void test(){
    t_vec4 vec;
    t_vec3 pos;
    t_mat4 mat;
    int i = 0;
    
    glmc_vec4(1.0, 0.0, 0.0, 1.0, vec);
    glmc_vec3(1.0, 1.0, 0.0, pos);
    glmc_identity(mat);
    
    glmc_translate(mat, pos, mat);
    
    for(i = 0; i < 16; i++){
        
        
        if(i%4 == 0){
            printf("\n");
        }
        printf("%.1f ", mat[i]);
    }
    printf("\n");
    
    mat4_x_vec4(mat, vec, vec);
    
    
    printf("X: %lf, Y: %lf, Z: %lf\n", vec[X], vec[Y], vec[Z]);
    
}

double degree_to_radian(double angle_in_degrees){
    return angle_in_degrees*(M_PI/180.0);
}

void gl_sanity_test(){
    GLenum err;
    while((err = glGetError())!= GL_NO_ERROR){
        printf("Something broke. OpenGL err: %x\n", err);
    }
}
//prints out 4x4array//
void print_4x4_matrix(t_mat4 matrix){
    int i;
    for(i = 0; i < 16; i++){
        //space out numbers incase of negative signs
        if(matrix[i] >= 0){
            printf(" ");
        }
        //print out data
        printf("%.1f ", matrix[i]);
        //makes a new row
        if(i!=0 && i%4 == 3){
            printf("\n");
        }
    }
    printf("\n");
}

//converts double matrix into float matrix.
void toFloatMat4(double in[], float out[]){
    
    for(int i = 0; i < 16; i++){
        out[i] = (float)in[i];
    }
}

void copyMat4(t_mat4 in, t_mat4 out){
    
    for(int i = 0; i <  16; i++){
        out[i] = in[i];
    }
}

void quat_mat4(t_vec3 axis, double radian, t_mat4 r){
    
    double ha = radian/2;
    double norm;
    
    double q0, q1, q2, q3; // q0 + q1i + q2j + q3k
    glmc_normalize(axis);
    
    q0 = cos(ha);
    q1 = sin(ha)*axis[0];
    q2 = sin(ha)*axis[1];
    q3 = sin(ha)*axis[2];
    
    norm = q0*q0 + q1*q1 + q2*q2 + q3*q3;
    
    if(norm > 0.0){
        q0 = q0/norm;
        q1 = q1/norm;
        q2 = q2/norm;
        q3 = q3/norm;
    }
    
    r[0] =  ( q0*q0 + q1*q1 - q2*q2- q3*q3);
    r[1] = 2*( q0*q3 + q1*q2);
    r[2] = 2*( q1*q3 - q0*q2);
    r[3] = 0;
    
    r[4] = 2*(q1*q2 - q0*q3);
    r[5] =  (q0*q0 - q1*q1 + q2*q2 - q3*q3);
    r[6] = 2*(q0*q1 + q2*q3);
    r[7] = 0;
    
    r[8] = 2*(q0*q2+ q1*q3);
    r[9] = 2*(q2*q3 - q0*q1);
    r[10]=  (q0*q0 - q1*q1 - q2*q2 + q3*q3);
    r[11]= 0;
    
    r[12] = 0;
    r[13] = 0;
    r[14] = 0;
    r[15] = 1;
    
}
