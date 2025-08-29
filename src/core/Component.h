#pragma once
#include <memory>

class GameObject; // Forward-declare

// The abstract base class for all components.
// Inherits from std::enable_shared_from_this to allow components to safely
// create shared_ptrs of themselves for other components to observe.
class Component : public std::enable_shared_from_this<Component> {
public:
  virtual ~Component() = default;

  // Called when the component is added to a GameObject
  virtual void Init() {}

  // Called every frame
  virtual void Update(float deltaTime) {}

  // Called every frame during the drawing phase
  virtual void Draw() {}

  // A non-owning (weak) pointer back to the GameObject that owns this
  // component. This prevents circular references and memory leaks.
  std::weak_ptr<GameObject> owner;
};
