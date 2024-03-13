#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    character_ = std::make_shared<Character>();

    root_.AddChild(character_);

    background_ = std::make_shared<Background>();

    root_.AddChild(background_);

    for (int i = 0; i < 10; ++i) {
        walls_.push_back(std::make_shared<Wall>(RESOURCE_DIR"/image/wall/test_wall.png"));

        root_.AddChild(walls_[i]);
    }
    walls_[0]->SetPosition({0, -200});
    walls_[1]->SetPosition({64, -200});
    walls_[2]->SetPosition({-64, -200});
    walls_[3]->SetPosition({-128, -200});
    walls_[4]->SetPosition({128, -200});
    walls_[5]->SetPosition({-192, -200});
    walls_[6]->SetPosition({192, -200});
    walls_[7]->SetPosition({-192, -136});
    walls_[8]->SetPosition({192, -136});
    walls_[9]->SetPosition({192, -72});

    door_ = std::make_shared<Door>();
    door_->SetPosition({ 128,-136 });
    root_.AddChild(door_);

    spikes_.push_back(std::make_shared<Spike>(RESOURCE_DIR"/image/component/spike.png"));
    spikes_[0]->SetPosition({ -128,-72 });
    root_.AddChild(spikes_[0]);

    current_state_ = State::UPDATE;
}

void App::Update() {

    character_->Update( walls_);
    for (const auto& spike : spikes_)
        if (spike->Update(character_))
            character_->Dead();
    if (door_->Update(character_)) {
        character_->Dead();
        door_->IfStageClear(true);
    }
        
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        current_state_ = State::END;
    }
    root_.Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
