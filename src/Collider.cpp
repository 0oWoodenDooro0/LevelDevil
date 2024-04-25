//
// Created by User on 2024/3/8.
//

#include "Collider.hpp"

Collider::Collider(glm::vec2 position, glm::vec2 _size) : center(position),
                                                          size(_size),
                                                          top(position.y + _size.y / 2),
                                                          bottom(position.y - _size.y / 2),
                                                          left(position.x - _size.x / 2),
                                                          right(position.x + _size.x / 2) {}
