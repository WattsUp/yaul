#include <yaul/application.hpp>

#include "common/string.hpp"
#include "window_mocked.hpp"

#include <gtest/gtest.h>
#include <future>
#include <vector>

using Level  = ::yaul::LogLevel;
using string = ::yaul::string;

class Application : public ::testing::Test {
 protected:
  string initialConfig;

  std::vector<string> commandLineArgs;

  ::yaul::Application* application = nullptr;

  std::vector<string> logOutput;

  void logFunction(Level /* level */, const char* msg) noexcept {
    logOutput.emplace_back(msg);
  }

  static void staticLogFunction(Level level, const char* msg) noexcept {
    if (object != nullptr)
      object->logFunction(level, msg);
  }

  static Application* object;

  virtual void SetUp() {
    object   = this;
    auto& wm = WindowManager::instance(true);

    int argc = ::yaul::randRange(0, 8);

    // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
    char** argv = new char*[static_cast<size_t>(argc + 1)];
    for (int i = 0; i != argc; ++i) {
      string argument = "--arg" + std::to_string(i) + "=";
      // Add emoticons
      for (int ii = 0; ii < 8; ++ii) {
        argument.push_back(
            static_cast<char32_t>(::yaul::randRange(0x1F601, 0x1F64F)));
      }
      commandLineArgs.emplace_back(argument);
      initialConfig += "  argv        = \"" + argument + "\";\n";

      size_t len = argument.size();
      // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
      argv[i] = new char[len + 1];
      auto a  = argument.c_str();
      strncpy_s(argv[i], len + 1, a, len);
    }
    argv[argc] = nullptr;

    ::yaul::ApplicationSettings settings;
    settings.customRenderLoop = false;
    settings.logger           = staticLogFunction;

    ASSERT_NO_THROW(application =
                        new ::yaul::Application(argc, argv, settings));

    // Clean up argv memory
    for (int i = 0; i < argc; ++i)
      delete[] argv[i];  // NOLINT (cppcoreguidelines-owning-memory)
    delete[] argv;       // NOLINT (cppcoreguidelines-owning-memory)

    wm.processAsyncCalls();
  }

  virtual void TearDown() {
    auto& wm = WindowManager::instance();
    EXPECT_TRUE(wm.windowClass.messages.empty());

    delete application;
    application = nullptr;

    EXPECT_FALSE(wm.anyErrors);
    EXPECT_TRUE(wm.asyncQueue.empty());

    ASSERT_EQ(wm.windows.size(), size_t{0});

    // Okay to have leftover messages after deletion
    wm.windowClass.messages.clear();

    if (::testing::Test::HasFailure()) {
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_RED,
                                         "Failing Test Configuration\n");
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_DEFAULT, "%s",
                                         initialConfig.c_str());
    }
  }
};

Application* Application::object = nullptr;

TEST_F(Application, Instantiation) {
  EXPECT_EQ(commandLineArgs, logOutput);

  auto& wm = WindowManager::instance();
  ASSERT_EQ(wm.windows.size(), size_t{0});
}

TEST_F(Application, AddWindow) {
  ::yaul::Size size;
  size.width  = ::yaul::randRange(SystemMetricsMocked::cXMin, 2048);
  size.height = ::yaul::randRange(SystemMetricsMocked::cYMin, 2048);

  string id = "Unique window ID: ";
  // Add emoticons
  for (int i = 0; i < 8; ++i) {
    id.push_back(static_cast<char32_t>(::yaul::randRange(0x1F601, 0x1F64F)));
  }

  auto showState = ::yaul::Window::ShowState::restore;

  initialConfig += "  size.width  = " + std::to_string(size.width) + ";\n";
  initialConfig += "  size.height = " + std::to_string(size.height) + ";\n";
  initialConfig += "  id          = \"" + id + "\";\n";

  auto window = application->addWindow(id.c_str(), size, showState);

  auto& wm = WindowManager::instance();
  wm.processAsyncCalls();
  wm.waitForMessageQueue();

  ASSERT_EQ(wm.windows.size(), size_t{1});

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  auto& wmWindow = wm.windows.begin()->second;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  EXPECT_EQ(wmWindow.title, id);
  EXPECT_EQ(wmWindow.frameRect, wmWindow.rect);
  EXPECT_EQ(wmWindow.rect, size);
  EXPECT_EQ(wmWindow.showState, showState);
}

TEST_F(Application, WaitForAllWindowsToClose) {
  application->addWindow("");

  auto& wm = WindowManager::instance();
  wm.processAsyncCalls();
  wm.waitForMessageQueue();

  ASSERT_EQ(wm.windows.size(), size_t{1});

  std::atomic<bool> closeUnblocked = false;
  std::atomic<bool> closeSent      = false;

  // Send close button after sendCloseDelay
  auto closeThreadFuture =
      std::async(std::launch::async, [&closeUnblocked, &closeSent] {
        std::this_thread::sleep_for(std::chrono::milliseconds(4));

        ASSERT_FALSE(closeUnblocked);
        auto& wm = WindowManager::instance();

        wm.sendEventToAll(WindowManager::Event::close);
        closeSent = true;
        wm.processAsyncCalls();
      });

  // Function blocks until window close is clicked
  application->waitForAllWindowsToClose();

  ASSERT_EQ(closeThreadFuture.wait_for(std::chrono::milliseconds(64)),
            std::future_status::ready);

  closeUnblocked = true;
  ASSERT_TRUE(closeSent);
  ASSERT_EQ(wm.windows.size(), size_t{0});

  wm.waitForMessageQueue();
}