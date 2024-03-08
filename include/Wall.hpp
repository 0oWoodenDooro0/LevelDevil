//
// Created by User on 2024/3/2.
//

#ifndef LEVELDEVIL_WALL_HPP
#define LEVELDEVIL_WALL_HPP


#include "Util/GameObject.hpp"
#include "Collider.hpp"

class Wall : public Util::GameObject {
public:
    explicit Wall(const std::string &image_path);

    void SetImage(const std::string &image_path);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    inline glm::vec2 GetPosition() { return m_Transform.translation; }

    inline glm::vec2 GetSize() { return m_Drawable->GetSize(); }

    inline Collider GetCollider() { return {GetPosition(), GetSize()}; }

private:
    std::string image_path_;
};


#endif //LEVELDEVIL_WALL_HPP
