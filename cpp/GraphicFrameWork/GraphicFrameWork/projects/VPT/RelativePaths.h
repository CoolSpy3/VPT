#pragma once
#include <string>
using namespace std;
#define USE_DEBUG_CLIENT

#ifndef USE_DEBUG_CLIENT
const string dir = "..\\..\\cpp\\GraphicFrameWork\\GraphicFrameWork\\projects\\VPT\\";
#else
const string dir = "..\\";
#endif

const string fontPath = dir + "PrinceValiant.ttf";