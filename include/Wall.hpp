//
// Created by User on 2024/3/2.
//

#ifndef LEVELDEVIL_WALL_HPP
#define LEVELDEVIL_WALL_HPP


#include "Util/GameObject.hpp"

class Wall: public Util::GameObject{
public:
    explicit Wall(const std::string &image_path);
    void SetImage(const std::string &image_path);
    void SetPosition(glm::vec2 position);
    glm::vec2 GetPosition();
    glm::vec2 GetSize();
private:
    std::string image_path_;
};


#endif //LEVELDEVIL_WALL_HPP
