#include "yaul/yaul.h"

#include <cstdio>
#include <exception>

/**
 * @brief Main entry point for program
 *
 * @param argc count of arguments
 * @param argv array of arguments
 * @return int zero on success, non-zero on failure
 */
int main(int argc, char* argv[]) {
  printf("Yaul adds 10 + 3 = %d", yaulAdd(10, 3));
  return 0;
}