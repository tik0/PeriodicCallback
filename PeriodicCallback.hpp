#include <iostream>
#include <unistd.h>
#include <chrono>
#include <future>
#include <functional>

class PeriodicCallback
{
public:

  PeriodicCallback(
      std::size_t interval_us = 1000000,
      std::function<void(void)> func = NULL,
      std::string prefix = "TPI: ",
      std::string suffix = "\n")
  :_execute(false) ,
   _tic(0) ,
   _interval_us(interval_us) ,
   _func(func) ,
   _prefix(prefix) ,
   _suffix(suffix) ,
   _time(std::chrono::high_resolution_clock::now())
  {}

  ~PeriodicCallback() {
    if( _execute.load(std::memory_order_acquire) ) {
      stop();
    };
  }

  void stop() {
    _execute.store(false, std::memory_order_release);
    if( _thd.joinable() )
      _thd.join();
  }

  void start() {
    if( _execute.load(std::memory_order_acquire) ) {
      stop();
    };
    _execute.store(true, std::memory_order_release);
    _thd = std::thread([this]() {
      while (_execute.load(std::memory_order_acquire)) {
        _time = std::chrono::high_resolution_clock::now();
        if (_func) {
          _func();
        } else {
          print();
        }
        resetTics();
        const std::size_t sleep_us = _interval_us - getCurrentIntervallDuration<std::chrono::microseconds>();
        if (sleep_us > 0) {
          std::this_thread::sleep_for(std::chrono::microseconds(sleep_us));
        } else {
          std::cerr << "Warning: Interval to short\n";
        }
      }
    });
  }

  bool isRunning() const noexcept {
    return ( _execute.load(std::memory_order_acquire) && _thd.joinable() );
  }

  void setIntervalFunction(std::function<void(void)> func = NULL) {
    _func = func;
  }

  void tic() {
    ++_tic;
  }

  void untic() {
    --_tic;
  }

  void resetTics() {
    _tic = 0;
  }

  void callUserFunction() {
    _func();
  }

  std::size_t getTics() {
    return _tic;
  }

  std::size_t getIntervallDuration() {
    return _interval_us;
  }


  template<typename T>
  std::size_t getCurrentIntervallDuration() {
    const auto elapsed = std::chrono::high_resolution_clock::now() - _time;
    return std::chrono::duration_cast<T>(elapsed).count();
  }

private:

  void print() {
    std::cout << _prefix << getTics() << _suffix;
  }

  std::atomic<bool> _execute;
  std::size_t _tic;
  std::size_t _interval_us;
  std::function<void(void)> _func;
  std::string _prefix;
  std::string _suffix;
  std::chrono::high_resolution_clock::time_point _time;
  std::thread _thd;
};
