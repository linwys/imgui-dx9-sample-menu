#include "visuals.hpp"
#include "ctx.hpp"
#include "imgui.h"
#include "checkbox.hpp"
#include "colorpicker.hpp"
#include "combo.hpp"

void drawvisuals() {
    if (!ctx.sub[0]) {
        ImGui::Columns(2, nullptr, false);
        ImGui::BeginChild("esp_m", ImVec2(0, 300), true);
        ImGui::TextDisabled("ESP"); ImGui::Dummy(ImVec2(0, 5));
        cbx("Enable", &ctx.espen); 
        cbx("Box", &ctx.box); clrpk("box_c", ctx.boxclr);
        cbx("Name", &ctx.name); clrpk("name_c", ctx.nmclr);
        cbx("Health", &ctx.hp); 
        ImGui::EndChild(); ImGui::NextColumn();
        ImGui::BeginChild("esp_mdl", ImVec2(0, 150), true);
        ImGui::TextDisabled("Models"); ImGui::Dummy(ImVec2(0, 5));
        cbx("Glow", &ctx.glwen); clrpk("glow_c", ctx.glwclr);
        static const char* c_arr[] = { "Flat", "Textured", "Wireframe" }; 
        cmb("Chams type", &ctx.hitbox, c_arr, 3);
        ImGui::EndChild();

        ImGui::BeginChild("esp_xtr", ImVec2(0, 0), true);
        ImGui::TextDisabled("Extra"); ImGui::Dummy(ImVec2(0, 5)); ImGui::TextDisabled("Coming soon...");
        ImGui::EndChild(); ImGui::Columns(1);
    } else {
        ImGui::BeginChild("w_esp", ImVec2(0, 0), true);
        ImGui::TextDisabled("WORLD"); ImGui::Dummy(ImVec2(0, 5)); ImGui::TextDisabled("Coming soon...");
        ImGui::EndChild();
    }
}