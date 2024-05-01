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
    enum class Position {
        Top,
        Bottom,
        Left,
        Right
    };

    explicit Spike(const std::string &image_path, Position position_inbox, AudioManager audio_manager);

    void SetImage(const std::string &image_path);

    inline void SetPosition(glm::vec2 position) { m_Transform.translation = position; }

    [[nodiscard]] inline glm::vec2 GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] inline glm::vec2 GetSize() const { return m_Drawable->GetSize(); }

    [[nodiscard]] Collider GetCollider() const;

    [[nodiscard]] inline bool IsEnable() const { return enable_; }

    void Update(const std::shared_ptr<Character> &character);

    void Enable() override;

    void Disable() override;

private:
    bool enable_ = true;
    Position position_inbox_ = Position::Bottom;
    std::string image_path_;
    AudioManager audio_manager_;
};

#endif //LEVELDEVIL_SPIKE_HPP