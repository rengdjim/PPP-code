#include <gtest/gtest.h>

#include <sstream>
#include <vector>

#include "lexer.h"

class TokenStreamV2Test : public ::testing::Test {
protected:
    TokenStreamV2Test() :ts(iss) {}

    std::istringstream iss;
    Token_stream ts;
};

TEST_F(TokenStreamV2Test, Get) {
    iss.str("2; 2+3*5%4; 1 +\t2\n* 3; 12.5/(2.3-.4);"
            "let var = 7.2; x+y*2;"
            "Hello world;"
            "srtvrqtiewcbet7rewaewre-wqcntrretewru*754389652743nvcqnwq;"
            "q");
    std::vector<Token> expected = {
        {number, 2}, {print},
        {number, 2}, {'+'}, {number, 3}, {'*'}, {number, 5}, {'%'}, {number, 4}, {print},
        {number, 1}, {'+'}, {number, 2}, {'*'}, {number, 3}, {print},
        {number, 12.5}, {'/'}, {'('}, {number, 2.3}, {'-'}, {number, 0.4}, {')'}, {print},
        {let}, {name, "var"}, {'='}, {number, 7.2}, {print},
        {name, "x"}, {'+'}, {name, "y"}, {'*'}, {number, 2}, {print},
        {name, "Hello"}, {name, "world"}, {print},
        {name, "srtvrqtiewcbet7rewaewre"}, {'-'}, {name, "wqcntrretewru"},
        {'*'}, {number, 754389652743.0}, {name, "nvcqnwq"}, {print},
        {quit}
    };

    for (Token e : expected) {
        Token t = ts.get();
        EXPECT_EQ(t.kind, e.kind);
        if (t.kind == number)
            EXPECT_DOUBLE_EQ(t.value, e.value);
        else if (t.kind == name)
            EXPECT_EQ(t.name, e.name);
    }
}

TEST_F(TokenStreamV2Test, GetBadToken) {
    std::vector<std::string> input = {"!", "@", "&"};
    for (const auto& s : input) {
        iss.str(s);
        EXPECT_THROW(ts.get(), Lexer_error) << "input: " << s;
        ts.ignore();
    }
}

TEST_F(TokenStreamV2Test, PutBack) {
    iss.str("+");
    ts.putback({number, 45});
    EXPECT_DOUBLE_EQ(ts.get().value, 45);
    EXPECT_EQ(ts.get().kind, '+');
}

TEST_F(TokenStreamV2Test, PutBackFull) {
    ts.putback({'-'});
    EXPECT_THROW(ts.putback({number, 42}), Lexer_error);
}

TEST_F(TokenStreamV2Test, Ignore) {
    iss.str("+");
    ts.putback({number, 45});
    ts.ignore();
    EXPECT_EQ(ts.get().kind, '+');
}

TEST_F(TokenStreamV2Test, IgnoreCharacter) {
    iss.str("2*3; 4+5;");
    ts.ignore(print);
    EXPECT_EQ(ts.get().value, 4);
    ts.ignore();

    iss.str("1@z; 2+3;");
    ts.ignore('2');
    EXPECT_EQ(ts.get().kind, '+');
    ts.ignore();
}
