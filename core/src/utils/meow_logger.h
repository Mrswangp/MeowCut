#pragma once

#include "spdlog/sinks/null_sink.h"
#include "spdlog/spdlog.h"

class MeowLogger {
 private:
  MeowLogger() {
    std::string tag = "MeowLogger";
    logger_ = spdlog::null_logger_mt(tag);
    //        logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%@] [%!] %v"); // %@ %!没有起作用
    logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");
  }

  auto getInternalLogger() { return logger_; }

 private:
  std::shared_ptr<spdlog::logger> logger_;

 public:
  static std::shared_ptr<spdlog::logger> getLogger() {
    static MeowLogger t;
    return t.getInternalLogger();
  }
};

static std::shared_ptr<spdlog::logger> Logger() { return MeowLogger::getLogger(); }
