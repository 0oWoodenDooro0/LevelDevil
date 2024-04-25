//
// Created by User on 2024/4/19.
//

#ifndef LEVELDEVIL_EASINGFUNCTION_HPP
#define LEVELDEVIL_EASINGFUNCTION_HPP


class EasingFunction {
public:
    static inline float EaseInCubic(float x) {
        return x * x * x;
    }

    static inline float EaseOutCubic(float x) {
        return 1 - powf(1 - x, 3);
    }
};


#endif //LEVELDEVIL_EASINGFUNCTION_HPP
