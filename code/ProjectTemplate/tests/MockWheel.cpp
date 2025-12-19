#include <gmock/gmock.h>
#include "Wheel.h"

class MockWheel : public Wheel {
public:
    MOCK_METHOD(void, spinWheel, (), (override));
    MOCK_METHOD(void, stopSpin, (), (override));
};
