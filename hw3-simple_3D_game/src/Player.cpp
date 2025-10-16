#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>

Player::Player() : position(0.0f, 0.5f, 0.0f), front(0.0f,0.0f,-1.0f), yaw(-90.0f), speed(6.0f) {}

void Player::LoadModel(const std::string& path) {
    model = Model(path);
}

void Player::Update(float dt, bool forward, bool back, bool left, bool right) {
    float turnSpeed = 90.0f; // deg/sec
    if(left) yaw += turnSpeed * dt;
    if(right) yaw -= turnSpeed * dt;
    // update front vector
    float rad = glm::radians(yaw);
    front = glm::normalize(glm::vec3(cos(rad), 0.0f, sin(rad)));
    if(forward) position += front * speed * dt;
    if(back) position -= front * speed * dt;
}

glm::mat4 Player::GetModelMatrix() const {
    glm::mat4 m(1.0f);
    m = glm::translate(m, position);
    m = glm::rotate(m, glm::radians(-yaw-90.0f), glm::vec3(0,1,0)); // orientation fix
    m = glm::scale(m, glm::vec3(0.5f));
    return m;
}
