// Use WinMain for windows application entry
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

/**
 * @brief Main entry point for program
 * User implemented
 *
 * @param argc count of arguments
 * @param argv array of arguments
 * @return int zero on success, non-zero on failure
 */
extern "C" int main(int argc, char* argv[]);

/**
 * @brief Windows main entry point
 * Used for GUI applications versus console applications
 * Translates command line arguments to standard argc/arv with UTF-8 encoding
 *
 * @return int zero on success, non-zero on failure
 */
extern "C" int APIENTRY WinMain(HINSTANCE /* hInstance */,
                                HINSTANCE /* hPrevInstance */,
                                LPSTR /* lpCmdLine */,
                                int /* nShowCmd */) {
  // Get command line arguments as classic argc, argv
  int argc        = 0;
  wchar_t** argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
  if (argvW == nullptr) {
    return -1;
  }

  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  char** argv = new char*[static_cast<size_t>(argc + 1)];
  for (int i = 0; i != argc; ++i) {
    int len = WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, nullptr, 0, nullptr,
                                  nullptr);
    // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
    argv[i] = new char[static_cast<size_t>(len)];
    WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, argv[i], len, nullptr,
                        nullptr);
  }
  argv[argc] = nullptr;
  LocalFree(argvW);

  // Call user implemented main
  const int exitCode = main(argc, argv);

  // Clean up argv memory
  for (int i = 0; i < argc; ++i) {
    delete[] argv[i];  // NOLINT (cppcoreguidelines-owning-memory)
  }
  delete[] argv;  // NOLINT (cppcoreguidelines-owning-memory)

  return exitCode;
}

#endif
