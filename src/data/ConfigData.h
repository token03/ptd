#include <glaze/glaze.hpp>

struct ConfigData {
  int screenWidth;
  int screenHeight;
};

template <>
struct glz::meta<ConfigData> {
  using T = ConfigData;
  static constexpr auto value =
      object("screenWidth", &T::screenWidth, "screenHeight", &T::screenHeight);
};
