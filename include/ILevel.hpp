//
// Created by User on 2024/3/21.
//

#ifndef LEVELDEVIL_ILEVEL_HPP
#define LEVELDEVIL_ILEVEL_HPP

class ILevel {
public:
    virtual ~ILevel() = default;

    virtual void Start() = 0;

    virtual void Update() = 0;
};

#endif //LEVELDEVIL_ILEVEL_HPP