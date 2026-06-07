#include "render.hpp"
#include "ctx.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "tab.hpp"
#include "visuals.hpp"
#include "aimbot.hpp"
#include "misc.hpp"
#include "helper.hpp"
#include "profile.hpp"

void applystyle(float mod) {
    auto& s = ImGui::GetStyle();
    s.WindowRounding = 4.0f; s.ChildRounding = 2.0f; s.FrameRounding = 2.0f; s.PopupRounding = 2.0f; s.GrabRounding = 2.0f; s.ScrollbarSize = 4.0f; s.ScrollbarRounding = 2.0f; s.Alpha = mod;
    s.WindowBorderSize = 1.0f; s.ChildBorderSize = 1.0f; s.FrameBorderSize = 0.0f; s.PopupBorderSize = 1.0f; s.ItemSpacing = ImVec2(8, 8);
    auto c = s.Colors;
    ImVec4 acc = ImVec4(0.42f, 0.65f, 0.95f, 1.0f); 
    c[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f); c[ImGuiCol_ChildBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f); c[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f * mod);
    c[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f); c[ImGuiCol_FrameBgHovered] = ImVec4(0.11f, 0.11f, 0.11f, 1.0f); c[ImGuiCol_FrameBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
    c[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.98f); c[ImGuiCol_CheckMark] = acc; c[ImGuiCol_SliderGrab] = acc; c[ImGuiCol_SliderGrabActive] = ImVec4(0.52f, 0.75f, 1.0f, 1.0f);
    c[ImGuiCol_Header] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f); c[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f); c[ImGuiCol_HeaderActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    c[ImGuiCol_Button] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f); c[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f); c[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
    c[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f); c[ImGuiCol_TextDisabled] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
}

void drawovr() {
    if (!ctx.showbinds) return;
    applystyle(1.0f); 
    ImGui::SetNextWindowSizeConstraints(ImVec2(180, -1), ImVec2(300, -1));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.04f, 0.04f, 0.04f, 0.9f));
    ImGui::Begin("b_hud", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::TextDisabled("keybinds"); ImGui::Dummy(ImVec2(0, 5));
    auto item = [](const char* n, const bind_t& b, bool e) {
        if (e && b.isact()) {
            ImGui::Text("%s", n); const char* st = (b.mode == 2) ? "[toggle]" : (b.mode == 0 ? "[always]" : "[hold]");
            ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize(st).x - 10); ImGui::TextColored(ImGui::GetStyle().Colors[ImGuiCol_TextDisabled], st);
        }
    };
    if (!(ctx.aim && ctx.aimkey.isact()) && !(ctx.trig && ctx.trigkey.isact())) ImGui::TextDisabled("empty"); 
    else { item("aimbot", ctx.aimkey, ctx.aim); item("triggerbot", ctx.trigkey, ctx.trig); }
    ImGui::End(); ImGui::PopStyleColor();
}

void drawmenu(float mod) {
    applystyle(mod);
    ImGui::SetNextWindowSize(ImVec2(680, 520), ImGuiCond_FirstUseEver);
    ImGui::Begin("m_wnd", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);
    
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetWindowPos(); ImVec2 sz = ImGui::GetWindowSize();
    dl->AddRectFilled(p, ImVec2(p.x + sz.x, p.y + sz.y), ImGui::GetColorU32(ImVec4(0.06f, 0.06f, 0.06f, 1.0f)));

    ImGui::PushFont(fttl); ImGui::SetCursorPos(ImVec2(20, 15)); ImGui::Text("sample ui"); ImGui::PopFont();
    
    ImGui::SameLine(130); ImGui::SetCursorPosY(15);
    if (mktab("Visuals", ctx.tab == 0, ImVec2(75, 25))) ctx.tab = 0; ImGui::SameLine();
    if (mktab("Aimbot", ctx.tab == 1, ImVec2(75, 25))) ctx.tab = 1; ImGui::SameLine();
    if (mktab("Misc", ctx.tab == 2, ImVec2(75, 25))) ctx.tab = 2; ImGui::SameLine();
    if (mktab("Helper", ctx.tab == 3, ImVec2(75, 25))) ctx.tab = 3; ImGui::SameLine();
    if (mktab("Profile", ctx.tab == 4, ImVec2(75, 25))) ctx.tab = 4;
    
    ImGui::Dummy(ImVec2(0, 15));
    dl->AddLine(ImVec2(p.x, ImGui::GetCursorPosY() + p.y), ImVec2(p.x + sz.x, ImGui::GetCursorPosY() + p.y), ImGui::GetColorU32(ImVec4(1,1,1,0.05f)));
    ImGui::Dummy(ImVec2(0, 5));

    ImGui::BeginChild("s_wnd", ImVec2(140, 0), true);
    if (ctx.tab == 0) {
        if (mksub("Player", ctx.sub[0] == 0)) ctx.sub[0] = 0;
        if (mksub("World", ctx.sub[0] == 1)) ctx.sub[0] = 1;
    } else if (ctx.tab == 1) {
        if (mksub("Pistols", ctx.sub[1] == 0)) ctx.sub[1] = 0;
        if (mksub("Rifles", ctx.sub[1] == 1)) ctx.sub[1] = 1;
    } else if (ctx.tab == 2) {
        if (mksub("Movement", ctx.sub[2] == 0)) ctx.sub[2] = 0;
        if (mksub("Settings", ctx.sub[2] == 1)) ctx.sub[2] = 1;
    } else if (ctx.tab == 3) {
        if (mksub("Utility", ctx.sub[3] == 0)) ctx.sub[3] = 0;
    } else if (ctx.tab == 4) {
        if (mksub("Manager", ctx.sub[4] == 0)) ctx.sub[4] = 0;
    }
    ImGui::EndChild(); ImGui::SameLine();

    ImGui::BeginChild("c_wnd", ImVec2(0, 0), false);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.04f, 0.04f, 0.04f, 1.0f)); 
    
    if (ctx.tab == 0) drawvisuals();
    else if (ctx.tab == 1) drawaimbot();
    else if (ctx.tab == 2) drawmisc();
    else if (ctx.tab == 3) drawhelper();
    else if (ctx.tab == 4) drawprofile();

    ImGui::PopStyleColor(); ImGui::EndChild(); ImGui::End();
}