#include "logging.hpp"

#include <gtest/gtest.h>

TEST(Logging, NoSinks) {
  common::logging::configure(nullptr);
#ifdef DEBUG
  EXPECT_EQ(size_t{1}, spdlog::default_logger()->sinks().size());
#else /* !DEBUG */
  EXPECT_EQ(size_t{0}, spdlog::default_logger()->sinks().size());
#endif
}

TEST(Logging, FileSink) {
  common::logging::configure("log.log");
#ifdef DEBUG
  EXPECT_EQ(size_t{2}, spdlog::default_logger()->sinks().size());
#else /* !DEBUG */
  EXPECT_EQ(size_t{1}, spdlog::default_logger()->sinks().size());
#endif
  spdlog::debug("debug Test");
  spdlog::info("info Test");
  spdlog::warn("warn Test");
  spdlog::error("error Test");
  spdlog::critical("critical Test");
}
