#include <gtest/gtest.h>
#include "PhraseHandler.h"

// Helper function to remove spaces from displayed phrase for testing
QString removeSpaces(const QString &str) {
    QString result = str;
    result.remove(' ');
    return result;
}

// ------------------- PhraseHandler Tests -------------------

// Test correct and incorrect letters
TEST(PhraseHandlerTest, GuessLetters) {
    PhraseHandler handler("HELLO");

    // Correct guess
    EXPECT_TRUE(handler.guessLetter('H'));
    EXPECT_TRUE(removeSpaces(handler.getDisplayedPhrase()).contains('H'));

    // Incorrect guess
    EXPECT_FALSE(handler.guessLetter('X'));

    // Guess remaining letters
    EXPECT_TRUE(handler.guessLetter('E'));
    EXPECT_TRUE(handler.guessLetter('L'));
    EXPECT_TRUE(handler.guessLetter('O'));

    // Phrase should now be complete
    EXPECT_TRUE(handler.isComplete());
    EXPECT_EQ(removeSpaces(handler.getDisplayedPhrase()), "HELLO");
}

// Test repeated letter guesses
TEST(PhraseHandlerTest, RepeatedLetterGuess) {
    PhraseHandler handler("BALLOON");

    EXPECT_TRUE(handler.guessLetter('L'));
    EXPECT_FALSE(handler.guessLetter('L'));  // guessing again should return false
    EXPECT_TRUE(removeSpaces(handler.getDisplayedPhrase()).contains('L'));
}

// Test revealing the full phrase
TEST(PhraseHandlerTest, RevealPhrase) {
    PhraseHandler handler("WORLD");

    EXPECT_FALSE(handler.isComplete());

    handler.revealPhrase();

    EXPECT_TRUE(handler.isComplete());
    EXPECT_EQ(removeSpaces(handler.getDisplayedPhrase()), "WORLD");
}

// Test guessing letters in lowercase
TEST(PhraseHandlerTest, GuessLowercaseLetters) {
    PhraseHandler handler("TEST");

    EXPECT_TRUE(handler.guessLetter('t')); // lowercase should be accepted
    EXPECT_TRUE(removeSpaces(handler.getDisplayedPhrase()).contains('T'));
}

// Test phrase completion edge case
TEST(PhraseHandlerTest, CompletePhraseEdgeCase) {
    PhraseHandler handler("A");  // single-letter phrase

    EXPECT_FALSE(handler.isComplete());

    EXPECT_TRUE(handler.guessLetter('A'));
    EXPECT_TRUE(handler.isComplete());
    EXPECT_EQ(removeSpaces(handler.getDisplayedPhrase()), "A");
}

// Test phrase with spaces
TEST(PhraseHandlerTest, PhraseWithSpaces) {
    PhraseHandler handler("HI THERE");

    EXPECT_TRUE(handler.guessLetter('H'));
    EXPECT_TRUE(handler.guessLetter('I'));
    EXPECT_TRUE(handler.guessLetter('T'));
    EXPECT_TRUE(handler.guessLetter('E'));
    EXPECT_TRUE(handler.guessLetter('R'));

    EXPECT_TRUE(handler.isComplete());
    EXPECT_EQ(removeSpaces(handler.getDisplayedPhrase()), "HITHERE");
}

// ------------------- Main function -------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

