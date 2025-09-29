// main.cpp
// Enhanced Kinetic Sculpture with Beautiful Visual Effects
// Uses 3D transformation matrices for complex animations and multiple light sources

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "filesystem.h"
#include "shader_m.h"
#include "camera.h"

#include <iostream>
#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera - positioned for better view
Camera camera(glm::vec3(0.0f, 8.0f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Enhanced Kinetic Sculpture", NULL, NULL);
    if (!window) { std::cout<<"Failed creating window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cout<<"Failed to init GLAD\n"; return -1; }
    glEnable(GL_DEPTH_TEST);

    // shaders
    Shader sculptureShader(FileSystem::getPath("shaders/sculpture.vs").c_str(), FileSystem::getPath("shaders/sculpture.fs").c_str());
    Shader lightCubeShader(FileSystem::getPath("shaders/light_cube.vs").c_str(), FileSystem::getPath("shaders/light_cube.fs").c_str());

    // base cube vertices (position, normal, texcoords)
    float vertices[] = {
        // positions          // normals           // texture coords
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    
        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // Generate VBO / VAO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    // pos
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texcoords
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // light cube VAO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1,&lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    // load textures
    unsigned int diffuseMap = loadTexture(FileSystem::getPath("resources/textures/3DPear004_HQ-1K-PNG_Color.png").c_str());
    unsigned int specularMap = loadTexture(FileSystem::getPath("resources/textures/3DPear004_HQ-1K-PNG_Roughness.png").c_str());

    sculptureShader.use();
    sculptureShader.setInt("material.diffuse", 0);
    sculptureShader.setInt("material.specular", 1);

    // Enhanced sculpture configuration: Create multiple interconnected structures
    std::vector<glm::vec3> instances;
    
    // Central spiral tower
    const int SPIRAL_LEVELS = 20;
    for (int i = 0; i < SPIRAL_LEVELS; i++) {
        float height = (float)i * 0.4f - 4.0f;
        float angle = (float)i * 0.5f;
        float radius = 2.0f + sin((float)i * 0.3f) * 1.0f;
        
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        instances.push_back(glm::vec3(x, height, z));
    }
    
    // Orbiting rings
    const int RINGS = 3;
    const int RING_SEGMENTS = 16;
    for (int ring = 0; ring < RINGS; ring++) {
        float ringHeight = (float)ring * 3.0f - 2.0f;
        float ringRadius = 5.0f + (float)ring * 1.5f;
        
        for (int i = 0; i < RING_SEGMENTS; i++) {
            float angle = (float)i / (float)RING_SEGMENTS * glm::two_pi<float>();
            float x = ringRadius * cos(angle);
            float z = ringRadius * sin(angle);
            instances.push_back(glm::vec3(x, ringHeight, z));
        }
    }
    
    // Floating geometric patterns
    const int PATTERN_COUNT = 24;
    for (int i = 0; i < PATTERN_COUNT; i++) {
        float angle = (float)i / (float)PATTERN_COUNT * glm::two_pi<float>();
        float radius = 8.0f;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        float y = sin(angle * 3.0f) * 2.0f;
        instances.push_back(glm::vec3(x, y, z));
    }

    // Enhanced multiple light sources with varied colors
    std::vector<glm::vec3> pointLightPositions = {
        glm::vec3( 6.0f,  4.0f,  6.0f),   // warm orange light
        glm::vec3(-6.0f,  2.0f, -6.0f),  // cool blue light
        glm::vec3( 0.0f, -3.0f,  8.0f),  // purple light
        glm::vec3( 8.0f,  1.0f, -4.0f),  // green light
        glm::vec3(-4.0f,  6.0f,  4.0f),  // red light
        glm::vec3( 2.0f, -2.0f, -8.0f)   // cyan light
    };

    // Light colors for variety
    std::vector<glm::vec3> lightColors = {
        glm::vec3(1.0f, 0.7f, 0.3f),  // warm orange
        glm::vec3(0.3f, 0.7f, 1.0f),  // cool blue
        glm::vec3(0.8f, 0.3f, 1.0f),  // purple
        glm::vec3(0.3f, 1.0f, 0.5f),  // green
        glm::vec3(1.0f, 0.3f, 0.3f),  // red
        glm::vec3(0.3f, 1.0f, 1.0f)   // cyan
    };

    // render loop
    while(!glfwWindowShouldClose(window)){
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Enhanced background with gradient effect
        glClearColor(0.02f, 0.02f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set lighting uniforms
        sculptureShader.use();
        sculptureShader.setVec3("viewPos", camera.Position);
        sculptureShader.setFloat("material.shininess", 128.0f);

        // Enhanced directional light (moonlight effect)
        sculptureShader.setVec3("dirLight.direction", -0.3f, -1.0f, -0.4f);
        sculptureShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.08f);
        sculptureShader.setVec3("dirLight.diffuse", 0.2f, 0.25f, 0.35f);
        sculptureShader.setVec3("dirLight.specular", 0.3f, 0.35f, 0.5f);

        // Animate multiple point lights with different patterns
        for (unsigned int i = 0; i < pointLightPositions.size() && i < 4; ++i) {
            glm::vec3 base = pointLightPositions[i];
            
            // Each light has unique movement pattern
            float timeOffset = (float)i * 1.57f; // π/2 offset
            float moveRadius = 2.0f + sin(currentFrame * 0.3f + timeOffset) * 1.0f;
            float moveSpeed = 0.8f + (float)i * 0.2f;
            
            glm::vec3 pos = base + glm::vec3(
                cos(currentFrame * moveSpeed + timeOffset) * moveRadius,
                sin(currentFrame * 0.7f + timeOffset) * 1.5f,
                sin(currentFrame * moveSpeed + timeOffset) * moveRadius
            );
            
            std::string idx = "pointLights[" + std::to_string(i) + "]";
            sculptureShader.setVec3(idx + ".position", pos);
            sculptureShader.setVec3(idx + ".ambient", lightColors[i] * 0.1f);
            sculptureShader.setVec3(idx + ".diffuse", lightColors[i] * 0.8f);
            sculptureShader.setVec3(idx + ".specular", lightColors[i]);
            sculptureShader.setFloat(idx + ".constant", 1.0f);
            sculptureShader.setFloat(idx + ".linear", 0.07f);
            sculptureShader.setFloat(idx + ".quadratic", 0.017f);
        }

        // Enhanced spotlight (follows camera)
        sculptureShader.setVec3("spotLight.position", camera.Position);
        sculptureShader.setVec3("spotLight.direction", camera.Front);
        sculptureShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(15.0f)));
        sculptureShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(20.0f)));
        sculptureShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        sculptureShader.setVec3("spotLight.diffuse", 1.2f, 1.2f, 1.0f);
        sculptureShader.setVec3("spotLight.specular", 1.5f, 1.5f, 1.2f);
        sculptureShader.setFloat("spotLight.constant", 1.0f);
        sculptureShader.setFloat("spotLight.linear", 0.045f);
        sculptureShader.setFloat("spotLight.quadratic", 0.0075f);

        // projection / view
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        sculptureShader.setMat4("projection", projection);
        sculptureShader.setMat4("view", view);

        sculptureShader.setFloat("time", currentFrame);

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Enhanced instance rendering with complex transformations
        glBindVertexArray(cubeVAO);
        for (size_t i = 0; i < instances.size(); ++i) {
            glm::vec3 pos = instances[i];
            float phase = (float)i * 0.15f;
            
            // Complex kinetic motion using multiple transformation matrices
            glm::mat4 model = glm::mat4(1.0f);
            
            // Primary translation with wave motion
            float waveOffset = sin(currentFrame * 1.5f + phase) * 0.4f;
            float spiralOffset = cos(currentFrame * 0.8f + phase * 0.7f) * 0.3f;
            glm::vec3 animatedPos = pos + glm::vec3(spiralOffset, waveOffset, spiralOffset * 0.5f);
            model = glm::translate(model, animatedPos);
            
            // Rotation around world Y-axis (orbital motion)
            if (i >= SPIRAL_LEVELS) { // Only rings and patterns orbit
                model = glm::rotate(model, currentFrame * 0.3f + phase, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            
            // Local rotation for each cube
            model = glm::rotate(model, currentFrame * 2.0f + phase, glm::vec3(1.0f, 0.3f, 0.7f));
            model = glm::rotate(model, currentFrame * 1.2f + phase * 0.5f, glm::vec3(0.2f, 1.0f, 0.1f));
            
            // Dynamic scaling with breathing effect
            float breathe = 0.3f + 0.15f * sin(currentFrame * 2.5f + phase * 0.8f);
            float pulse = 0.05f * sin(currentFrame * 8.0f + phase);
            model = glm::scale(model, glm::vec3(breathe + pulse));
            
            sculptureShader.setMat4("model", model);

            // Enhanced per-instance coloring with smooth transitions
            float colorPhase = (float)i * 0.08f + currentFrame * 0.5f;
            glm::vec3 baseColor = glm::vec3(
                0.4f + 0.4f * sin(colorPhase),
                0.5f + 0.3f * sin(colorPhase * 1.3f + 1.0f),
                0.6f + 0.4f * sin(colorPhase * 0.7f + 2.0f)
            );
            
            // Add height-based color variation
            float heightFactor = (pos.y + 5.0f) / 10.0f;
            baseColor = mix(baseColor, glm::vec3(0.8f, 0.3f, 0.9f), heightFactor * 0.3f);
            
            sculptureShader.setVec3("overrideColor", baseColor);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Enhanced light cube rendering with colors
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        glBindVertexArray(lightCubeVAO);
        
        for (unsigned int i = 0; i < pointLightPositions.size(); ++i) {
            glm::vec3 base = pointLightPositions[i];
            
            // Same movement as lighting calculation
            float timeOffset = (float)i * 1.57f;
            float moveRadius = 2.0f + sin(currentFrame * 0.3f + timeOffset) * 1.0f;
            float moveSpeed = 0.8f + (float)i * 0.2f;
            
            glm::vec3 pos = base + glm::vec3(
                cos(currentFrame * moveSpeed + timeOffset) * moveRadius,
                sin(currentFrame * 0.7f + timeOffset) * 1.5f,
                sin(currentFrame * moveSpeed + timeOffset) * moveRadius
            );
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            
            // Pulsing scale for light cubes
            float lightPulse = 0.2f + 0.1f * sin(currentFrame * 6.0f + (float)i);
            model = glm::scale(model, glm::vec3(lightPulse));
            
            // Rotate light cubes
            model = glm::rotate(model, currentFrame * 3.0f + (float)i, glm::vec3(1.0f, 1.0f, 0.0f));
            
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&cubeVAO);
    glDeleteVertexArrays(1,&lightCubeVAO);
    glDeleteBuffers(1,&VBO);

    glfwTerminate();
    return 0;
}

// callbacks and helper functions
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window,true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { 
    glViewport(0,0,width,height); 
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = (float)xposIn, ypos = (float)yposIn;
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    float xoffset = xpos - lastX, yoffset = lastY - ypos;
    lastX = xpos; lastY = ypos;
    camera.ProcessMouseMovement(xoffset,yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { 
    camera.ProcessMouseScroll((float)yoffset); 
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID; 
    glGenTextures(1,&textureID);
    int width,height,nrComponents;
    unsigned char *data = stbi_load(path,&width,&height,&nrComponents,0);
    if (data) {
        GLenum format = (nrComponents==1?GL_RED:(nrComponents==3?GL_RGB:GL_RGBA));
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        stbi_image_free(data);
    } else {
        std::cout<<"Failed to load texture at "<<path<<"\n";
        stbi_image_free(data);
    }
    return textureID;
}