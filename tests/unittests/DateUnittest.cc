#include "core/math/date.h"
#include <gtest/gtest.h>
#include <string>
#include <iostream>

TEST(Date, constructorTest)
{
    EXPECT_STREQ("1985-01-01 00:00:00",
                 Date(1985, 1, 1)
                     .toCustomedFormattedStringLocal("%Y-%m-%d %H:%M:%S")
                     .c_str());
    EXPECT_STREQ("2004-02-29 00:00:00.000000",
                 Date(2004, 2, 29)
                     .toCustomedFormattedStringLocal("%Y-%m-%d %H:%M:%S", true)
                     .c_str());
    EXPECT_STRNE("2001-02-29 00:00:00.000000",
                 Date(2001, 2, 29)
                     .toCustomedFormattedStringLocal("%Y-%m-%d %H:%M:%S", true)
                     .c_str());
    EXPECT_STREQ("2018-01-01 00:00:00.000000",
                 Date(2018, 1, 1, 12, 12, 12, 2321)
                     .roundDay()
                     .toCustomedFormattedStringLocal("%Y-%m-%d %H:%M:%S", true)
                     .c_str());
}
TEST(Date, DatabaseStringTest)
{
    auto now = Date::now();
    EXPECT_EQ(now, Date::fromDbStringLocal(now.toDbStringLocal()));
    std::string dbString = "2018-01-01 00:00:00.123";
    auto dbDate = Date::fromDbStringLocal(dbString);
    auto ms = (dbDate.microSecondsSinceEpoch() % 1000000) / 1000;
    EXPECT_EQ(ms, 123);
    dbString = "2018-01-01 00:00:00";
    dbDate = Date::fromDbStringLocal(dbString);
    ms = (dbDate.microSecondsSinceEpoch() % 1000000) / 1000;
    EXPECT_EQ(ms, 0);
}
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}