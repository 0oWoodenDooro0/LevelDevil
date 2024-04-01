//
// Created by User on 2024/3/15.
//

#ifndef LEVELDEVIL_DOORBUTTON_HPP
#define LEVELDEVIL_DOORBUTTON_HPP


#include "Button.hpp"

class DoorButton : public Button {
public:
    explicit DoorButton(const std::shared_ptr<Core::Drawable> &idle, AudioManager audio_manager);

    void OnIdle() override;

    void OnHover() override;
};


#endif //LEVELDEVIL_DOORBUTTON_HPP
