#include <gmock/gmock.h>
#include "Wheel.h"

/* The MockWheel class is a fake version of the Wheel class used in tests to verify
   that the GameController correctly calls wheel functions.
*/

class MockWheel : public Wheel {
public:
    MOCK_METHOD(void, spinWheel, (), (override));
    MOCK_METHOD(void, stopSpin, (), (override));

    // State for tests
    bool spinningFlag = false;
    void setSpinning(bool val) { spinningFlag = val; }
    bool spinning() const override { return spinningFlag; }
};
