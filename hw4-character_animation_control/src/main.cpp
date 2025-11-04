#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "animator.h"
#include "model_animation.h"
#include "filesystem.h"

#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <direct.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Animator &animator, glm::vec3 &characterPos, float &characterRotation);

// Global animation pointers
Animation* idleAnim_ptr = nullptr;
Animation* walkAnim_ptr = nullptr;
Animation* danceAnim_ptr = nullptr;

// Dance state tracking
bool isDancingState = false;
float lastDanceKeyPressTime = -1.0f;
float danceAnimationDuration = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"Character Animation Control - WASD to move, E to dance, Arrow keys for camera",NULL,NULL);
    if(!window){
        std::cout<<"Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);

    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to initialize GLAD\n";
        return -1;
    }

    // Tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    // Use relative paths for models so textures can be found correctly
    // Relative paths are easier to handle than absolute paths on different systems
    std::string modelPath = "resources/objects/doozy/Happy Idle.dae";
    std::string walkModelPath = "resources/objects/doozy/Walking.dae";
    std::string danceModelPath = "resources/objects/doozy/Gangnam Style.dae";
    std::string shaderVSPath = FileSystem::getPath("shaders/anim_model.vs");
    std::string shaderFSPath = FileSystem::getPath("shaders/anim_model.fs");
    
    // Load shader first
    Shader ourShader(shaderVSPath.c_str(), shaderFSPath.c_str());

    // Load models using relative paths

    Model ourModel(modelPath);
    Animation idleAnim(modelPath, &ourModel);
    Animation walkAnim(walkModelPath, &ourModel);
    Animation danceAnim(danceModelPath, &ourModel);
    
    // Set global pointers
    idleAnim_ptr = &idleAnim;
    walkAnim_ptr = &walkAnim;
    danceAnim_ptr = &danceAnim;
    danceAnimationDuration = danceAnim.GetDuration();
    
    Animator animator(&idleAnim);
    
    // Character position and rotation
    glm::vec3 characterPos(0.0f, -0.5f, 0.0f);
    float characterRotation = 0.0f;

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, animator, characterPos, characterRotation);
        animator.UpdateAnimation(deltaTime);

        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f,100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection",projection);
        ourShader.setMat4("view",view);

        auto transforms = animator.GetFinalBoneMatrices();
        for(int i=0;i<transforms.size();++i)
            ourShader.setMat4("finalBonesMatrices["+std::to_string(i)+"]",transforms[i]);

        glm::mat4 model=glm::mat4(1.0f);
        model=glm::translate(model, characterPos);
        model=glm::rotate(model, characterRotation, glm::vec3(0.0f, 1.0f, 0.0f));
        model=glm::scale(model,glm::vec3(0.5f));
        ourShader.setMat4("model",model);

        ourModel.Draw(ourShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, Animator &animator, glm::vec3 &characterPos, float &characterRotation)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);

    bool isMoving = false;
    
    // Character movement controls
    float moveSpeed = 2.0f * deltaTime;
    
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS) {
        // Move forward
        characterPos.z -= moveSpeed * cos(characterRotation);
        characterPos.x -= moveSpeed * sin(characterRotation);
        isMoving = true;
    }
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS) {
        // Move backward  
        characterPos.z += moveSpeed * cos(characterRotation);
        characterPos.x += moveSpeed * sin(characterRotation);
        isMoving = true;
    }
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS) {
        // Turn left
        characterRotation += 2.0f * deltaTime;
        isMoving = true;
    }
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS) {
        // Turn right
        characterRotation -= 2.0f * deltaTime;
        isMoving = true;
    }
    
    // Dance animation triggered by E key
    if(glfwGetKey(window,GLFW_KEY_E)==GLFW_PRESS && !isDancingState) {
        isDancingState = true;
        lastDanceKeyPressTime = glfwGetTime();
        animator.PlayAnimation(danceAnim_ptr);
    }
    
    // Check if dance animation has finished
    float currentTime = glfwGetTime();
    if(isDancingState && (currentTime - lastDanceKeyPressTime) > danceAnimationDuration) {
        isDancingState = false;
    }
    
    // Set animation based on input (only if not dancing)
    if (!isDancingState) {
        if (isMoving) {
            animator.PlayAnimation(walkAnim_ptr);
        } else {
            // Play idle animation when not moving or dancing
            animator.PlayAnimation(idleAnim_ptr);
        }
    }
    
    // Camera controls (keep original camera movement for better viewing)
    if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window,GLFW_KEY_DOWN)==GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height){ glViewport(0,0,width,height);}
void mouse_callback(GLFWwindow* window,double xpos,double ypos){
    if(firstMouse){ lastX=xpos; lastY=ypos; firstMouse=false; }
    float xoffset=xpos-lastX;
    float yoffset=lastY-ypos;
    lastX=xpos; lastY=ypos;
    camera.ProcessMouseMovement(xoffset,yoffset);
}
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset){ camera.ProcessMouseScroll(yoffset);}
