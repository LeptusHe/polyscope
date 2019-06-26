#pragma once

#include <string>

namespace polyscope {

// A 'quantity' (in Polyscope terminology) is data which is associated with a structure; any structure might have many
// quantities. For instance a mesh structure might have a scalar quantity associated with it, or a point cloud might
// have a vector field quantity associated with it.

template <typename S>
class Quantity {
public:
  Quantity(std::string name, S& parentStructure, bool dominates = false);
  virtual ~Quantity();

  // Draw the quantity.
  virtual void draw() = 0;

  // Draw the ImGUI ui elements
  virtual void drawUI(); // draws the tree node and enabled checkbox common to almost all quantities, and calls
                         // drawCustomUI() below. Can still be overidden in case something else is wanted.
  virtual void drawCustomUI() = 0; // overridden by children to add custom data to UI

  // Enable and disable the quantity
  bool isEnabled();
  virtual void setEnabled(bool newEnabled);

  // = Utility

  // A decorated name for the quantity that will be used in headers. For instance, for surface scalar named "value" we
  // return "value (scalar)"
  virtual std::string niceName();


  // === Member variables ===
  S& parent;              // the parent structure with which this quantity is associated
  const std::string name; // a name for this quantity, which must be unique amongst quantities on `parent`

protected:
  // Is this quantity currently being displayed?
  bool enabled = false; // should be set by setEnabled()
  const bool dominates = false;
};


} // namespace polyscope


#include "polyscope/quantity.ipp"
