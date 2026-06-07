#include "profile.hpp"
#include "ctx.hpp"
#include "imgui.h"
#include <sstream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string dumpcfg() {
    std::stringstream ss;
    ss << ctx.espen << "|" << ctx.box << "|" << ctx.name << "|" << ctx.hp << "|" << ctx.glwen << "|"
       << ctx.boxclr[0] << "|" << ctx.boxclr[1] << "|" << ctx.boxclr[2] << "|"
       << ctx.nmclr[0] << "|" << ctx.nmclr[1] << "|" << ctx.nmclr[2] << "|"
       << ctx.glwclr[0] << "|" << ctx.glwclr[1] << "|" << ctx.glwclr[2] << "|"
       << ctx.aim << "|" << ctx.aimkey.key << "|" << ctx.aimkey.mode << "|"
       << ctx.drawfov << "|" << ctx.fov << "|" << ctx.smooth << "|"
       << ctx.hitbox << "|" << ctx.trig << "|" << ctx.trigkey.key << "|" << ctx.trigkey.mode << "|"
       << ctx.trigdel << "|" << ctx.bhop << "|" << ctx.strafe << "|" << ctx.wm << "|" << ctx.showbinds << "|"
       << ctx.nade << "|" << ctx.bomb;
    return ss.str();
}

void parsecfg(const std::string& data) {
    std::stringstream ss(data); std::string item; std::vector<std::string> t;
    while (std::getline(ss, item, '|')) t.push_back(item);
    if (t.size() >= 31) {
        ctx.espen = std::stoi(t[0]); ctx.box = std::stoi(t[1]); ctx.name = std::stoi(t[2]); ctx.hp = std::stoi(t[3]); ctx.glwen = std::stoi(t[4]);
        ctx.boxclr[0] = std::stof(t[5]); ctx.boxclr[1] = std::stof(t[6]); ctx.boxclr[2] = std::stof(t[7]);
        ctx.nmclr[0] = std::stof(t[8]); ctx.nmclr[1] = std::stof(t[9]); ctx.nmclr[2] = std::stof(t[10]);
        ctx.glwclr[0] = std::stof(t[11]); ctx.glwclr[1] = std::stof(t[12]); ctx.glwclr[2] = std::stof(t[13]);
        ctx.aim = std::stoi(t[14]); ctx.aimkey.key = std::stoi(t[15]); ctx.aimkey.mode = std::stoi(t[16]);
        ctx.drawfov = std::stoi(t[17]); ctx.fov = std::stof(t[18]); ctx.smooth = std::stof(t[19]);
        ctx.hitbox = std::stoi(t[20]); ctx.trig = std::stoi(t[21]); ctx.trigkey.key = std::stoi(t[22]); ctx.trigkey.mode = std::stoi(t[23]);
        ctx.trigdel = std::stoi(t[24]); ctx.bhop = std::stoi(t[25]); ctx.strafe = std::stoi(t[26]); ctx.wm = std::stoi(t[27]); ctx.showbinds = std::stoi(t[28]);
        ctx.nade = std::stoi(t[29]); ctx.bomb = std::stoi(t[30]);
    }
}

std::string encstr(const std::string& in) {
    std::string x = in; for (char& c : x) c ^= 0x5C; 
    const char hex[] = "0123456789ABCDEF"; std::string out;
    for (unsigned char c : x) { out += hex[c >> 4]; out += hex[c & 0xF]; }
    return "EMP-" + out; 
}

std::string decstr(std::string in) {
    if (in.substr(0, 4) == "EMP-") in = in.substr(4);
    std::string out;
    for (size_t i = 0; i < in.length(); i += 2) out += (char)strtol(in.substr(i, 2).c_str(), nullptr, 16);
    for (char& c : out) c ^= 0x5C;
    return out;
}

std::vector<std::string> getcfgs() {
    std::vector<std::string> l;
    if (!fs::exists("configs")) fs::create_directory("configs");
    for (const auto& e : fs::directory_iterator("configs")) if (e.path().extension() == ".txt") l.push_back(e.path().stem().string());
    return l;
}

void savecfg(const std::string& name) {
    if (!fs::exists("configs")) fs::create_directory("configs");
    if (std::ofstream out("configs/" + name + ".txt"); out.is_open()) out << encstr(dumpcfg());
}

void loadcfg(const std::string& name) {
    if (std::ifstream in("configs/" + name + ".txt"); in.is_open()) parsecfg(decstr(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>())));
}

void drawprofile() {
    ImGui::BeginChild("cfg_m", ImVec2(0, 0), true);
    ImGui::TextDisabled("CONFIG MANAGER"); ImGui::Dummy(ImVec2(0, 5));
    static auto list = getcfgs();
    
    ImGui::PushItemWidth(-1);
    if (ImGui::ListBoxHeader("##cfgs", ImVec2(0, 140))) {
        for (size_t i = 0; i < list.size(); i++) {
            if (ImGui::Selectable(list[i].c_str(), ctx.cfgidx == static_cast<int>(i))) ctx.cfgidx = static_cast<int>(i);
        }
        ImGui::ListBoxFooter();
    }
    ImGui::PopItemWidth(); ImGui::Dummy(ImVec2(0, 5));

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 70);
    ImGui::InputText("##cfg_name", ctx.cfgnm, IM_ARRAYSIZE(ctx.cfgnm)); ImGui::SameLine();
    if (ImGui::Button("Create", ImVec2(60, 0)) && strlen(ctx.cfgnm) > 0) { savecfg(ctx.cfgnm); list = getcfgs(); }

    ImGui::Dummy(ImVec2(0, 10));
    if (ImGui::Button("Save Selected", ImVec2(130, 25)) && !list.empty()) savecfg(list[ctx.cfgidx]); ImGui::SameLine();
    if (ImGui::Button("Load Selected", ImVec2(130, 25)) && !list.empty()) loadcfg(list[ctx.cfgidx]); ImGui::SameLine();
    if (ImGui::Button("Refresh", ImVec2(75, 25))) list = getcfgs();

    ImGui::Dummy(ImVec2(0, 10)); ImGui::TextDisabled("SHARING"); ImGui::Dummy(ImVec2(0, 5));
    if (ImGui::Button("Export to Clipboard", ImVec2(160, 25))) { ImGui::SetClipboardText(encstr(dumpcfg()).c_str()); ctx.cpytime = 2.0f; } ImGui::SameLine();
    if (ImGui::Button("Import from Clipboard", ImVec2(160, 25))) { if (const char* clip = ImGui::GetClipboardText()) parsecfg(decstr(clip)); }
    if (ctx.cpytime > 0.0f) { ctx.cpytime -= ImGui::GetIO().DeltaTime; ImGui::SameLine(); ImGui::TextColored(ImGui::GetStyle().Colors[ImGuiCol_CheckMark], "Copied!"); }
    ImGui::EndChild();
}