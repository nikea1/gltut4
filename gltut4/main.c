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

int main(int argc, const char * argv[]) {
    
    
    
    initShaderFiles("vshader.vsh", "fshader.fsh");
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
