#include "logger.hpp"

#include <gtest/gtest.h>

using Level  = ::yaul::LogLevel;
using string = ::yaul::string;

class Logger : public ::testing::Test {
 protected:
  Level latestLogLevel;
  string latestLogOutput;

  void logFunction(Level level, const char* msg) noexcept {
    latestLogLevel  = level;
    latestLogOutput = msg;
  }

  static void staticLogFunction(Level level, const char* msg) noexcept {
    if (object != nullptr)
      object->logFunction(level, msg);
  }

  static Logger* object;
  virtual void SetUp() {
    object = this;
    ::yaul::Logger::instance().setLogger(&staticLogFunction);
  }

  virtual void TearDown() {
    ::yaul::Logger::instance().setLogger(nullptr);
    object = nullptr;
  }
};

Logger* Logger::object = nullptr;

TEST_F(Logger, NoLogger) {
  ::yaul::Logger::instance().setLogger(nullptr);
  EXPECT_NO_THROW(
      ::yaul::Logger::instance().log(Level::critical, "This should not print"));
  EXPECT_TRUE(latestLogOutput.empty());
}

TEST_F(Logger, LogLevels) {
  string msg;

  msg = "This is a debug log";
  ::yaul::Logger::instance().log(Level::debug, msg);
  EXPECT_EQ(latestLogLevel, Level::debug);
  EXPECT_EQ(latestLogOutput.compare(msg), int{0});

  msg = "This is an info log";
  ::yaul::Logger::instance().log(Level::info, msg);
  EXPECT_EQ(latestLogLevel, Level::info);
  EXPECT_EQ(latestLogOutput.compare(msg), int{0});

  msg = "This is a warning log";
  ::yaul::Logger::instance().log(Level::warning, msg);
  EXPECT_EQ(latestLogLevel, Level::warning);
  EXPECT_EQ(latestLogOutput.compare(msg), int{0});

  msg = "This is an error log";
  ::yaul::Logger::instance().log(Level::error, msg);
  EXPECT_EQ(latestLogLevel, Level::error);
  EXPECT_EQ(latestLogOutput.compare(msg), int{0});

  msg = "This is a critical log";
  ::yaul::Logger::instance().log(Level::critical, msg);
  EXPECT_EQ(latestLogLevel, Level::critical);
  EXPECT_EQ(latestLogOutput.compare(msg), int{0});

  msg = "This is an off log";
  ::yaul::Logger::instance().log(Level::off, msg);
  EXPECT_EQ(latestLogLevel, Level::off);
  EXPECT_EQ(latestLogOutput.compare(msg), int{0});
}
