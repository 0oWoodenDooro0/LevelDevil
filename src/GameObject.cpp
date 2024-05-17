//
// Created by User on 2024/5/10.
//

#include "GameObject.hpp"

void GameObject::Enable() {
    SetVisible(true);
    SetEnabled(true);
}

void GameObject::Disable() {
    SetVisible(false);
    SetEnabled(false);
}