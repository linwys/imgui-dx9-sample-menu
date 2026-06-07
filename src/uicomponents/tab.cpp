#include <windows.h>
#include "tab.hpp"
#include "imgui_internal.h"
#include <string>

bool mktab(const char* txt, bool active, ImVec2 sz) {
    auto p = ImGui::GetCursorScreenPos(); ImRect bb(p, ImVec2(p.x + sz.x, p.y + sz.y));
    ImGui::ItemSize(sz, 0.0f); if (!ImGui::ItemAdd(bb, 0)) return false;
    bool h, c; bool pressed = ImGui::ButtonBehavior(bb, ImGui::GetID(txt), &h, &c);
    auto draw = ImGui::GetWindowDrawList(); float m = ImGui::GetStyle().Alpha;

    draw->AddText(ImVec2(bb.Min.x + (sz.x - ImGui::CalcTextSize(txt).x) * 0.5f, bb.Min.y + (sz.y - ImGui::CalcTextSize(txt).y) * 0.5f), active ? ImGui::GetColorU32(ImVec4(1,1,1,m)) : h ? ImGui::GetColorU32(ImVec4(0.75f,0.75f,0.75f,m)) : ImGui::GetColorU32(ImVec4(0.4f,0.4f,0.4f,m)), txt);
    if (active) draw->AddLine(ImVec2(bb.Min.x, bb.Max.y - 1.5f), ImVec2(bb.Max.x, bb.Max.y - 1.5f), ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_CheckMark]), 1.5f);
    return pressed;
}

bool mksub(const char* txt, bool active) {
    auto p = ImGui::GetCursorScreenPos(); ImVec2 sz(ImGui::GetContentRegionAvail().x, 26); ImRect bb(p, ImVec2(p.x + sz.x, p.y + sz.y));
    ImGui::ItemSize(sz, 0.0f); if (!ImGui::ItemAdd(bb, 0)) return false;
    bool h, c; bool pressed = ImGui::ButtonBehavior(bb, ImGui::GetID(txt), &h, &c);
    auto draw = ImGui::GetWindowDrawList(); float m = ImGui::GetStyle().Alpha;

    if (active) draw->AddRectFilled(bb.Min, ImVec2(bb.Min.x + 2, bb.Max.y), ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_CheckMark]));
    draw->AddText(ImVec2(bb.Min.x + 12, bb.Min.y + (sz.y - ImGui::CalcTextSize(txt).y) * 0.5f), active ? ImGui::GetColorU32(ImVec4(1,1,1,m)) : h ? ImGui::GetColorU32(ImVec4(0.8f,0.8f,0.8f,m)) : ImGui::GetColorU32(ImVec4(0.5f,0.5f,0.5f,m)), txt);
    return pressed;
}

void mkbind(const char* id, bind_t& b) {
    ImGui::SameLine(ImGui::GetWindowWidth() - 50 - ImGui::GetStyle().WindowPadding.x);
    auto txt = b.bind ? "..." : "[" + std::string(keynm(b.key)) + "]";
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1,1,1,0.05f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_Text, b.isact() ? ImGui::GetStyle().Colors[ImGuiCol_CheckMark] : ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);

    ImGui::Button((txt + "##" + id).c_str(), ImVec2(50, 0));
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) b.bind = true;
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) ImGui::OpenPopup(("ctx_" + std::string(id)).c_str());
    ImGui::PopStyleColor(4);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup(("ctx_" + std::string(id)).c_str())) {
        ImGui::TextDisabled("mode"); ImGui::Separator();
        if (ImGui::Selectable("always", !b.mode)) b.mode = 0;
        if (ImGui::Selectable("hold", b.mode == 1)) b.mode = 1;
        if (ImGui::Selectable("toggle", b.mode == 2)) b.mode = 2;
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

    if (b.bind) {
        for (int i = 1; i < 256; i++) {
            if (i == VK_LBUTTON || i == VK_RBUTTON || i == VK_MBUTTON) continue;
            if (presschk(i)) { b.key = (i == VK_ESCAPE) ? 0 : i; b.bind = false; }
        }
    }
}