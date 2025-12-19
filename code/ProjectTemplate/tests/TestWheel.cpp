#include <gmock/gmock.h>

#include <QApplication>
#include <QSignalSpy>

#include "Wheel.h"

class WheelTest : public ::testing::Test {
protected:
    static QApplication* app;
    Wheel* wheel;

    // Creates a fresh Wheel instance before each test
    void SetUp() override {
        wheel = new Wheel();
        wheel->show();
    }

    // Deletes the wheel after each test case
    void TearDown() override {
        delete wheel;
    }
};

QApplication* WheelTest::app = nullptr;

// Verifies that the wheel object was created
TEST_F(WheelTest, WheelExists) {
    EXPECT_NE(wheel, nullptr);
}

// Confirms that the wheel start in a correct default state
TEST_F(WheelTest, InitialState) {
    EXPECT_FALSE(wheel->spinning());
    EXPECT_DOUBLE_EQ(wheel->rotation(), 0.0);
}

// Checks that spinWheel() updates the wheel's state correctly
TEST_F(WheelTest, SpinWheelSetsSpinning) {
    wheel->spinWheel();
    EXPECT_TRUE(wheel->spinning());
}

// Ensures stopping teh wheel behaves correctly and emits a signal
TEST_F(WheelTest, StopSpinEmitsSignal) {
    QSignalSpy spy(wheel, &Wheel::landedSegment);

    wheel->spinWheel();
    wheel->stopSpin();

    EXPECT_FALSE(wheel->spinning());
    EXPECT_EQ(spy.count(), 1);  // landedSegment should emit once
    int seg = spy.takeFirst().at(0).toInt();
    EXPECT_GE(seg, 0);
    EXPECT_LT(seg, 8); // the wheel has 8 segments
}

// Validates of the wheel landed on a valid segment
TEST_F(WheelTest, CalculateSegmentValid) {
    int index = wheel->calculateSegment(0.0);
    EXPECT_GE(index, 0);
    EXPECT_LT(index, 8);

    index = wheel->calculateSegment(360.0);
    EXPECT_GE(index, 0);
    EXPECT_LT(index, 8);
}
