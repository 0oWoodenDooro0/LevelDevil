//
// Created by User on 2024/3/2.
//

#include "CollisionHandler.hpp"

bool CollisionHandler::CheckCollision(glm::vec2 position1, glm::vec2 size1, glm::vec2 position2, glm::vec2 size2) {
    return position1.x + size1.x / 2 > position2.x - size2.x / 2 &&
           position1.x - size1.x / 2 < position2.x + size2.x / 2 &&
           position1.y + size1.y / 2 > position2.y - size2.y / 2 &&
           position1.y - size1.y / 2 < position2.y + size2.y / 2;
}

glm::vec2 CollisionHandler::SweepTest(glm::vec2 position1, glm::vec2 position2, glm::vec2 size1,
                                      glm::vec2 size2, glm::vec2 velocity) {
    float deltaX = velocity.x;
    float deltaY = velocity.y;
    glm::vec2 collider1_max = position1 + glm::vec2(size1.x / 2, size1.y / 2);
    glm::vec2 collider1_min = position1 - glm::vec2(size1.x / 2, size1.y / 2);
    glm::vec2 collider2_max = position2 + glm::vec2(size2.x / 2, size2.y / 2);
    glm::vec2 collider2_min = position2 - glm::vec2(size2.x / 2, size2.y / 2);
    if (collider1_max.y > collider2_min.y && collider1_min.y < collider2_max.y) {
        float d1;

        if (deltaX > 0 && collider1_max.x <= collider2_min.x) {
            d1 = collider2_min.x - collider1_max.x;

            if (d1 < deltaX) {
                deltaX = d1;
            }
        } else if (deltaX < 0 && collider1_min.x >= collider2_max.x) {
            d1 = collider2_max.x - collider1_min.x;

            if (d1 > deltaX) {
                deltaX = d1;
            }
        }
    }

    if (collider1_max.x > collider2_min.x && collider1_min.x < collider2_max.x) {
        float d1;

        if (deltaY > 0 && collider1_max.y <= collider2_min.y) {
            d1 = collider2_min.y - collider1_max.y;

            if (d1 < deltaY) {
                deltaY = d1;
            }
        } else if (deltaY < 0 && collider1_min.y >= collider2_max.y) {
            d1 = collider2_max.y - collider1_min.y;

            if (d1 > deltaY) {
                deltaY = d1;
            }
        }
    }
    return {deltaX, deltaY};
}
