#ifndef CCC_EXCEPTION_HPP
#define CCC_EXCEPTION_HPP

#include <exception>
#include <fmt/format.h>
#include <string>
#include <string_view>

namespace ccc {

class Exception : public std::exception {
public:
  explicit Exception(std::string_view fmt, auto... args) : m_buffer(make_buffer(fmt, args...)) {}

  auto what() const noexcept -> const char * override {
    return &m_buffer[0];
  }

  virtual auto name() const noexcept -> std::string_view {
    return "Exception";
  }

protected:
  virtual auto fmt() const noexcept -> std::string_view {
    return "";
  };

  auto make_buffer(std::string_view fmt, auto... args) const {
    return fmt::format(fmt::runtime(fmt), args...);
  }

private:
  std::string m_buffer;
};

}  // namespace ccc

#endif
