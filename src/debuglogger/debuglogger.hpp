#pragma once

#include <mutex>
#include <string>

#define DEBUG_LINE_LENGTH 1024

namespace Koala {
class DebugLogger {
public:
  typedef enum {
    DEBUG_ERROR,
    DEBUG_WARNING,
    DEBUG_STATUS,
    DEBUG_INFO,
    DEBUG_VERBOSE,
    DEBUG_NUM_LEVELS,
  } DebugLevel;

  typedef enum {
    COLOR_RESET,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_CYAN,
    COLOR_MAGENTA,
    COLOR_YELLOW,
    COLOR_WHITE,
    DEBUG_NUM_COLORS,
  } DebugColor;

  DebugLogger(std::string debugTag, DebugColor color, bool bold);

  template <typename... Args> void Error(const char *format, Args... args);
  template <typename... Args> void Warning(const char *format, Args... args);
  template <typename... Args> void Status(const char *format, Args... args);
  template <typename... Args> void Info(const char *format, Args... args);
  template <typename... Args> void Verbose(const char *format, Args... args);

  void WriteLog(DebugLevel level, const char *format, ...) const noexcept;
  void SetDebugModuleName(std::string newName) noexcept;
  void SetDebugColor(DebugColor newColor) noexcept;
  void SetDebugBold(bool bold) noexcept;

private:
  static const char *debugLevels[DebugLevel::DEBUG_NUM_LEVELS];
  static const char *debugColors[DebugColor::DEBUG_NUM_COLORS];
  static std::mutex loggerLock;

  std::string debugTag;
  DebugColor debugColor;
  bool debugBold;
};
} // namespace Koala
