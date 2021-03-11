#include <yaul/monitor.hpp>

#include "monitor_mocked.hpp"

#include <gtest/gtest.h>

TEST(Monitor, Enumeration) {
  auto monitors = ::yaul::Monitor::enumerate();
  ASSERT_EQ(monitors.size(), MonitorMock::count);

  for (const auto& monitorInfo : MonitorMock::monitors) {
    auto& monitor = monitors.front();

    EXPECT_STREQ(monitor.getName(), monitorInfo.name);
    EXPECT_EQ(monitor.getSize().width, monitorInfo.rect.width);
    EXPECT_EQ(monitor.getSize().height, monitorInfo.rect.height);
    EXPECT_FLOAT_EQ(monitor.getDPMM(), monitorInfo.dpmm);
    EXPECT_EQ(monitor.getPosition().x, monitorInfo.rect.x);
    EXPECT_EQ(monitor.getPosition().y, monitorInfo.rect.y);
    EXPECT_EQ(monitor.isPrimary(), monitorInfo.primary);

    monitors.pop_front();
  }
}