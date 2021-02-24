/*
 * Created by Qt Creator 4.11.0 @ 23 Feb 2021 20:23:10
 *
 * Project: ChallengeGame
 * File: utilities.h
 *
 * Author: Sergey Lola
 */

#include <memory>
#include <string>
#include <stdexcept>

namespace utilities
{

// ---------------------------------------------------------------------------
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
  // дополнительный +1 для символа терминирующего нуля
  std::size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1u;
  if(size <= 0u)
  {
    throw std::runtime_error("Error during formatting.");
  }
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args...);
  // символ '\0' отбрасывается
  return std::string(buf.get(), buf.get() + size - 1u);
}

} // namespace utilities
