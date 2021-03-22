#include <yaul/monitor.hpp>

#include "monitor_mocked.hpp"

#include <gtest/gtest.h>

TEST(Monitor, Enumeration) {
  auto monitors = ::yaul::Monitor::enumerate();
  ASSERT_EQ(monitors.size(), MonitorMock::count);

  for (const auto& monitorInfo : MonitorMock::monitors) {
    auto& monitor = monitors.front();

    EXPECT_STREQ(monitor.getName(), monitorInfo.name);
    EXPECT_EQ(monitor.getSize(), monitorInfo.rect);
    EXPECT_FLOAT_EQ(monitor.getDPMM(), monitorInfo.dpmm);
    EXPECT_EQ(monitor.getPosition(), monitorInfo.rect);
    EXPECT_EQ(monitor.isPrimary(), monitorInfo.primary);

    monitors.pop_front();
  }
}