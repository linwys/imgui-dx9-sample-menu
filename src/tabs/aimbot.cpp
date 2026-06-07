#include "aimbot.hpp"
#include "ctx.hpp"
#include "imgui.h"
#include "checkbox.hpp"
#include "tab.hpp"
#include "combo.hpp"

void drawaimbot() {
    ImGui::Columns(2, nullptr, false);
    ImGui::BeginChild("aim_m", ImVec2(0, 0), true);
    ImGui::TextDisabled("AIMBOT"); ImGui::Dummy(ImVec2(0, 5));
    cbx("Enable", &ctx.aim); mkbind("aim_b", ctx.aimkey);
    cbx("Visualize FOV", &ctx.drawfov); ImGui::Dummy(ImVec2(0, 5));
    ImGui::SetNextItemWidth(140.0f); ImGui::SliderFloat("##fov_sz", &ctx.fov, 0.f, 180.f, "%.0f");
    ImGui::SameLine(); ImGui::Text("FOV Size");
    ImGui::SetNextItemWidth(140.0f); ImGui::SliderFloat("##smooth_v", &ctx.smooth, 0.f, 10.f, "%.1f");
    ImGui::SameLine(); ImGui::Text("Smooth");
    
    ImGui::Dummy(ImVec2(0, 5));
    static const char* h_arr[] = { "Head", "Neck", "Chest", "Pelvis" }; 
    cmb("Hitbox", &ctx.hitbox, h_arr, 4);
    ImGui::EndChild(); ImGui::NextColumn();
    
    ImGui::BeginChild("trig_m", ImVec2(0, 0), true);
    ImGui::TextDisabled("TRIGGERBOT"); ImGui::Dummy(ImVec2(0, 5));
    
    cbx("Enable##trig", &ctx.trig); mkbind("trig_b", ctx.trigkey); ImGui::Dummy(ImVec2(0, 5));
    ImGui::SetNextItemWidth(140.0f); ImGui::SliderInt("##trig_d", &ctx.trigdel, 0, 500);
    ImGui::SameLine(); ImGui::Text("Delay (ms)");
    ImGui::EndChild(); ImGui::Columns(1);
}