#include <string>
#include "colorpicker.hpp"
#include "imgui.h"

void clrpk(const char* id, float* col) {
    ImGui::SameLine(ImGui::GetWindowWidth() - 34 - ImGui::GetStyle().WindowPadding.x);
    if (ImGui::ColorButton((std::string("##btn") + id).c_str(), ImVec4(col[0], col[1], col[2], 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoAlpha, ImVec2(24, 12))) ImGui::OpenPopup((std::string("##pop") + id).c_str());
    if (ImGui::BeginPopup((std::string("##pop") + id).c_str())) {
        ImGui::ColorPicker3(id, col, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }
}