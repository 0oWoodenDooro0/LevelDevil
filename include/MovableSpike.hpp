//
// Created by User on 2024/4/27.
//

#ifndef LEVELDEVIL_MOVABLESPIKE_HPP
#define LEVELDEVIL_MOVABLESPIKE_HPP

#include "Spike.hpp"
#include "Core/Drawable.hpp"

class MovableSpike : public Spike {
public:
    explicit MovableSpike(const std::string& image_path, Position position_inbox, AudioManager audio_manager);

    void Move(glm::vec2 target_position, float speed);
};


#endif //LEVELDEVIL_MOVABLESPIKE_HPP
