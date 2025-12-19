#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QApplication>
#include <QSignalSpy>

#include "GameController.h"
#include "MockWheel.h"

using ::testing::_;

class GameControllerTest : public ::testing::Test {
protected:
    static QApplication* app;
    GameController* controller;
    MockWheel* mockWheel;

    void SetUp() override {
        if (!app) {
            int argc = 0;
            app = new QApplication(argc, nullptr);
        }

        controller = new GameController(0); // easy difficulty

        // Create mock wheel and inject
        mockWheel = new MockWheel();
        controller->setWheel(mockWheel);
    }

    void TearDown() override {
        delete controller;  // mockWheel deleted inside controller
    }
};

QApplication* GameControllerTest::app = nullptr;

// -------------------- UI Existence Tests --------------------

// Labels exist
TEST_F(GameControllerTest, LabelsExist) {
    EXPECT_NE(controller->getPhraseLabel(), nullptr);
    EXPECT_NE(controller->getCategoryLabel(), nullptr);
    EXPECT_NE(controller->getTimerLabel(), nullptr);
}

// Buttons exist
TEST_F(GameControllerTest, ButtonsExist) {
    EXPECT_NE(controller->getSpinButton(), nullptr);
    EXPECT_NE(controller->getBuyVowelButton(), nullptr);
    EXPECT_NE(controller->getBuyHintButton(), nullptr);
    EXPECT_NE(controller->getSolveButton(), nullptr);
}

// -------------------- Wheel Interaction Tests --------------------

// spinWheel calls the wheel
TEST_F(GameControllerTest, SpinWheelCallsWheel) {
    EXPECT_CALL(*mockWheel, spinWheel()).Times(1);
    controller->testSpinWheel();  // use public wrapper
}

// -------------------- Wheel State Tests --------------------

// Test that spinning flag is updated
TEST_F(GameControllerTest, WheelSpinningFlag) {
    mockWheel->setSpinning(false);
    EXPECT_FALSE(mockWheel->spinning());

    EXPECT_CALL(*mockWheel, spinWheel()).WillOnce([this]() {
        mockWheel->setSpinning(true);  // simulate spin
    });

    controller->testSpinWheel();
    EXPECT_TRUE(mockWheel->spinning());
}

// -------------------- Signal Tests --------------------

// Test that landedSegment signal is emitted when stopping wheel
TEST_F(GameControllerTest, LandedSegmentSignalEmitted) {
    Wheel realWheel;
    QSignalSpy spy(&realWheel, &Wheel::landedSegment);

    realWheel.spinWheel();
    realWheel.stopSpin();

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(GameControllerTest, SpinWheelWhileAlreadySpinning) {
    mockWheel->setSpinning(true);

    EXPECT_CALL(*mockWheel, spinWheel()).Times(0);  // should not spin again
    controller->testSpinWheel();

    EXPECT_TRUE(mockWheel->spinning());  // spinning flag should remain true
}




