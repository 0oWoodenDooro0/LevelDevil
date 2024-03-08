//
// Created by User on 2024/3/6.
//

#ifndef LEVELDEVIL_SPIKE_HPP
#define LEVELDEVIL_SPIKE_HPP

#include <string>
#include <glm/vec2.hpp>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Wall.hpp"
#include "Animator.hpp"
#include "Character.hpp"

class Spike : public Util::GameObject {
public:
    explicit Spike(const std::string& image_path);

    void SetImage(const std::string& image_path);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return { 64, 64 }; }

    void Update(std::shared_ptr<Character> character_);

    void Translate(const glm::vec2& position);

private:
    std::string image_path_;
};

#endif //LEVELDEVIL_Spike_HPP