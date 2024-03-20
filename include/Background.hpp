//
// Created by User on 2024/3/1.
//

#ifndef LEVELDEVIL_BACKGROUND_HPP
#define LEVELDEVIL_BACKGROUND_HPP

#include <memory>
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"

class Background : public Util::GameObject {
public:
    explicit Background(const std::string& path);
};

#endif //LEVELDEVIL_BACKGROUND_HPP
