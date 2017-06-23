#include "PeriodicCallback.hpp"

void print() {
  std::cout << "t2: Interval passed" << std::endl;
}

PeriodicCallback t3(3000000);  // 3 s interval in us

void printWithArg(std::size_t *idx) {
  std::cout << "t3: Interval with count(" << t3.getTics()
            << ") vs. index(" <<  *idx
            << ") passed after interval[ms]("
            << t3.getCurrentIntervallDuration<std::chrono::milliseconds>()  // This returns 0 if interval was passed
            << ")" << std::endl;
}

int main(int argc, char **argv)
{

  std::size_t idx = 0;

  PeriodicCallback t1(
      1000000,                   // 1 s interval in us
      NULL,                      // Use the standard print function
      "t1: Tics per interval: ", // Set prefix of the print function
      " tics\n");                // Set suffix of the print function


  PeriodicCallback t2(
      2000000,                   // 2 s interval in us
      std::bind(print));         // Use a user defined print function

  // Set function of t3
  t3.setIntervalFunction(std::bind(printWithArg, &idx));

  t1.start();
  t2.start();
  t3.start();


  const std::size_t idxMax = 10000;
  for (idx = 0; idx < idxMax; ++idx) {
    t1.tic();
    t2.tic();
    t3.tic();
    if ( idx == idxMax / 2) {
      printWithArg(&idx);
    }
    usleep(1000);
  }

  t1.stop();
  t2.stop();
  t3.stop();

  return 0;
}
