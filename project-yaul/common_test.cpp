#include <yaul/common.hpp>

#include "common/string.hpp"
#include "common/version.h"

#include <gtest/gtest.h>

using string = ::yaul::string;
using Result = ::yaul::Result;

class Common : public ::testing::Test {
 protected:
  ::yaul::string initialConfig;
  int min;
  int max;

  static constexpr char const* exceptionMessage{
      "Exception was requested to be thrown"};

  void voidFunction(bool throwException) noexcept(false) {
    YAUL_EXCEPTION_WRAPPER_THROW_VOID(apiVoidFunction, throwException);
  }

  void apiVoidFunction(bool throwException, Result* r) noexcept {
    YAUL_EXCEPTION_WRAPPER_CATCH(implVoidFunction(throwException));
  }

  void implVoidFunction(bool throwException) noexcept(false) {
    if (throwException)
      throw std::runtime_error(exceptionMessage);
  }

  bool function(bool throwException, bool secondParameter) noexcept(false) {
    YAUL_EXCEPTION_WRAPPER_THROW(bool, apiFunction, throwException,
                                 secondParameter);
  }

  bool apiFunction(bool throwException,
                   bool secondParameter,
                   Result* r) noexcept {
    YAUL_EXCEPTION_WRAPPER_CATCH(
        return implFunction(throwException, secondParameter));
    return false;
  }

  bool implFunction(bool throwException, bool secondParameter) noexcept(false) {
    if (throwException || secondParameter)
      throw std::runtime_error(exceptionMessage);
    return true;
  }

  virtual void SetUp() {
    min = rand();
    max = rand();
    if (min > max)
      std::swap(min, max);
    initialConfig += "  min         = " + std::to_string(min) + ";\n";
    initialConfig += "  max         = " + std::to_string(max) + ";\n";
  }

  virtual void TearDown() {
    if (::testing::Test::HasFailure()) {
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_RED,
                                         "Failing Test Configuration\n");
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_DEFAULT, "%s",
                                         initialConfig.c_str());
    }
  }
};

TEST_F(Common, RandRange) {
  for (int i = 0; i < 65536; ++i) {
    auto random = ::yaul::randRange(min, max);
    ASSERT_GE(random, min);
    ASSERT_LE(random, max);
  }
}

TEST_F(Common, VersionString) {
  EXPECT_STREQ(VERSION_STRING, ::yaul::getVersionString());
  EXPECT_STREQ(VERSION_STRING_FULL, ::yaul::getVersionStringFull());
}

TEST_F(Common, decodeHexChar) {
  EXPECT_EQ(uint8_t{0}, ::yaul::decodeHexChar('0'));
  EXPECT_EQ(uint8_t{1}, ::yaul::decodeHexChar('1'));
  EXPECT_EQ(uint8_t{9}, ::yaul::decodeHexChar('9'));
  EXPECT_EQ(uint8_t{10}, ::yaul::decodeHexChar('A'));
  EXPECT_EQ(uint8_t{11}, ::yaul::decodeHexChar('B'));
  EXPECT_EQ(uint8_t{15}, ::yaul::decodeHexChar('F'));
  EXPECT_EQ(uint8_t{10}, ::yaul::decodeHexChar('a'));
  EXPECT_EQ(uint8_t{11}, ::yaul::decodeHexChar('b'));
  EXPECT_EQ(uint8_t{15}, ::yaul::decodeHexChar('f'));
  EXPECT_THROW(::yaul::decodeHexChar('@'), std::invalid_argument);
  EXPECT_THROW(::yaul::decodeHexChar('`'), std::invalid_argument);
  EXPECT_THROW(::yaul::decodeHexChar('G'), std::invalid_argument);
  EXPECT_THROW(::yaul::decodeHexChar('g'), std::invalid_argument);
  EXPECT_THROW(::yaul::decodeHexChar('/'), std::invalid_argument);
  EXPECT_THROW(::yaul::decodeHexChar(':'), std::invalid_argument);
}

TEST_F(Common, Success) {
  auto result = ::yaul::Result();
  EXPECT_FALSE(result.failed());
  EXPECT_EQ(nullptr, static_cast<char*>(result));
}

TEST_F(Common, Failure) {
  ::yaul::string msg{"This is the reason the result is failed"};
  auto result = ::yaul::Result(msg.c_str());
  EXPECT_TRUE(result.failed());
  EXPECT_STREQ(msg.c_str(), static_cast<char*>(result));
}

TEST_F(Common, ExceptionWrapper) {
  EXPECT_NO_THROW(voidFunction(false));
  EXPECT_THROW(voidFunction(true), std::runtime_error);
  EXPECT_NO_THROW(function(false, false));
  EXPECT_TRUE(function(false, false));
  EXPECT_THROW(function(true, false), std::runtime_error);

  try {
    function(true, false);
  } catch (const std::exception& e) {
    EXPECT_STREQ(e.what(), exceptionMessage);
  }
}