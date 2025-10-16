#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "Shader.h"
#include "Camera.h"
#include "Player.h"
#include "Collision.h"
#include "Model.h"
#include "stb_image.h"

namespace fs = std::filesystem;
int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;
bool keys[1024];

// ✅ Auto-detect correct path for file (works if running from build/)
std::string findPath(const std::string &relative)
{
    if (fs::exists(relative))
        return relative;
    if (fs::exists("../" + relative))
        return "../" + relative;
    if (fs::exists("../../" + relative))
        return "../../" + relative;
    std::cerr << "⚠️ File not found: " << relative << std::endl;
    return relative;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    keys[GLFW_KEY_W] = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    keys[GLFW_KEY_S] = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    keys[GLFW_KEY_A] = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    keys[GLFW_KEY_D] = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
}

unsigned int loadCubemap(const std::vector<std::string> &faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); ++i)
    {
        std::string path = findPath(faces[i]);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Cubemap load failed: " << faces[i] << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int main()
{
    std::cout << "Current working directory: " << fs::current_path() << std::endl;

    // --- Initialize GLFW ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Simple 3D Game", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // --- Load shaders ---
    Shader modelShader(findPath("shaders/model.vert").c_str(), findPath("shaders/model.frag").c_str());
    Shader skyShader(findPath("shaders/skybox.vert").c_str(), findPath("shaders/skybox.frag").c_str());

    // --- Load models ---
    Player player;
    player.LoadModel(findPath("assets/models/Dog.fbx"));
    Model sceneModel(findPath("assets/models/Trash.fbx"));
    Model itemModel(findPath("assets/models/bone.fbx"));

    // --- Setup skybox ---
    float skyVertices[] = {
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};

    unsigned int skyVAO, skyVBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glBindVertexArray(0);

    std::vector<std::string> faces = {
        "assets/cubemap/right.png", "assets/cubemap/left.png",
        "assets/cubemap/top.png", "assets/cubemap/bottom.png",
        "assets/cubemap/front.png", "assets/cubemap/back.png"};
    unsigned int cubemapTex = loadCubemap(faces);

    skyShader.use();
    skyShader.setInt("skybox", 0);

    Camera camera(glm::vec3(0.0f, 5.0f, 15.0f));
    std::vector<glm::vec3> bonePositions = {
        {2.0f, 0.2f, -4.0f}, {-1.0f, 0.2f, 2.0f}, {4.0f, 0.2f, 1.0f}};
    std::vector<bool> bonesCollected(3, false);

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glm::vec3 prevPos = player.position;
        player.Update(dt, keys[GLFW_KEY_W], keys[GLFW_KEY_S], keys[GLFW_KEY_A], keys[GLFW_KEY_D]);

        // Check collisions with bones
        AABB dogBox = Collision::FromPositionSize(player.position, glm::vec3(0.5f, 0.4f, 0.8f));
        for (int i = 0; i < 3; ++i)
        {
            if (!bonesCollected[i])
            {
                AABB boneBox = Collision::FromPositionSize(bonePositions[i], glm::vec3(0.3f));
                if (Collision::TestAABB(dogBox, boneBox))
                    bonesCollected[i] = true;
            }
        }

        // trash collision
        glm::vec3 trashPos = glm::vec3(5.0f, 0.0f, -3.0f);
        glm::vec3 trashHalf = glm::vec3(0.6f, 1.0f, 0.6f);
        AABB trashBox;
        trashBox.min = trashPos - trashHalf;
        trashBox.max = trashPos + trashHalf;
        if (Collision::TestAABB(dogBox, trashBox))
        {
            // revert to previous position so dog cannot pass through
            player.position = prevPos;
        }

        // Update camera (third-person follow)
        glm::vec3 camOffset = -player.front * 8.0f + glm::vec3(0.0f, 3.0f, 0.0f);
        camera.Position = player.position + camOffset;
        camera.Front = glm::normalize(player.position - camera.Position);

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // Draw scene
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        modelShader.setVec3("viewPos", camera.Position);
        modelShader.setVec3("lightPos", glm::vec3(10.0f, 10.0f, 10.0f));
        modelShader.setVec3("lightColor", glm::vec3(1.0f));

        // make sure shader uses texture for scene/player
        modelShader.setInt("useObjectColor", 0);

        glm::mat4 sceneM(1.0f);
        sceneM = glm::translate(sceneM, glm::vec3(5.0f, 0.0f, -3.0f));
        sceneM = glm::scale(sceneM, glm::vec3(0.025f));
        modelShader.setMat4("model", sceneM);
        glm::vec3 trashColor = glm::vec3(0.25f, 0.25f, 0.27f);
        modelShader.setVec3("objectColor", trashColor);
        modelShader.setInt("useObjectColor", 1);
        sceneModel.Draw();
        // reset to use texture for subsequent objects (player/bones)
        modelShader.setInt("useObjectColor", 0);

        // Draw player (rotate only while holding R)
        glm::mat4 dogM = player.GetModelMatrix();
        static float accumulatedAngle = 0.0f;
        const float rotationSpeedDegPerSec = 240.0f;
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            accumulatedAngle += glm::radians(rotationSpeedDegPerSec) * dt;
            accumulatedAngle = fmod(accumulatedAngle, glm::two_pi<float>());
        }
        dogM = glm::rotate(dogM, accumulatedAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", dogM);
        player.model.Draw();

        // Draw bones
        for (int i = 0; i < 3; ++i)
        {
            if (!bonesCollected[i])
            {
                glm::mat4 itemM(1.0f);
                itemM = glm::translate(itemM, bonePositions[i]);
                itemM = glm::scale(itemM, glm::vec3(0.05f));
                itemM = glm::rotate(itemM, (float)glfwGetTime() * 2.0f, glm::vec3(0, 1, 0));
                glm::vec3 baseColor = glm::vec3(0.94f, 0.88f, 0.72f);
                float pulse = (std::sin((float)glfwGetTime() * 2.0f) * 0.5f + 0.5f) * 0.04f;
                glm::vec3 finalColor = glm::clamp(baseColor + glm::vec3(pulse), 0.0f, 1.0f);
                modelShader.setVec3("objectColor", finalColor);
                modelShader.setInt("useObjectColor", 1);
                modelShader.setMat4("model", itemM);
                itemModel.Draw();
            }
        }
        modelShader.setInt("useObjectColor", 0);

        // Draw skybox last
        glDepthFunc(GL_LEQUAL);
        skyShader.use();
        skyShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyShader.setMat4("projection", projection);
        glBindVertexArray(skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
