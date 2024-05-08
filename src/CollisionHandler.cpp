//
// Created by User on 2024/3/2.
//

#include "CollisionHandler.hpp"
#include "Collider.hpp"

bool CollisionHandler::CheckCollision(const Collider &collider, const Collider &other) {
    return collider.right >= other.left && collider.left <= other.right && collider.top >= other.bottom &&
           collider.bottom <= other.top;
}

bool CollisionHandler::IsCollide(glm::vec2 position, const Collider &collider) {
    return collider.right > position.x && collider.left < position.x && collider.top > position.y &&
           collider.bottom < position.y;
}

glm::vec2 CollisionHandler::SweepTest(const Collider &collider, const Collider &other, glm::vec2 velocity) {
    float deltaX = velocity.x;
    float deltaY = velocity.y;
    if (collider.top > other.bottom && collider.bottom < other.top) {
        float d1;

        if (deltaX > 0 && collider.right <= other.left) {
            d1 = other.left - collider.right;

            if (d1 < deltaX) {
                deltaX = d1;
            }
        } else if (deltaX < 0 && collider.left >= other.right) {
            d1 = other.right - collider.left;

            if (d1 > deltaX) {
                deltaX = d1;
            }
        }
    }

    if (collider.right > other.left && collider.left < other.right) {
        float d1;

        if (deltaY > 0 && collider.top <= other.bottom) {
            d1 = other.bottom - collider.top;

            if (d1 < deltaY) {
                deltaY = d1;
            }
        } else if (deltaY < 0 && collider.bottom >= other.top) {
            d1 = other.top - collider.bottom;

            if (d1 > deltaY) {
                deltaY = d1;
            }
        }
    }
    return {deltaX, deltaY};
}
