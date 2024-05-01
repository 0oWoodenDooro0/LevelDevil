//
// Created by User on 2024/3/12.
//

#ifndef LEVELDEVIL_SPRITE_HPP
#define LEVELDEVIL_SPRITE_HPP


#include <string>
#include "Util/GameObject.hpp"
#include "Collider.hpp"
#include "IBehaviour.hpp"

class Sprite : public Util::GameObject, public IBehaviour {
public:
    explicit Sprite(const std::shared_ptr<Core::Drawable> &drawable, float z_index = 0);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return m_Drawable->GetSize(); }

    [[nodiscard]] inline Collider GetCollider() const { return {GetPosition(), GetSize()}; }

    void Enable() override;

    void Disable() override;

    bool enabled = true;
};


#endif //LEVELDEVIL_SPRITE_HPP
