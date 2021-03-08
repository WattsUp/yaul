#include <gtest/gtest.h>

#include "version.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif /* WIN32 */

namespace testing {
class MemoryLeakDetector : public EmptyTestEventListener {
 public:
  virtual void OnTestStart(const TestInfo&) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    ::_CrtMemCheckpoint(&memState);
#endif /* WIN32 */
  }

  virtual void OnTestEnd(const TestInfo& testInfo) {
    if (!testInfo.result()->Passed())
      return;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    ::_CrtMemState postTest, memStateDiff;
    ::_CrtMemCheckpoint(&postTest);

    auto result = ::_CrtMemDifference(&memStateDiff, &memState, &postTest);
    if (result != 0) {
      FAIL() << "Memory leak of " << memStateDiff.lSizes[_NORMAL_BLOCK]
             << " bytes detected.";
    }
#elif defined(__linux) || defined(__linux__)
      // TODO (WattsUp) do memory leak testing for linux
#endif /* WIN32, __linux__ */
  }

 private:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  ::_CrtMemState memState;
#endif /* WIN32 */
};
}  // namespace testing

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN,
                                     "[  SemVer  ] ");
  ::testing::internal::ColoredPrintf(testing::internal::COLOR_DEFAULT, "%s\n",
                                     VERSION_STRING_FULL);

  ::testing::UnitTest::GetInstance()->listeners().Append(
      new ::testing::MemoryLeakDetector());
  return RUN_ALL_TESTS();
}