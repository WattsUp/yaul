#include <yaul/yaul.hpp>

// #define YAUL_EXPOSE_BACKEND
// #include <yaul/vg/gpu/gl.hpp>
// #include <yaul/vg/gpu/vulkan.hpp>

#include "logging.h"

using App         = ::yaul::Application;
using AppSettings = ::yaul::ApplicationSettings;
using Window      = ::yaul::Window;

const float fieldOfView = 60.0F;
const float clipNear    = 1.0F;
const float clipFar     = 30.0F;

const float cubeX           = 0.0F;
const float cubeY           = 0.0F;
const float cubeZ           = -7.0F;
const float cubeRotationInc = 0.9F;

const float arcX = 50.0F;
const float arcY = 50.0F;
const float arcR = 40.0F;

/**
 * @brief Main entry point for program
 *
 * @param argc count of arguments
 * @param argv array of arguments
 * @return int zero on success, non-zero on failure
 */
int main(int argc, char* argv[]) {
  initializeLogFile("3d-and-vg.log");
  AppSettings settings;
  settings.logger           = libyaulLog;
  settings.customRenderLoop = true;
  auto app                  = App(argc, argv, settings);

  Window* window = nullptr;
  try {
    window = app.addWindow(u8"🌍3D and vector graphics");
  } catch (const ::std::exception& e) {
    spdlog::error(e.what());
    return -1;
  }

  while (!window->shouldClose())
    window->render();
  app.waitForAllWindowsToClose();

  // float rotation = 0.0F;

  // while (window.isOpen()) {
  //   // Clear the buffer
  //   window.clear(color::white);

  //   // Draw 3d rotating cube
  //   if (window.backend() == gpu::opengl) {
  //   }

  //   // Restore default backend settings for 2D
  //   window.resetBackend();

  //   Paint paint(color::black);
  //   window.arc(arcX, arcY, arcR, 0, radians(rotation), paint);
  //   // or (equalivalent)
  //   // Arc arc(arcX, arcY, arcR, 0, 0, color::black); // create object above
  //   // arc.setSweepAngle(radians(rotation));
  //   // window.draw(arc);
  //   window.render();

  //   // Rotate the cube
  //   rotation += cubeRotationInc;
  // }

  return 0;
}