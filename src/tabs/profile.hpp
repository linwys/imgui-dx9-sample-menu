#pragma once
#include <string>
#include <vector>

std::string dumpcfg();
void parsecfg(const std::string& data);
std::string encstr(const std::string& in);
std::string decstr(std::string in);
std::vector<std::string> getcfgs();
void savecfg(const std::string& name);
void loadcfg(const std::string& name);
void drawprofile();