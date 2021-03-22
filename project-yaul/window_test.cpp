#include <yaul/window.hpp>

#include "common/string.hpp"
#include "window_mocked.hpp"

#include <gtest/gtest.h>

class Window : public ::testing::Test {
 protected:
  ::yaul::string initialConfig;

  ::yaul::Size size;
  ::yaul::string title;
  ::yaul::Window::ShowState showState;

  ::yaul::Window* window = nullptr;

  virtual void SetUp() {
    auto& wm = WindowManager::instance(true);

    size.width  = ::yaul::randRange(SystemMetricsMocked::cXMin, 2048);
    size.height = ::yaul::randRange(SystemMetricsMocked::cYMin, 2048);

    title = "Random window title: ";
    // Add emoticons
    for (int i = 0; i < 8; ++i)
      title.push_back(
          static_cast<char32_t>(::yaul::randRange(0x1F601, 0x1F64F)));

    // Add transport and map symbols
    for (int i = 0; i < 8; ++i)
      title.push_back(
          static_cast<char32_t>(::yaul::randRange(0x1F680, 0x1F6C0)));

    // Add Cyrillic glyphs
    for (int i = 0; i < 8; ++i)
      title.push_back(static_cast<char32_t>(::yaul::randRange(0x0400, 0x04FF)));

    showState = ::yaul::Window::ShowState::restore;

    initialConfig += "  size.width:   " + std::to_string(size.width) + "\n";
    initialConfig += "  size.height:  " + std::to_string(size.height) + "\n";
    initialConfig += "  title=       \"" + title + "\";\n";

    ASSERT_NO_THROW(window =
                        new ::yaul::Window(size, title.c_str(), showState));
    wm.processAsyncCalls();
  }

  virtual void TearDown() {
    delete window;
    window = nullptr;

    auto& wm = WindowManager::instance();
    EXPECT_FALSE(wm.anyErrors);
    EXPECT_TRUE(wm.asyncQueue.empty());

    if (::testing::Test::HasFailure()) {
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_RED,
                                         "Failing Test Configuration\n");
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_DEFAULT, "%s",
                                         initialConfig.c_str());
    }
  }
};

TEST_F(Window, Instantiation) {
  // Zero dimensions
  EXPECT_THROW(
      auto a = ::yaul::Window({0, 1}, "", ::yaul::Window::ShowState::hidden),
      std::runtime_error);
  EXPECT_THROW(
      auto b = ::yaul::Window({1, 0}, "", ::yaul::Window::ShowState::hidden),
      std::runtime_error);

  // Instantiation does this
  // createNativeWindow();
  // setSize(size);
  // setTitle(title);
  // setShowState(state);

  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  EXPECT_EQ(wmWindow.title, title);
  EXPECT_EQ(wmWindow.rect, size);
  EXPECT_EQ(wmWindow.showState, showState);
}

TEST_F(Window, CloseRequest) {
  ASSERT_FALSE(window->shouldClose());
  window->closeRequest();
  EXPECT_TRUE(window->shouldClose());
}

TEST_F(Window, CloseButton) {
  ASSERT_FALSE(window->shouldClose());
  auto& wm = WindowManager::instance();
  wm.sendEventToAll(WindowManager::Event::close);
  EXPECT_TRUE(window->shouldClose());
}

TEST_F(Window, DISABLED_Render) {
  // TODO (WattsUp) test render function
}

TEST_F(Window, GetSize) {
  ASSERT_EQ(window->getSize(), size);
}

TEST_F(Window, SetSize) {
  size.width  = ::yaul::randRange(1, 2048);
  size.height = ::yaul::randRange(1, 2048);
  initialConfig += "  size.width:   " + std::to_string(size.width) + "\n";
  initialConfig += "  size.height:  " + std::to_string(size.height) + "\n";

  ASSERT_TRUE(window->setSize(size));
  WindowManager::instance().processAsyncCalls();

  ASSERT_EQ(window->getSize(), size);
}

TEST_F(Window, SetPosition) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  ::yaul::Position position{::yaul::randRange(1, 2048),
                            ::yaul::randRange(1, 2048)};
  initialConfig += "  position.x:   " + std::to_string(position.x) + "\n";
  initialConfig += "  position.y:   " + std::to_string(position.y) + "\n";

  ASSERT_TRUE(window->setPosition(position, nullptr));
  wm.processAsyncCalls();

  EXPECT_EQ(wmWindow.frameRect, position);

  auto monitor = ::yaul::Monitor::enumerate().back();
  ASSERT_TRUE(window->setPosition(position, &monitor));
  wm.processAsyncCalls();
  EXPECT_EQ(wmWindow.frameRect, position + monitor.getPosition());
}

TEST_F(Window, SetTitle) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  title = "Random window title: ";

  // Add emoticons
  for (int i = 0; i < 8; ++i)
    title.push_back(static_cast<char32_t>(::yaul::randRange(0x1F601, 0x1F64F)));

  // Add transport and map symbols
  for (int i = 0; i < 8; ++i)
    title.push_back(static_cast<char32_t>(::yaul::randRange(0x1F680, 0x1F6C0)));

  // Add Cyrillic glyphs
  for (int i = 0; i < 8; ++i)
    title.push_back(static_cast<char32_t>(::yaul::randRange(0x0400, 0x04FF)));

  initialConfig += "  title=       \"" + title + "\";\n";

  window->setTitle(title.c_str());

  EXPECT_EQ(wmWindow.title, title);
}

TEST_F(Window, SetShowState) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  showState = ::yaul::Window::ShowState::hidden;
  window->setShowState(showState);
  wm.processAsyncCalls();
  EXPECT_EQ(wmWindow.showState, showState);
  EXPECT_EQ(window->getShowState(), showState);
  EXPECT_EQ(wmWindow.rect, (::yaul::Rectangle{0, 0, 0, 0}));

  showState = ::yaul::Window::ShowState::minimize;
  window->setShowState(showState);
  wm.processAsyncCalls();
  EXPECT_EQ(wmWindow.showState, showState);
  EXPECT_EQ(window->getShowState(), showState);
  EXPECT_EQ(wmWindow.rect, (::yaul::Rectangle{0, 0, 0, 0}));

  showState = ::yaul::Window::ShowState::maximize;
  window->setShowState(showState);
  wm.processAsyncCalls();
  EXPECT_EQ(wmWindow.showState, showState);
  EXPECT_EQ(window->getShowState(), showState);
  EXPECT_EQ(wmWindow.frameRect, MonitorMock::monitors.front().rect);

  showState = ::yaul::Window::ShowState::restore;
  window->setShowState(showState);
  wm.processAsyncCalls();
  EXPECT_EQ(wmWindow.showState, showState);
  EXPECT_EQ(window->getShowState(), showState);
  EXPECT_EQ(wmWindow.rect, size);
}

TEST_F(Window, GetShowState) {
  auto& wm = WindowManager::instance();

  wm.sendEventToAll(WindowManager::Event::maximize);
  EXPECT_EQ(window->getShowState(), ::yaul::Window::ShowState::maximize);

  wm.sendEventToAll(WindowManager::Event::minimize);
  EXPECT_EQ(window->getShowState(), ::yaul::Window::ShowState::minimize);

  wm.sendEventToAll(WindowManager::Event::restore);
  EXPECT_EQ(window->getShowState(), ::yaul::Window::ShowState::restore);
}

TEST_F(Window, SetBorderless) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  // aeroborderless
  ASSERT_EQ(window->getSize(), size);
  ASSERT_NE(wmWindow.frameRect, wmWindow.rect);

  window->setBorderless(true);
  wm.processAsyncCalls();

  // Client size should be preserved
  ASSERT_EQ(window->getSize(), size);
  ASSERT_EQ(wmWindow.frameRect, wmWindow.rect);

  window->setBorderless(false);
  wm.processAsyncCalls();

  // Client size should be preserved
  ASSERT_EQ(window->getSize(), size);
  ASSERT_NE(wmWindow.frameRect, wmWindow.rect);

  // basicborderless
  wm.compositionEnabled = false;
  window->setBorderless(true);
  wm.processAsyncCalls();

  // Client size should be preserved
  ASSERT_EQ(window->getSize(), size);
  ASSERT_EQ(wmWindow.frameRect, wmWindow.rect);

  window->setBorderless(false);
  wm.processAsyncCalls();

  // Client size should be preserved
  ASSERT_EQ(window->getSize(), size);
  ASSERT_NE(wmWindow.frameRect, wmWindow.rect);
}

TEST_F(Window, SetBorderlessShadow) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  // Composition enabled
  ASSERT_FALSE(wmWindow.frameExtendedIntoClientArea);

  // Not borderless so expecting no change
  window->setBorderlessShadow(false);
  ASSERT_FALSE(wmWindow.frameExtendedIntoClientArea);

  // BorderlessShadow is false and can be realized now
  window->setBorderless(true);
  wm.processAsyncCalls();
  ASSERT_FALSE(wmWindow.frameExtendedIntoClientArea);

  window->setBorderlessShadow(true);
  ASSERT_TRUE(wmWindow.frameExtendedIntoClientArea);

  window->setBorderless(false);
  wm.processAsyncCalls();

  // Composition disabled
  wm.compositionEnabled = false;
  window->setBorderless(true);
  wm.processAsyncCalls();

  window->setBorderlessShadow(false);
  ASSERT_FALSE(wmWindow.frameExtendedIntoClientArea);
  window->setBorderlessShadow(true);
  ASSERT_FALSE(wmWindow.frameExtendedIntoClientArea);
}

TEST_F(Window, SetFullscreen) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  auto originalRect = wmWindow.rect;

  auto monitor = ::yaul::Monitor::enumerate().front();
  window->setFullscreen(true, nullptr);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.rect, wmWindow.frameRect);
  ASSERT_EQ(window->getSize(), monitor.getSize());
  ASSERT_EQ(wmWindow.rect, monitor.getPosition());

  // Move to other monitor
  monitor = ::yaul::Monitor::enumerate().back();
  window->setFullscreen(true, &monitor);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.rect, wmWindow.frameRect);
  ASSERT_EQ(window->getSize(), monitor.getSize());
  ASSERT_EQ(wmWindow.rect, monitor.getPosition());

  // Restore to original state
  window->setFullscreen(false);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.rect, originalRect);
  ASSERT_NE(wmWindow.rect, wmWindow.frameRect);

  // Borderless before will restore to borderless after
  window->setBorderless(true);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.rect, wmWindow.frameRect);
  ASSERT_TRUE(wmWindow.frameExtendedIntoClientArea);
  originalRect = wmWindow.rect;

  window->setFullscreen(true, &monitor);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.rect, wmWindow.frameRect);
  ASSERT_EQ(window->getSize(), monitor.getSize());
  ASSERT_EQ(wmWindow.rect, monitor.getPosition());

  // Restore to original state
  window->setFullscreen(false);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.rect, originalRect);
  ASSERT_EQ(wmWindow.rect, wmWindow.frameRect);
  ASSERT_TRUE(wmWindow.frameExtendedIntoClientArea);

  // Maximize restore test after fullscreening
  originalRect = wmWindow.rect;
  window->setShowState(::yaul::Window::ShowState::maximize);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.showState, ::yaul::Window::ShowState::maximize);
  ASSERT_EQ(window->getShowState(), ::yaul::Window::ShowState::maximize);

  window->setFullscreen(true, &monitor);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.rect, wmWindow.frameRect);
  ASSERT_EQ(window->getSize(), monitor.getSize());
  ASSERT_EQ(wmWindow.rect, monitor.getPosition());

  window->setFullscreen(false);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.rect, wmWindow.frameRect);
  ASSERT_TRUE(wmWindow.frameExtendedIntoClientArea);

  window->setShowState(::yaul::Window::ShowState::restore);
  wm.processAsyncCalls();
  ASSERT_EQ(wmWindow.showState, ::yaul::Window::ShowState::restore);
  ASSERT_EQ(window->getShowState(), ::yaul::Window::ShowState::restore);

  ASSERT_EQ(wmWindow.rect, originalRect);
  ASSERT_EQ(wmWindow.rect, wmWindow.frameRect);
  ASSERT_TRUE(wmWindow.frameExtendedIntoClientArea);
}

TEST_F(Window, SetResizable) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  HWND hWnd      = wm.windows.begin()->first;
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  // Bordered test
  ::yaul::Position cursor;
  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTTOPLEFT);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTTOP);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTTOPRIGHT);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height / 2;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTRIGHT);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBOTTOMRIGHT);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width / 2;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBOTTOM);
  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBOTTOMLEFT);
  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTLEFT);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle +
             SystemMetricsMocked::cYCaption;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCLIENT);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle + 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCAPTION);

  window->setResizable(false);
  wm.processAsyncCalls();

  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBORDER);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBORDER);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBORDER);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height / 2;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBORDER);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBORDER);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width / 2;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBORDER);
  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBORDER);
  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCAPTION);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle +
             SystemMetricsMocked::cYCaption;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCLIENT);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle + 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCAPTION);

  // Borderless test
  window->setResizable(true);
  window->setBorderless(true);
  wm.processAsyncCalls();

  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTTOPLEFT);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTTOP);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTTOPRIGHT);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height / 2;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTRIGHT);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBOTTOMRIGHT);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width / 2;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBOTTOM);
  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTBOTTOMLEFT);
  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTLEFT);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle +
             SystemMetricsMocked::cYCaption;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor),
            (wmWindow.frameRect.height == SystemMetricsMocked::cYMin)
                ? HTBOTTOM
                : HTCLIENT);  // cYMin size windows have no client area pixels
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle + 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCAPTION);

  window->setResizable(false);
  wm.processAsyncCalls();

  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCAPTION);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCAPTION);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCLIENT);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height / 2;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCLIENT);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width - 1;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCLIENT);
  cursor.x = wmWindow.frameRect.x + wmWindow.frameRect.width / 2;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCLIENT);
  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y + wmWindow.frameRect.height - 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCLIENT);
  cursor.x = wmWindow.frameRect.x;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCAPTION);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle +
             SystemMetricsMocked::cYCaption;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCLIENT);
  cursor.x = wmWindow.frameRect.x + SystemMetricsMocked::cXResizeHandle + 1;
  cursor.y = wmWindow.frameRect.y + SystemMetricsMocked::cYResizeHandle + 1;
  EXPECT_EQ(wm.sendHitTest(hWnd, cursor), HTCAPTION);
}

TEST_F(Window, SetResizingBorder) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  HWND hWnd      = wm.windows.begin()->first;
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  // Only affects borderless
  window->setBorderless(true);

  ::yaul::Edges border{::yaul::randRange(0, 16), ::yaul::randRange(0, 16),
                       ::yaul::randRange(0, 16), ::yaul::randRange(0, 16)};
  initialConfig += "  top=          " + std::to_string(border.top) + ";\n";
  initialConfig += "  right=        " + std::to_string(border.right) + ";\n";
  initialConfig += "  bottom=       " + std::to_string(border.bottom) + ";\n";
  initialConfig += "  left=         " + std::to_string(border.left) + ";\n";

  ASSERT_LT(border.top, SystemMetricsMocked::cYSize);

  window->setResizingBorder(border);

  wm.processAsyncCalls();

  auto edges = static_cast<::yaul::Edges>(wmWindow.frameRect);

  ::yaul::Position cursor;
  int hitCount = 0;

  // Top edge
  cursor.x = edges.left + border.left;
  cursor.y = edges.top;
  hitCount = 0;
  for (; cursor.y < (edges.bottom - border.bottom); ++cursor.y) {
    if (wm.sendHitTest(hWnd, cursor) == HTTOP)
      hitCount += 1;
    else
      break;
  }
  EXPECT_EQ(hitCount, border.top);

  // Right edge
  cursor.x = edges.right - 1;
  cursor.y = edges.top + SystemMetricsMocked::cYSize;
  hitCount = 0;
  for (; cursor.x >= (edges.left + border.left); --cursor.x) {
    if (wm.sendHitTest(hWnd, cursor) == HTRIGHT)
      hitCount += 1;
    else
      break;
  }
  EXPECT_EQ(hitCount, border.right);

  // Bottom edge
  cursor.x = edges.left + border.left;
  cursor.y = edges.bottom - 1;
  hitCount = 0;
  for (; cursor.y >= (edges.top + SystemMetricsMocked::cYSize); --cursor.y) {
    if (wm.sendHitTest(hWnd, cursor) == HTBOTTOM)
      hitCount += 1;
    else
      break;
  }
  EXPECT_EQ(hitCount, border.bottom);

  // Left edge
  cursor.x = edges.left;
  cursor.y = edges.top + SystemMetricsMocked::cYSize;
  hitCount = 0;
  for (; cursor.x < (edges.right - border.right); ++cursor.x) {
    if (wm.sendHitTest(hWnd, cursor) == HTLEFT)
      hitCount += 1;
    else
      break;
  }
  EXPECT_EQ(hitCount, border.left);
}

TEST_F(Window, SetDraggingArea) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  HWND hWnd      = wm.windows.begin()->first;
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  // Only affects borderless
  window->setBorderless(true);

  // Remove resizing
  window->setResizable(false);

  int bottom    = ::yaul::randRange(0, size.height);
  int menuWidth = ::yaul::randRange(0, size.width);

  initialConfig += "  bottom=       " + std::to_string(bottom) + ";\n";
  initialConfig += "  menuWidth=    " + std::to_string(menuWidth) + ";\n";

  window->setDraggingArea(bottom, menuWidth);

  wm.processAsyncCalls();

  auto edges = static_cast<::yaul::Edges>(wmWindow.frameRect);

  ::yaul::Position cursor;
  int hitCount = 0;

  // Vertical test
  cursor.x = edges.left;
  cursor.y = edges.top;
  hitCount = 0;
  for (; cursor.y < (edges.bottom + bottom); ++cursor.y) {
    if (wm.sendHitTest(hWnd, cursor) == HTCAPTION)
      hitCount += 1;
    else
      break;
  }
  // Full width menu will have no draggable pixels
  EXPECT_EQ(hitCount, (menuWidth == size.width) ? 0 : bottom);

  // Horizontal test
  cursor.x = edges.left;
  cursor.y = edges.top;
  hitCount = 0;
  for (; cursor.x < (edges.right); ++cursor.x) {
    if (wm.sendHitTest(hWnd, cursor) == HTCAPTION)
      hitCount += 1;
    else
      break;
  }
  // Zero height region will have no draggable pixels
  EXPECT_EQ(hitCount, (bottom == 0) ? 0 : (size.width - menuWidth));
}

TEST_F(Window, DraggingAreaMask) {
  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  HWND hWnd      = wm.windows.begin()->first;
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  // Only affects borderless
  window->setBorderless(true);
  wm.processAsyncCalls();
  auto edges = static_cast<::yaul::Edges>(wmWindow.frameRect);

  // Remove resizing
  window->setResizable(false);

  constexpr int bottom = 16;
  window->setDraggingArea(bottom, 0);

  ::yaul::Rectangle rectA{};
  rectA.width  = ::yaul::randRange(0, size.width);
  rectA.height = ::yaul::randRange(0, bottom);
  rectA.x      = ::yaul::randRange(0, size.width - rectA.width);
  rectA.y      = ::yaul::randRange(0, bottom - rectA.height);

  initialConfig += "  rectA.x:      " + std::to_string(rectA.x) + "\n";
  initialConfig += "  rectA.y:      " + std::to_string(rectA.y) + "\n";
  initialConfig += "  rectA.width:  " + std::to_string(rectA.width) + "\n";
  initialConfig += "  rectA.height: " + std::to_string(rectA.height) + "\n";

  // Validate rectA is wholely inside the dragging area
  auto rectAEdges = static_cast<::yaul::Edges>(rectA);
  ASSERT_LE(edges.top, wmWindow.frameRect.y + rectAEdges.top);
  ASSERT_GE(edges.right, wmWindow.frameRect.x + rectAEdges.right);
  ASSERT_GE(edges.top + bottom, wmWindow.frameRect.y + rectAEdges.bottom);
  ASSERT_LE(edges.left, wmWindow.frameRect.x + rectAEdges.left);

  ::yaul::Rectangle rectB{};
  rectB.width  = ::yaul::randRange(0, size.width);
  rectB.height = ::yaul::randRange(0, bottom);
  rectB.x      = ::yaul::randRange(0, size.width - rectB.width);
  rectB.y      = ::yaul::randRange(0, bottom - rectB.height);

  initialConfig += "  rectB.x:      " + std::to_string(rectB.x) + "\n";
  initialConfig += "  rectB.y:      " + std::to_string(rectB.y) + "\n";
  initialConfig += "  rectB.width:  " + std::to_string(rectB.width) + "\n";
  initialConfig += "  rectB.height: " + std::to_string(rectB.height) + "\n";

  // Validate rectB is wholely inside the dragging area
  auto rectBEdges = static_cast<::yaul::Edges>(rectB);
  ASSERT_LE(edges.top, wmWindow.frameRect.y + rectBEdges.top);
  ASSERT_GE(edges.right, wmWindow.frameRect.x + rectBEdges.right);
  ASSERT_GE(edges.top + bottom, wmWindow.frameRect.y + rectBEdges.bottom);
  ASSERT_LE(edges.left, wmWindow.frameRect.x + rectBEdges.left);

  wm.processAsyncCalls();

  ::yaul::Position cursor;
  int hitCount;

  // No mask area methodology test
  hitCount = 0;
  for (cursor.x = edges.left - 1; cursor.x <= edges.right; ++cursor.x) {
    for (cursor.y = edges.top - 1; cursor.y <= edges.top + bottom; ++cursor.y) {
      if (wm.sendHitTest(hWnd, cursor) == HTCAPTION)
        hitCount += 1;
    }
  }
  ASSERT_EQ(hitCount, size.width * bottom);

  window->addDraggingAreaMask(rectA);
  // Point test
  cursor.x = edges.left;
  cursor.y = edges.top;
  ASSERT_EQ(wm.sendHitTest(hWnd, cursor),
            (rectA.x == 0 && rectA.y == 0) ? HTCLIENT : HTCAPTION);
  cursor.x = edges.left + ((rectA.width != 0) ? rectA.x : 0);
  cursor.y = edges.top + ((rectA.height != 0) ? rectA.y : 0);
  ASSERT_EQ(wm.sendHitTest(hWnd, cursor),
            (rectA.width != 0 && rectA.height != 0) ? HTCLIENT : HTCAPTION);

  hitCount = 0;
  for (cursor.x = edges.left - 1; cursor.x <= edges.right; ++cursor.x) {
    for (cursor.y = edges.top - 1; cursor.y <= edges.top + bottom; ++cursor.y) {
      if (wm.sendHitTest(hWnd, cursor) == HTCAPTION)
        hitCount += 1;
    }
  }
  ASSERT_EQ(hitCount, size.width * bottom - rectA.area());

  window->addDraggingAreaMask(rectB);
  hitCount = 0;
  for (cursor.x = edges.left - 1; cursor.x <= edges.right; ++cursor.x) {
    for (cursor.y = edges.top - 1; cursor.y <= edges.top + bottom; ++cursor.y) {
      if (wm.sendHitTest(hWnd, cursor) == HTCAPTION)
        hitCount += 1;
    }
  }
  // Total area - aArea - bArea + a&bArea (double counted)
  ASSERT_EQ(hitCount, size.width * bottom - rectA.area() - rectB.area() +
                          (rectA & rectB).area());

  window->clearDraggingAreaMasks();
  hitCount = 0;
  for (cursor.x = edges.left - 1; cursor.x <= edges.right; ++cursor.x) {
    for (cursor.y = edges.top - 1; cursor.y <= edges.top + bottom; ++cursor.y) {
      if (wm.sendHitTest(hWnd, cursor) == HTCAPTION)
        hitCount += 1;
    }
  }
  ASSERT_EQ(hitCount, size.width * bottom);
}
