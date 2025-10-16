#pragma once
#include <glm/glm.hpp>
#include "Model.h"

class Player {
public:
    glm::vec3 position;
    glm::vec3 front;
    float yaw;
    float speed;
    Model model;
    Player();
    void LoadModel(const std::string& path);
    void Update(float dt, bool forward, bool back, bool left, bool right);
    glm::mat4 GetModelMatrix() const;
};
