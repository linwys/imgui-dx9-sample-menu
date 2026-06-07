#include "combo.hpp"
#include "imgui.h"
#include <string>

bool cmb(std::string_view label, int* curr, const char** items, int size) {
    ImGui::TextDisabled("%.*s", static_cast<int>(label.size()), label.data());
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 10);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));
    bool res = ImGui::Combo((std::string("##cb") + label.data()).c_str(), curr, items, size);
    ImGui::PopStyleVar();
    return res;
}