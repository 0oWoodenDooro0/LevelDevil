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
#include "Animator.hpp"
#include "Character.hpp"
#include "IBehaviour.hpp"
#include "AudioManager.hpp"
#include "SoundEffect.hpp"

class Spike : public Util::GameObject, public IBehaviour {
public:
    explicit Spike(const std::string &image_path, AudioManager audiomanager);

    void SetImage(const std::string &image_path);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return m_Drawable->GetSize(); }

    [[nodiscard]] inline Collider GetCollider() const {
        return {GetPosition() - glm::vec2(0, 21), GetSize() - glm::vec2(0, 42)};
    }

    void Update(const std::shared_ptr<Character> &character_);

    void Enable() override;

    void Disable() override;

private:
    std::string image_path_;
    AudioManager audiomanager_;
};

#endif //LEVELDEVIL_SPIKE_HPP