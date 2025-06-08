
// C++ implementation of a pod_compare helper function to reduce C-style struct comparison logic
// --------------------------------------------------------------------------------------------------

#include <cstddef>
#include <iostream>
#include <tuple>
#include <array>
#include <cstdint>

struct Bar {
  uint16_t a;
  uint32_t b;
  char c[8];
  uint16_t d[4];

  // compiler generates operator==
  bool operator==(const Bar&) const = default;
};

// Helper functions implementation

// Overload for C-style arrays members to convert to std::array
template <typename T, std::size_t N>
constexpr auto tie_helper(const T (&member)[N]) {
  return std::to_array(member);
}

// Overload for other members
template <typename T>
constexpr auto tie_helper(const T& member) {
  return std::tie(member);
}

template <typename T, typename... Members>
constexpr auto tie_members(const T& obj, Members T::*... members) {
  return std::tuple{ tie_helper(obj.*members)... };
}

template <typename T, typename... Members>
constexpr bool pod_compare(const T& lhs, const T& rhs, Members T::*... members) {
  static_assert(std::is_standard_layout_v<T> && std::is_trivial_v<T>); // pod type check
  return tie_members(lhs, members...) == tie_members(rhs, members...);
}

// Usage example

typedef struct {
  int16_t a;
  uint32_t b;
  char c[8];
  uint16_t d[4];
} foo_t;

// Define operator== and use pod_compare helper function with struct members
bool operator==(const foo_t& lhs, const foo_t& rhs) {
  return pod_compare(lhs, rhs, &foo_t::a, &foo_t::b, &foo_t::c, &foo_t::d);
}

// Clasic approach: manually compare all fields

// bool operator==(const foo_t & lhs, const foo_t & rhs) {
//   return lhs.a == rhs.a and 
//          lhs.b == rhs.b and 
//          // std::array has operator== defined
//          std::to_array(lhs.c) == std::to_array(rhs.c) and
//          std::to_array(lhs.d) == std::to_array(rhs.d);
// }


int main() {
  if (foo_t(1, 1, {1, 1}) == foo_t(1, 1, {1, 1})) {
    std::cout << "same\n";
  }
  if (Bar{1, 1, {1, 1}} == Bar{1, 1, {1, 1}}) {
    std::cout << "same\n";
  }
}
