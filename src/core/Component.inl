#pragma once

template <typename... T>
void Component::assignRequiredComponent(std::weak_ptr<T> &...components) {
  if (auto ownerPtr = owner.lock()) {
    auto assign = [&](auto &component_ptr) {
      using ComponentType =
          typename std::decay_t<decltype(*component_ptr.lock())>;
      component_ptr = ownerPtr->template GetComponentShared<ComponentType>();
    };

    (assign(components), ...);

    auto validate = [](const auto &component_ptr) {
      if (component_ptr.expired()) {
        using ComponentType =
            typename std::decay_t<decltype(*component_ptr.lock())>;
        throw std::runtime_error(
            std::string("Failed to get required component: ") +
            typeid(ComponentType).name());
      }
    };

    (validate(components), ...);

  } else {
    throw std::runtime_error(
        "Component owner is null during required component assignment.");
  }
}
