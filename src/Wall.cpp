//
// Created by User on 2024/3/2.
//

#include "Wall.hpp"
#include "Util/Image.hpp"

Wall::Wall(const std::string &image_path) {
    SetImage(image_path);
    SetZIndex(0);
}

void Wall::SetImage(const std::string &image_path) {
    image_path_ = image_path;
    m_Drawable = std::make_shared<Util::Image>(image_path);
}