//
// Created by User on 2024/5/10.
//

#ifndef LEVELDEVIL_MOVABLEPORTAL_HPP
#define LEVELDEVIL_MOVABLEPORTAL_HPP

#include "Portal.hpp"
#include "Core/Drawable.hpp"

class MovablePortal : public Portal {
public:
    explicit MovablePortal(std::vector<std::string> image_paths, glm::vec2 position, AudioManager audiomanager);

    void Move(glm::vec2 target_position, float speed);
};


#endif //LEVELDEVIL_MOVABLEPORTAL_HPP
