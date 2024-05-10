//
// Created by User on 2024/5/10.
//

#ifndef LEVELDEVIL_MOVABLEGAMEOBJECT_HPP
#define LEVELDEVIL_MOVABLEGAMEOBJECT_HPP

#include "Util/GameObject.hpp"

class MovableGameObject {
public:
    explicit MovableGameObject(std::shared_ptr<Util::GameObject> gameObject);

    inline void SetPosition(glm::vec2 position) const { gameObject_->m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return gameObject_->m_Transform.translation; }

    [[nodiscard]] inline std::shared_ptr<Util::GameObject> GetGameObject() const { return gameObject_; }

    void Move(glm::vec2 target_position, float speed);

private:
    std::shared_ptr<Util::GameObject> gameObject_;
};


#endif //LEVELDEVIL_MOVABLEGAMEOBJECT_HPP
