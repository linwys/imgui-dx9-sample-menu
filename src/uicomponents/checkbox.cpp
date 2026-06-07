#include "checkbox.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>

bool cbx(const char* label, bool* v) {
    ImGuiWindow* w = ImGui::GetCurrentWindow();
    if (w->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const auto& style = g.Style;
    const auto id = w->GetID(label);
    const auto lsz = ImGui::CalcTextSize(label, nullptr, true);
    
    const float sq = 11.0f; 
    const auto pos = w->DC.CursorPos;
    const ImRect bb(pos, ImVec2(pos.x + sq + (lsz.x > 0.0f ? style.ItemInnerSpacing.x + lsz.x : 0.0f), pos.y + lsz.y + style.FramePadding.y * 2.0f));
    
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool h, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &h, &held);
    if (pressed) { *v = !(*v); ImGui::MarkItemEdited(id); }

    float anim = w->StateStorage.GetFloat(id, 0.0f);
    float target = *v ? 1.0f : 0.0f;
    if (anim != target) {
        anim += (target - anim) * g.IO.DeltaTime * 7.0f; 
        if (std::abs(anim - target) < 0.01f) anim = target;
        w->StateStorage.SetFloat(id, anim);
    }

    const ImRect cbb(ImVec2(pos.x, pos.y + (bb.GetHeight() - sq) * 0.5f), ImVec2(pos.x + sq, pos.y + (bb.GetHeight() - sq) * 0.5f + sq));
    const auto cmark = style.Colors[ImGuiCol_CheckMark];
    const auto fbg = style.Colors[ImGuiCol_FrameBg];
    
    ImVec4 curbg(fbg.x + (cmark.x - fbg.x) * anim, fbg.y + (cmark.y - fbg.y) * anim, fbg.z + (cmark.z - fbg.z) * anim, style.Alpha);
    w->DrawList->AddRectFilled(cbb.Min, cbb.Max, ImGui::GetColorU32(curbg), 1.0f);
    w->DrawList->AddRect(cbb.Min, cbb.Max, ImGui::GetColorU32(h ? ImVec4(0.3f, 0.3f, 0.3f, style.Alpha) : ImVec4(0.15f, 0.15f, 0.15f, style.Alpha)), 0.0f);
    
    if (anim > 0.01f) {
        const auto ccol = ImGui::GetColorU32(ImVec4(0.04f, 0.04f, 0.04f, style.Alpha * anim));
        const auto start = ImVec2(cbb.Min.x + 2.5f, cbb.Min.y + sq * 0.5f);
        const auto mid = ImVec2(cbb.Min.x + sq * 0.4f, cbb.Max.y - 3.0f);
        w->DrawList->AddLine(start, ImVec2(start.x + (mid.x - start.x) * ImMin(anim * 1.5f, 1.0f), start.y + (mid.y - start.y) * ImMin(anim * 1.5f, 1.0f)), ccol, 1.5f);
        if (anim > 0.6f) w->DrawList->AddLine(mid, ImVec2(mid.x + (ImVec2(cbb.Max.x - 2.0f, cbb.Min.y + 2.5f).x - mid.x) * ((anim - 0.6f) / 0.4f), mid.y + (ImVec2(cbb.Max.x - 2.0f, cbb.Min.y + 2.5f).y - mid.y) * ((anim - 0.6f) / 0.4f)), ccol, 1.5f);
    }
    if (lsz.x > 0.0f) ImGui::RenderText(ImVec2(cbb.Max.x + style.ItemInnerSpacing.x, pos.y + style.FramePadding.y), label);
    return pressed;
}