#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <SDL_ttf.h>
using namespace std;
class AppObject
{
public:
	AppObject() {};
	void PreInit(const char* img_path);
	void BasicInit(SDL_Renderer* renderer, int w, int h, int x, int y);
	virtual void Init(SDL_Renderer* renderer, int w, int h, int x, int y);
	virtual void draw();
	virtual void update();
	virtual void ChangeImage(const char* img_path);
	const char* image_path;
	SDL_Texture* texture;
	SDL_Renderer* renderer_m;
	SDL_Rect* srcR, destR;
	int width, height;
	int x_m, y_m;
	int id;
};

class Text : public AppObject{
public:
	Text(string font, string text, SDL_Color textcolor, int textsize);
	void BasicInit(SDL_Renderer* renderer, int w, int h, int x, int y);
	void Init(SDL_Renderer* renderer, int w, int h, int x, int y);
	void draw();
	void update();
	void ChangeText();
	TTF_Font* font_m;
	SDL_Texture* texture;
	SDL_Rect* srcR, destR;
	SDL_Color textcolor_m;
	SDL_Renderer* renderer_m;
	int width, height;
	int x_m, y_m;
	int textsize_m;
	SDL_Surface* tmpsurface;
};

class Button : public AppObject {
public:
	Button(const char* img_path, const char* hovered_img_path, Text* text, int text_w, int text_h, int x_offset, int y_offset);
	void button_update(int CollisionVal);
	void Init(SDL_Renderer* renderer, int w, int h, int x, int y);
	void draw();
	void input(SDL_Event e);
private:
	const char* hovered_image_path;
	const char* image_path1;
	int CollisionVal_m;
	Text* text_m;
	int text_w_m;
	int text_h_m;
	int x_offset_m;
	int y_offset_m;
};