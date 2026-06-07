#include <windows.h>
#include <d3d9.h>
#include <dwmapi.h>
#include <algorithm>
#include <string>
#include "imgui.h"
#include "ctx.hpp"
#include "render.hpp"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

ImFont* fnrm = nullptr;
ImFont* fttl = nullptr;

bool presschk(int vk) { return keys[vk] && !oldkeys[vk]; }
bool downchk(int vk) { return keys[vk]; }

void bind_t::updbind() {
    if (mode == 2 && presschk(key) && key != 0) active = !active;
}

bool bind_t::isact() const {
    if (mode == 0) return true;
    if (!key) return false;
    return (mode == 1) ? downchk(key) : active;
}

std::string_view keynm(int vk) {
    if (!vk) return "-";
    static char n[128];
    UINT sc = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);
    if (vk == VK_LEFT || vk == VK_UP || vk == VK_RIGHT || vk == VK_DOWN || vk == VK_PRIOR || vk == VK_NEXT || vk == VK_END || vk == VK_HOME || vk == VK_INSERT || vk == VK_DELETE || vk == VK_DIVIDE || vk == VK_NUMLOCK || vk == VK_XBUTTON1 || vk == VK_XBUTTON2) sc |= KF_EXTENDED;
    if (vk == VK_LBUTTON) return "M1"; if (vk == VK_RBUTTON) return "M2"; if (vk == VK_MBUTTON) return "M3"; if (vk == VK_XBUTTON1) return "M4"; if (vk == VK_XBUTTON2) return "M5";
    return (!GetKeyNameTextA(sc << 16, n, sizeof(n))) ? std::string_view(std::to_string(vk)) : std::string_view(n);
}

static LPDIRECT3D9 d3d = nullptr; 
static LPDIRECT3DDEVICE9 dev = nullptr; 
static D3DPRESENT_PARAMETERS d3dpp = {};

bool initd3d(HWND hwnd) {
    if (!(d3d = Direct3DCreate9(D3D_SDK_VERSION))) return false;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE; d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; d3dpp.EnableAutoDepthStencil = TRUE; d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    return d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dev) >= 0;
}

void freed3d() { if (dev) { dev->Release(); dev = nullptr; } if (d3d) { d3d->Release(); d3d = nullptr; } }

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
LRESULT WINAPI wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) return true;
    if (msg == WM_SIZE && dev && wparam != SIZE_MINIMIZED) { d3dpp.BackBufferWidth = LOWORD(lparam); d3dpp.BackBufferHeight = HIWORD(lparam); dev->Reset(&d3dpp); }
    return (msg == WM_DESTROY) ? (PostQuitMessage(0), 0) : DefWindowProc(hwnd, msg, wparam, lparam);
}

int APIENTRY WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, wndproc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, "b_wnd", nullptr };
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, "b_wnd", "ovr", WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);

    if (!initd3d(hwnd)) { freed3d(); UnregisterClass("b_wnd", wc.hInstance); return 1; }
    ShowWindow(hwnd, show); UpdateWindow(hwnd);
    
    IMGUI_CHECKVERSION(); ImGui::CreateContext(); ImGuiIO& io = ImGui::GetIO(); 
    fnrm = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 14.0f);
    fttl = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 16.0f);

    ImGui_ImplWin32_Init(hwnd); ImGui_ImplDX9_Init(dev);

    bool done = false; float anim = 1.0f;
    while (!done) {
        MSG msg; while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessage(&msg); if (msg.message == WM_QUIT) done = true; } if (done) break;
        for (int i = 0; i < 256; i++) { oldkeys[i] = keys[i]; keys[i] = GetAsyncKeyState(i) & 0x8000; }
        
        if (presschk(VK_INSERT)) ctx.open = !ctx.open;
        ctx.aimkey.updbind(); if (ctx.aimkey.mode == 2 && presschk(ctx.aimkey.key) && ctx.aimkey.key != 0) { ctx.aim = !ctx.aim; ctx.aimkey.active = ctx.aim; }
        ctx.trigkey.updbind(); if (ctx.trigkey.mode == 2 && presschk(ctx.trigkey.key) && ctx.trigkey.key != 0) { ctx.trig = !ctx.trig; ctx.trigkey.active = ctx.trig; }

        anim = std::clamp(anim + (ImGui::GetIO().DeltaTime * 7.0f * (ctx.open ? 1.0f : -1.0f)), 0.0f, 1.0f);

        ImGui_ImplDX9_NewFrame(); ImGui_ImplWin32_NewFrame(); ImGui::NewFrame();
        if (ctx.showbinds) drawovr();
        if (anim > 0.01f) drawmenu(anim);
        ImGui::EndFrame();

        dev->SetRenderState(D3DRS_ZENABLE, FALSE); dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); dev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        dev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(15, 15, 15), 1.0f, 0);
        if (dev->BeginScene() >= 0) { ImGui::Render(); ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData()); dev->EndScene(); }
        dev->Present(nullptr, nullptr, nullptr, nullptr);
    }
    ImGui_ImplDX9_Shutdown(); ImGui_ImplWin32_Shutdown(); ImGui::DestroyContext(); freed3d(); DestroyWindow(hwnd); UnregisterClass("b_wnd", wc.hInstance);
    return 0;
}