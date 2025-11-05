#ifndef WHEEL_H
#define WHEEL_H

#include <string>
#include <vector>

class Wheel {
private:
    std::vector<std::string> segments;

public:
    Wheel();                // constructor
    std::string spin();     // spins and returns the result
};

#endif // WHEEL_H
