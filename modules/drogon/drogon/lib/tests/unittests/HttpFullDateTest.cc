#include <drogon/drogon_test.h>
#include <drogon/utils/Utilities.h>
#include <iostream>
#include <string>

using namespace drogon;
DROGON_TEST(HttpFullDateTest) {
	auto str = utils::getHttpFullDate();
	auto date = utils::getHttpDate(str);
	CHECK(utils::getHttpFullDate(date) == str);
}
