#include <gmock/gmock.h>

#include <QApplication>
#include <QSignalSpy>

#include "Wheel.h"

class WheelTest : public ::testing::Test {
protected:
    static QApplication* app;
    Wheel* wheel;

    void SetUp() override {
        wheel = new Wheel();
        wheel->show();
    }

    void TearDown() override {
        delete wheel;
    }
};

QApplication* WheelTest::app = nullptr;

// Test that the wheel widget exists
TEST_F(WheelTest, WheelExists) {
    EXPECT_NE(wheel, nullptr);
}

// Test that initial state is correct
TEST_F(WheelTest, InitialState) {
    EXPECT_FALSE(wheel->spinning());
    EXPECT_DOUBLE_EQ(wheel->rotation(), 0.0);
}

// Test spinWheel changes state to spinning
TEST_F(WheelTest, SpinWheelSetsSpinning) {
    wheel->spinWheel();
    EXPECT_TRUE(wheel->spinning());
}


// Test stopSpin stops the wheel and emits landedSegment
TEST_F(WheelTest, StopSpinEmitsSignal) {
    QSignalSpy spy(wheel, &Wheel::landedSegment);

    wheel->spinWheel();
    wheel->stopSpin();

    EXPECT_FALSE(wheel->spinning());
    EXPECT_EQ(spy.count(), 1);  // landedSegment should emit once
    int seg = spy.takeFirst().at(0).toInt();
    EXPECT_GE(seg, 0);
    EXPECT_LT(seg, 8); // assuming 8 segments
}

// Test calculateSegment returns valid index
TEST_F(WheelTest, CalculateSegmentValid) {
    int index = wheel->calculateSegment(0.0);
    EXPECT_GE(index, 0);
    EXPECT_LT(index, 8);

    index = wheel->calculateSegment(360.0);
    EXPECT_GE(index, 0);
    EXPECT_LT(index, 8);
}
