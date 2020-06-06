#include "client.h"
const string path = "..\\PrinceValiant.ttf";

client::client::client()
{
}

void client::client::Init(const char* window_title, int w, int h)
{
	BasicInit(window_title, w, h);
	string img_path = "..\\";
	//init objects here

	//AppObjects vector because is need for collision component
	AppObjects.push_back(new AppObject());
	AppObjects[0]->PreInit("");
	AppObjects[0]->Init(renderer, 1, 1, 0, 0);
	//AppObjects vector because is need for collision component

	//text init
	text = new Text(path, "VPT", SDL_Color{ 0, 0, 0, 255 }, 100);
	text->Init(renderer, 0, 0, 270, 0);
	AppObjects.push_back(text);
	//text init

	//button init
	//button init

	//tf init
	TextFieldData tfd;
	tf = new TextField(path, tfd.textsize, tfd.x_offset, tfd.y_offset);
	tf->Init(renderer, tfd.w, tfd.h, 270, 160);
	AppObjects.push_back(tf);
	tf1 = new TextField(path, tfd.textsize, tfd.x_offset, tfd.y_offset);
	tf1->Init(renderer, tfd.w, tfd.h, 270, 270);
	AppObjects.push_back(tf1);
	//tg init

	for (unsigned int i = 0; i < AppObjects.size(); i++) {
		AppObjects[i]->id = i;
	}
	//init objects here


	//init components here
	for (AppObject* object : AppObjects) {
		if (TextField* TextFieldObj = dynamic_cast<TextField*>(object)) {
			cm.AttachComponent(new CollisionBox(AppObjects), object);
		}
	}
	//init components here
}

void client::client::Draw()
{
	SDL_RenderClear(renderer);
	for (AppObject* object : AppObjects) {
		object->draw();
	}
	SDL_RenderPresent(renderer);
}

void client::client::Update()
{
	int UpdateVal;
	for (AppObject* object : AppObjects) {
		object->update();
	}
	for (Component* c : cm.UpdateSectorComponents) {
		UpdateVal = c->run(AppObjects);
		AppObject* object1 = AppObjects[c->parent_m->id];
		if (TextField* TextFieldObj = dynamic_cast<TextField*>(object1)) {
			SDL_StartTextInput();
			TextFieldObj->TextFieldupdate(UpdateVal);
			
		}
		else {
			SDL_StopTextInput();
		}
	}
}

void client::client::Input()
{
	for (Component* c: cm.InputSectorComponents) {
		c->run(AppObjects);
	}
	if (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) { running = false; }
		if (e.type == SDL_MOUSEMOTION) {
			AppObjects[0]->x_m = e.motion.x;
			AppObjects[0]->y_m = e.motion.y;
		}
		for (AppObject* object : AppObjects) {
			object->input(e);
		}
	}
	
}
