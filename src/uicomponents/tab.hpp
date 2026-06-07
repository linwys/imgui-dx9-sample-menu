#pragma once
#include "imgui.h"
#include "ctx.hpp"

extern ImFont* fnrm;
extern ImFont* fttl;

extern bool mktab(const char* txt, bool active, ImVec2 sz);
extern bool mksub(const char* txt, bool active);
extern void mkbind(const char* id, bind_t& b);