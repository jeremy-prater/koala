#include "debuglogger.hpp"
#include <iostream>
#include <stdarg.h>
#include <stdio.h>

using namespace Koala;

const char
    *DebugLogger::debugLevels[DebugLogger::DebugLevel::DEBUG_NUM_LEVELS] = {
        "     ERROR", "   WARNING", "  STATUS  ", "  INFO    ", "VERBOSE   ",
};

const char
    *DebugLogger::debugColors[DebugLogger::DebugColor::DEBUG_NUM_COLORS] = {
        "\x1B[0m",     "\x1B[%d;31m", "\x1B[%d;32m", "\x1B[%d;34m",
        "\x1B[%d;36m", "\x1B[%d;35m", "\x1B[%d;33m", "\x1B[%d;37m"};

std::mutex DebugLogger::loggerLock;

DebugLogger::DebugLogger(std::string debugTag, DebugColor color, bool bold)
    : debugTag(debugTag), debugColor(color), debugBold(bold) {}

void DebugLogger::SetDebugModuleName(std::string newName) noexcept {
  debugTag = newName;
}

void DebugLogger::SetDebugColor(DebugColor newColor) noexcept {
  debugColor = newColor;
}

void DebugLogger::SetDebugBold(bool bold) noexcept { debugBold = bold; }

void DebugLogger::Error(const char *format, ...) const noexcept {
  va_list args = {};
  va_start(args, format);
  WriteLogInternal(DebugLogger::DebugLevel::DEBUG_ERROR, format, args);
  va_end(args);
}

void DebugLogger::Warning(const char *format, ...) const noexcept {
  va_list args = {};
  va_start(args, format);
  WriteLogInternal(DebugLogger::DebugLevel::DEBUG_WARNING, format, args);
  va_end(args);
}

void DebugLogger::Status(const char *format, ...) const noexcept {
  va_list args = {};
  va_start(args, format);
  WriteLogInternal(DebugLogger::DebugLevel::DEBUG_STATUS, format, args);
  va_end(args);
}

void DebugLogger::Info(const char *format, ...) const noexcept {
  va_list args = {};
  va_start(args, format);
  WriteLogInternal(DebugLogger::DebugLevel::DEBUG_INFO, format, args);
  va_end(args);
}

void DebugLogger::Verbose(const char *format, ...) const noexcept {
  va_list args = {};
  va_start(args, format);
  WriteLogInternal(DebugLogger::DebugLevel::DEBUG_VERBOSE, format, args);
  va_end(args);
}

void DebugLogger::WriteLog(DebugLogger::DebugLevel level, const char *format,
                           ...) const noexcept {
  va_list args = {};
  va_start(args, format);
  WriteLogInternal(level, format, args);
  va_end(args);
}

void DebugLogger::WriteLogInternal(DebugLogger::DebugLevel level,
                                   const char *format, va_list args) const
    noexcept {
  static char buffer[DEBUG_LINE_LENGTH];
  static char levelColor[8];
  static char tagColor[8];

  vsnprintf(buffer, DEBUG_LINE_LENGTH, format, args);

  int levelColorIndex;
  bool levelBold = false;

  switch (level) {
  case DebugLevel::DEBUG_ERROR:
    levelColorIndex = DebugColor::COLOR_RED;
    levelBold = true;
    break;
  case DebugLevel::DEBUG_WARNING:
    levelColorIndex = DebugColor::COLOR_YELLOW;
    levelBold = true;
    break;
  case DebugLevel::DEBUG_STATUS:
    levelColorIndex = DebugColor::COLOR_GREEN;
    break;
  case DebugLevel::DEBUG_INFO:
    levelColorIndex = DebugColor::COLOR_WHITE;
    break;
  case DebugLevel::DEBUG_VERBOSE:
    levelColorIndex = DebugColor::COLOR_CYAN;
    break;

  default:
    levelColorIndex = DebugColor::COLOR_MAGENTA;
    break;
  };

  snprintf(levelColor, 8, debugColors[levelColorIndex], levelBold);
  snprintf(tagColor, 8, debugColors[debugColor], debugBold);

  {
    std::lock_guard<std::mutex> guard(loggerLock);
    std::cout << "[" << levelColor << debugLevels[level]
              << debugColors[DebugColor::COLOR_RESET] << "] [" << tagColor
              << debugTag << debugColors[DebugColor::COLOR_RESET] << "] "
              << buffer << std::endl;
  }
}