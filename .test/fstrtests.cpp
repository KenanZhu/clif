/**
    \file : fstrtests.cpp
    \brief : Unit test for 'CLIF::FStr'.
 */

#include <CLIF.hpp>
#include "fstrtestfixtures.h"

/**
    \note :
    In the pre-test note, the ‘CLIF::FStr’ class is a utility class in the CLIF
    framework primarily handling basic string 
    operations such as splitting, concatenating, and formatting.

    For the specific static methods included in this class, please refer to the
    actual implementation of CLIF::FStr.
 */

///////////////////////////////////////////////////////////////////////////////
/// CLIF::FStr

/// --Public--

/**
    \func : CLIF::FStr::trim.
    \note :
    These test case have 4 tests in total. Mainly check methods of class below:

    static std::string trim(const std::string &str, int mode = 0);
 */
TEST(TrimTest, Trim_Basic)
{
    EXPECT_EQ(CLIF::FStr::trim("  a  "),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::trim("a  "),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::trim("  a"),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::trim("  a  b  c  "),
              std::string("a  b  c"));
}
TEST(TrimTest, Trim_Mode_1)
{
    EXPECT_EQ(CLIF::FStr::trim("  a  ", 1),
              std::string("  a"));

    EXPECT_EQ(CLIF::FStr::trim("a  ", 1),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::trim("  a  b  c  ", 1),
              std::string("  a  b  c"));

    EXPECT_EQ(CLIF::FStr::trim("", 1),
              std::string(""));
}
TEST(TrimTest, Trim_Mode_2)
{
    EXPECT_EQ(CLIF::FStr::trim("  a  ", 2),
              std::string("a  "));

    EXPECT_EQ(CLIF::FStr::trim("  a", 2),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::trim("  a  b  c  ", 2),
              std::string("a  b  c  "));

    EXPECT_EQ(CLIF::FStr::trim("", 2),
              std::string(""));
}
TEST(TrimTest, Trim_Special)
{
    EXPECT_EQ(CLIF::FStr::trim("\t\n  a \t\n"),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::trim("   "),
              std::string(""));

    EXPECT_EQ(CLIF::FStr::trim("a"),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::trim(" a b c "),
              std::string("a b c"));
}

/**
    \func : CLIF::FStr::toLower.
    \note :
    These test case have 2 tests in total. Mainly check methods of class below:

    static std::string toLower(const std::string &str);
 */
TEST(ToLowerTest, ToLower_Basic)
{
    EXPECT_EQ(CLIF::FStr::toLower("a"),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::toLower("A"),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::toLower("aBc"),
              std::string("abc"));

    EXPECT_EQ(CLIF::FStr::toLower(""),
              std::string(""));
}
TEST(ToLowerTest, ToLower_Special)
{
    EXPECT_EQ(CLIF::FStr::toLower("aBc"),
              std::string("abc"));

    EXPECT_EQ(CLIF::FStr::toLower("+=awQE"),
              std::string("+=awqe"));

    EXPECT_EQ(CLIF::FStr::toLower("123"),
              std::string("123"));

    EXPECT_EQ(CLIF::FStr::toLower("!@#$%^&*()_+"),
              std::string("!@#$%^&*()_+"));
}

/**
    \func : CLIF::FStr::toUpper.
    \note :
    These test case have 2 tests in total. Mainly check methods of class below:

    static std::string toUpper(const std::string &str);
 */
TEST(ToUpperTest, ToUpper_Basic)
{
    EXPECT_EQ(CLIF::FStr::toUpper("a"),
              std::string("A"));

    EXPECT_EQ(CLIF::FStr::toUpper("A"),
              std::string("A"));

    EXPECT_EQ(CLIF::FStr::toUpper("aBc"),
              std::string("ABC"));

    EXPECT_EQ(CLIF::FStr::toUpper(""),
              std::string(""));
}
TEST(ToUpperTest, ToUpper_Special)
{
    EXPECT_EQ(CLIF::FStr::toUpper("aBc"),
              std::string("ABC"));

    EXPECT_EQ(CLIF::FStr::toUpper("+=awQE"),
              std::string("+=AWQE"));

    EXPECT_EQ(CLIF::FStr::toUpper("123"),
              std::string("123"));

    EXPECT_EQ(CLIF::FStr::toUpper("!@#$%^&*()_+"),
              std::string("!@#$%^&*()_+"));
}

/**
    \func : CLIF::FStr::onlyAlpha.
    \note :
    These test case have 2 tests in total. Mainly check methods of class below:

    static std::string onlyAlpha(const std::string &str);
 */
TEST(OnlyAlphaTest, OnlyAlpha_Basic)
{
    EXPECT_EQ(CLIF::FStr::onlyAlpha("a"),
              std::string("a"));

    EXPECT_EQ(CLIF::FStr::onlyAlpha("A"),
              std::string("A"));

    EXPECT_EQ(CLIF::FStr::onlyAlpha("1a2B3c"),
              std::string("aBc"));

    EXPECT_EQ(CLIF::FStr::onlyAlpha("123"),
              std::string(""));
}
TEST(OnlyAlphaTest, OnlyAlpha_Special)
{
    EXPECT_EQ(CLIF::FStr::onlyAlpha("aBc123"),
              std::string("aBc"));

    EXPECT_EQ(CLIF::FStr::onlyAlpha("123abc"),
              std::string("abc"));

    EXPECT_EQ(CLIF::FStr::onlyAlpha("&@123-+*"),
              std::string(""));

    EXPECT_EQ(CLIF::FStr::onlyAlpha("%$aBc-+*"),
              std::string("aBc"));
}

/**
    \func : CLIF::FStr::onlyAlNum.
    \note :
    These test case have 2 tests in total. Mainly check methods of class below:

    static std::string onlyAlNum(const std::string &str);
 */
TEST(OnlyAlNumTest, OnlyAlNum_Basic)
{
    EXPECT_EQ(CLIF::FStr::onlyAlNum("aBc123"),
              std::string("aBc123"));

    EXPECT_EQ(CLIF::FStr::onlyAlNum("123abc"),
              std::string("123abc"));

    EXPECT_EQ(CLIF::FStr::onlyAlNum("1A2b3c*"),
              std::string("1A2b3c"));

    EXPECT_EQ(CLIF::FStr::onlyAlNum(""),
              std::string(""));
}
TEST(OnlyAlNumTest, OnlyAlNum_Special)
{
    EXPECT_EQ(CLIF::FStr::onlyAlNum("(aBc)<123>*"),
              std::string("aBc123"));

    EXPECT_EQ(CLIF::FStr::onlyAlNum(";a[B|c,1.2.3;"),
              std::string("aBc123"));

    EXPECT_EQ(CLIF::FStr::onlyAlNum(" A b C 88"),
              std::string("AbC88"));

    EXPECT_EQ(CLIF::FStr::onlyAlNum("-+=\\|"),
              std::string(""));
}

/**
    \func : CLIF::FStr::splitBy.
    \note :
    These test case have 4 tests in total. Mainly check methods of class below:

    static std::vector<std::string> splitBy(const std::string &str,
                                            const char &delimiter);
    static std::vector<std::string> splitBy(const std::string &str,
                                            const std::string &delimiter);
 */
TEST(SplitByTest, Split_Basic)
{
    EXPECT_EQ(CLIF::FStr::splitBy("a,b,c", ','),
              std::vector<std::string>({"a", "b", "c"}));

    EXPECT_EQ(CLIF::FStr::splitBy("x|y|z", '|'),
              std::vector<std::string>({"x", "y", "z"}));

    EXPECT_EQ(CLIF::FStr::splitBy(",a,b,", ','),
              std::vector<std::string>({"a", "b"}));

    EXPECT_EQ(CLIF::FStr::splitBy("1,2", ','),
              std::vector<std::string>({"1", "2"}));
}
TEST(SplitByTest, Split_Special)
{
    EXPECT_EQ(CLIF::FStr::splitBy("", ','),
              std::vector<std::string>({}));

    EXPECT_EQ(CLIF::FStr::splitBy("single", ','),
              std::vector<std::string>({"single"}));

    EXPECT_EQ(CLIF::FStr::splitBy(",", ','),
              std::vector<std::string>({}));

    EXPECT_EQ(CLIF::FStr::splitBy(",,", ','),
              std::vector<std::string>({}));
}
TEST(SplitByTest, Split_Special_Characters)
{
    EXPECT_EQ(CLIF::FStr::splitBy("a$,b$", '$'),
              std::vector<std::string>({"a", ",b"}));

    EXPECT_EQ(CLIF::FStr::splitBy("\\n,\\t", ','),
              std::vector<std::string>({"\\n", "\\t"}));

    EXPECT_EQ(CLIF::FStr::splitBy("汉字,文字", ','),
              std::vector<std::string>({"汉字", "文字"}));

    EXPECT_EQ(CLIF::FStr::splitBy("😀,😎", ','),
              std::vector<std::string>({"😀", "😎"}));
}
TEST(SplitByTest, Split_Special_Edge)
{
    EXPECT_EQ(CLIF::FStr::splitBy(" my name is  :  .", ' '),
              std::vector<std::string>({"my", "name", "is", ":", "."}));

    EXPECT_EQ(CLIF::FStr::splitBy("...config.cursor.animation.duration...", '.'),
              std::vector<std::string>({"config", "cursor", "animation", "duration"}));

    EXPECT_EQ(CLIF::FStr::splitBy("-=+-_/[-]", '-'),
              std::vector<std::string>({"=+", "_/[", "]"}));

    EXPECT_EQ(CLIF::FStr::splitBy("have an apple", 'a'),
              std::vector<std::string>({"h", "ve ", "n ", "pple"}));
}

/**
    \func : CLIF::FStr::splitByBrackets.
    \note :
    These test case have 8 tests in total. Mainly check methods of class below:

    static std::vector<std::string> splitByBrackets(const std::string &str, const char &delimiter, bool strip = true);
    static std::vector<std::string> splitByBrackets(const std::string &str, const std::string &delimiter, bool strip = true);
 */
TEST(SplitByBracketsTest, Split_Basic)
{
    EXPECT_EQ(CLIF::FStr::splitByBrackets("a,b,c", ','),
              std::vector<std::string>({"a", "b", "c"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("x|y|z", '|'),
              std::vector<std::string>({"x", "y", "z"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets(",a,b,", ','),
              std::vector<std::string>({"", "a", "b", ""}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("1,,2", ','),
              std::vector<std::string>({"1", "", "2"}));
}
TEST(SplitByBracketsTest, Split_Special) {
    EXPECT_EQ(CLIF::FStr::splitByBrackets("", ','),
              std::vector<std::string>({""}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("single", ','),
              std::vector<std::string>({"single"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets(",", ','),
              std::vector<std::string>({"", ""}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets(",,", ','),
              std::vector<std::string>({"", "", ""}));
}
TEST(SplitByBracketsTest, Split_Special_Characters)
{
    EXPECT_EQ(CLIF::FStr::splitByBrackets("a$,b$", '$'),
              std::vector<std::string>({"a", ",b", ""}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("\\n,\\t", ','),
              std::vector<std::string>({"\\n", "\\t"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("汉字,文字", ','),
              std::vector<std::string>({"汉字", "文字"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("😀,😎", ','),
              std::vector<std::string>({"😀", "😎"}));
}
TEST(SplitByBracketsTest, Split_WithBracket)
{
    EXPECT_EQ(CLIF::FStr::splitByBrackets("a,(b,c),d", ','),
              std::vector<std::string>({"a", "b,c", "d"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("(a),b", ','),
              std::vector<std::string>({"a", "b"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("a,b)", ','),
              std::vector<std::string>({"a", "b)"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("a,(b,c", ','),
              std::vector<std::string>({"a", "(b,c"}));
}
TEST(SplitByBracketsTest, Split_WithBracket_Nested)
{
    EXPECT_EQ(CLIF::FStr::splitByBrackets("1,(2,(3,4)),5", ','),
              std::vector<std::string>({"1", "2,(3,4)", "5"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("(a,(b,c))", ','),
              std::vector<std::string>({"a,(b,c)"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("x,(y,z),w", ','),
              std::vector<std::string>({"x", "y,z", "w"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("a,(b,(c,d),e)", ','),
              std::vector<std::string>({"a", "b,(c,d),e"}));
}
TEST(SplitByBracketsTest, Split_WithBracket_Mixed)
{
    EXPECT_EQ(CLIF::FStr::splitByBrackets("[9],{8},<0>,(1)", ','),
              std::vector<std::string>({"9", "8", "0", "1"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("9>,[8,0],1)", ','),
              std::vector<std::string>({"9>", "8,0", "1)"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("a(b,c),<d>", ','),
              std::vector<std::string>({"a(b,c)", "d"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("[a],b(c,d)", ','),
              std::vector<std::string>({"a", "b(c,d)"}));
}
TEST(SplitByBracketsTest, Split_WithBracket_WithBlank)
{
    // default : strip=true
    EXPECT_EQ(CLIF::FStr::splitByBrackets(" a , b ", ','),
              std::vector<std::string>({" a ", " b "}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets(" 1 , ( 2 , 3 ) ", ','),
              std::vector<std::string>({" 1 ", " ( 2 , 3 ) "}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("x y z", ' '),
              std::vector<std::string>({"x", "y", "z"}));

    // set : strip=false
    EXPECT_EQ(CLIF::FStr::splitByBrackets("{ a },{ b }", ',', false),
              std::vector<std::string>({"{ a }", "{ b }"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets(" ( 1 , 2 ) ", ',', false),
              std::vector<std::string>({" ( 1 , 2 ) "}));
}
TEST(SplitByBracketsTest, Split_WithBracket_Edge)
{
    EXPECT_EQ(CLIF::FStr::splitByBrackets("())", ','),
              std::vector<std::string>({")"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("a,(,),b", ','),
              std::vector<std::string>({"a", ",", "b"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets("a(b)c,d", ','),
              std::vector<std::string>({"a(b)c", "d"}));

    EXPECT_EQ(CLIF::FStr::splitByBrackets(">)a,b],{c}", ','),
              std::vector<std::string>({">)a", "b]", "c"}));
}