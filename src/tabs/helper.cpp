#include "helper.hpp"
#include "ctx.hpp"
#include "imgui.h"
#include "checkbox.hpp"

void drawhelper() {
    ImGui::BeginChild("hlp_m", ImVec2(0, 0), true);
    ImGui::TextDisabled("UTILITY"); ImGui::Dummy(ImVec2(0, 5));
    cbx("Grenade prediction", &ctx.nade); cbx("Bomb timer", &ctx.bomb);
    ImGui::EndChild();
}