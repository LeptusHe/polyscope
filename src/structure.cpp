#include "polyscope/structure.h"

#include "polyscope/polyscope.h"

#include "imgui.h"

namespace polyscope {

Structure::Structure(std::string name_) : name(name_) {}

Structure::~Structure(){};

void Structure::setEnabled(bool newEnabled) {
  if (newEnabled == enabled) return;
  newEnabled = enabled;
};

bool Structure::isEnabled() { return enabled; };

void Structure::drawUI() {

  ImGui::PushID(name.c_str()); // ensure there are no conflicts with
                               // identically-named labels


  if (ImGui::TreeNode(name.c_str())) {

    bool currEnabled = enabled;
    ImGui::Checkbox("Enabled", &currEnabled);
    setEnabled(currEnabled);
    ImGui::SameLine();

    // Options popup
    if (ImGui::Button("Options")) {
      ImGui::OpenPopup("OptionsPopup");
    }
    if (ImGui::BeginPopup("OptionsPopup")) {

      // Transform
      if (ImGui::BeginMenu("Transform")) {
        if (ImGui::MenuItem("Center")) centerBoundingBox();
        if (ImGui::MenuItem("Reset")) resetTransform();
        ImGui::EndMenu();
      }

      // Do any structure-specific stuff here
      this->drawCustomOptionsUI();

      ImGui::EndPopup();
    }

    // Do any structure-specific stuff here
    this->drawCustomUI();

    // Draw quantities list, in the common case of a QuantityStructure
    this->drawQuantitiesUI();

    ImGui::TreePop();
  }
  ImGui::PopID();
}

void Structure::drawQuantitiesUI() {}

void Structure::drawSharedStructureUI() {}

void Structure::resetTransform() {
  objectTransform = glm::mat4(1.0);
  updateStructureExtents();
}

void Structure::centerBoundingBox() {
  std::tuple<glm::vec3, glm::vec3> bbox = boundingBox();
  glm::vec3 center = (std::get<1>(bbox) + std::get<0>(bbox)) / 2.0f;
  glm::mat4x4 newTrans = glm::translate(glm::mat4x4(1.0), -glm::vec3(center.x, center.y, center.z));
  objectTransform = objectTransform * newTrans;
  updateStructureExtents();
}

glm::mat4 Structure::getModelView() { return view::getCameraViewMatrix() * objectTransform; }

} // namespace polyscope
