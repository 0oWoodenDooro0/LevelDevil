//
// Created by User on 2024/3/2.
//

#include "CollisionHandler.hpp"
#include "Collider.hpp"

bool CollisionHandler::CheckCollision(glm::vec2 position1, glm::vec2 size1, glm::vec2 position2, glm::vec2 size2) {
    return position1.x + size1.x / 2 > position2.x - size2.x / 2 &&
           position1.x - size1.x / 2 < position2.x + size2.x / 2 &&
           position1.y + size1.y / 2 > position2.y - size2.y / 2 &&
           position1.y - size1.y / 2 < position2.y + size2.y / 2;
}

glm::vec2 CollisionHandler::SweepTest(Collider collider, Collider other, glm::vec2 velocity) {
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
