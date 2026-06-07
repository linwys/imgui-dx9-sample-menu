#include "misc.hpp"
#include "ctx.hpp"
#include "imgui.h"
#include "checkbox.hpp"

void drawmisc() {
    if (!ctx.sub[2]) {
        ImGui::BeginChild("mov_m", ImVec2(0, 0), true);
        ImGui::TextDisabled("MOVEMENT"); ImGui::Dummy(ImVec2(0, 5));
        cbx("Bunnyhop", &ctx.bhop); cbx("Auto strafer", &ctx.strafe);
        ImGui::EndChild();
    } else {
        ImGui::BeginChild("ui_m", ImVec2(0, 0), true);
        ImGui::TextDisabled("UI SETTINGS"); ImGui::Dummy(ImVec2(0, 5));
        cbx("Show watermark", &ctx.wm); cbx("Show keybinds", &ctx.showbinds);
        ImGui::TextDisabled("Hui znaet");
        ImGui::EndChild();
    }
}