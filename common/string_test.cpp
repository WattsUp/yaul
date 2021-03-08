#include "string.hpp"

#include <gtest/gtest.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
TEST(WideChar, EmptyString) {
  yaul::WideChar string{u8""};
  EXPECT_EQ(wchar_t{0}, *string.c_str());
}

TEST(WideChar, ASCIIString) {
  yaul::WideChar string{u8"Hello world!"};
  EXPECT_STREQ(L"Hello world!", string.c_str());
}

TEST(WideChar, Unicode) {
  yaul::WideChar string{u8"Hello world!🌍"};
  EXPECT_STREQ(L"Hello world!🌍", string.c_str());
}

TEST(WideChar, ReverseEmptyString) {
  yaul::string string = yaul::WideChar::rev(L"");
  EXPECT_TRUE(string.empty());
}

TEST(WideChar, ReverseASCIIString) {
  yaul::string string = yaul::WideChar::rev(L"Hello world!");
  EXPECT_EQ(yaul::string{u8"Hello world!"}.compare(string), int{0});
}

TEST(WideChar, ReverseUnicode) {
  yaul::string string = yaul::WideChar::rev(L"Hello world!🌍");
  EXPECT_EQ(yaul::string{u8"Hello world!🌍"}.compare(string), int{0});
}

#endif /* WIN32 */
