class MockDifficulty : public Difficulty {
public:
    MOCK_METHOD(void, easyClicked, (), (override));
    MOCK_METHOD(void, hardClicked, (), (override));
};

TEST(DifficultyMockTest, EasyClickedCalled) {
    MockDifficulty dlg;

    EXPECT_CALL(dlg, easyClicked())
        .Times(1);

    // simulate button click
    dlg.easyClicked(); // should trigger the mock
}
