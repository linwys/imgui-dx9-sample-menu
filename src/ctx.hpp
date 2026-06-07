#pragma once
#include <string_view>

struct bind_t {
    int key = 0;
    int mode = 1; 
    bool active = false;
    bool bind = false;

    void updbind();
    bool isact() const;
};

struct ctx_t {
    bool open = true;
    int tab = 0;
    int sub[5] = {0};
    bool espen = false;
    bool box = false;
    bool name = true;
    bool hp = false;
    bool glwen = false;

    float boxclr[3] = { 0.42f, 0.65f, 0.95f };
    float nmclr[3] = { 1.0f, 1.0f, 1.0f };
    float glwclr[3] = { 0.8f, 0.2f, 0.8f };

    bool aim = false;
    bool drawfov = true;
    bind_t aimkey;
    float fov = 120.f;
    float smooth = 3.f;
    int hitbox = 0;

    bool trig = false;
    bind_t trigkey;
    int trigdel = 0;

    bool bhop = true;
    bool strafe = false;
    bool wm = true;
    bool showbinds = false; 

    bool nade = false;
    bool bomb = true;
    
    int cfgidx = 0;
    char cfgnm[64] = "";
    float cpytime = 0.0f;
};

inline ctx_t ctx;
inline bool keys[256];
inline bool oldkeys[256];

bool presschk(int vk);
bool downchk(int vk);
std::string_view keynm(int vk);