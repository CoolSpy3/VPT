#include "editor.h"

void editor::editor::Init(const char* window_title, int w, int h)
{
	BasicInit(window_title, w, h);
	AppObjects.push_back(new AppObject());
	AppObjects[0]->PreInit("");
	AppObjects[0]->Init(renderer, 1, 1, 0, 0);
	
	TextBoxData tbd;
	tb = new TextBox(fontPath, 
	tbd.textsize, tbd.x_offset, tbd.y_offset, "Button");
	tb->Init(renderer, tbd.w, tbd.h, 0, 0);
	AppObjects.push_back(tb);

	for (unsigned int i = 0; i < AppObjects.size(); i++) {
		AppObjects[i]->id = i;
	}

	for (unsigned int t = 0; t < AppObjects.size(); t++) {
		cm.AttachComponent(new CollisionBox(AppObjects), AppObjects[t]);
	}
}

void editor::editor::Draw()
{
	SDL_RenderClear(renderer);
	for (AppObject* object : AppObjects) {
		object->draw();
	}
	SDL_RenderPresent(renderer);
}

void editor::editor::Update()
{
	for (Component* c : cm.UpdateSectorComponents) {
		UpdateVal = c->run(AppObjects);
		AppObject* object = AppObjects[c->parent_m->id];
		if (TextBox* obj = dynamic_cast<TextBox*>(object)) {
			if (UpdateVal != -1) {
				message_m = obj->message;
				
			}
			else {
				message_m = "";
			}
		}
		else {
			message_m = "";
		}
	}
	for (AppObject* object : AppObjects) {
		object->update();
	}
	//cout << message_m << endl;
}

void editor::editor::Input()
{
	for (Component* c : cm.UpdateSectorComponents) {
		AppObjects[c->parent_m->id]->collide(c->run(AppObjects));
	}
	if (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) { running = false; }
		if (e.type == SDL_MOUSEMOTION) {
			AppObjects[0]->x_m = e.motion.x;
			AppObjects[0]->y_m = e.motion.y;
			for (AppObject* object : AppObjects) {
				if (PlaceableBounding* obj = dynamic_cast<PlaceableBounding*>(object)) {
					if (object->id == 1) {
						object->x_m = AppObjects[0]->x_m - obj->selectW;
						object->y_m = AppObjects[0]->y_m - obj->selectH;
					}
				}
			}
		}
		if (e.type == SDL_MOUSEBUTTONUP) {
			AppObjSelected selectedval = Selected();
			if (selectedval.selected == true) {
				AppObjects[selectedval.index]->id = 0;
			}
			else {
				for (unsigned int i = 0; i < AppObjects.size(); i++) {
					if (PlaceableBounding* placeableObj = dynamic_cast<PlaceableBounding*>(AppObjects[i])) {
						if (Utils::contains(AppObjects[i]->getBounds(), SDL_Point{ AppObjects[0]->x_m, AppObjects[0]->y_m })) {
							placeableObj->id = 1;
							placeableObj->selectW = AppObjects[0]->x_m - placeableObj->x_m;
							placeableObj->selectH = AppObjects[0]->y_m - placeableObj->y_m;
							break;
						}
					}
				}
			}
			if (message_m == "Button") {
				PlaceableButton* newObj = new PlaceableButton();
				AppObjects.push_back(newObj);
				AppObjects[AppObjects.size() - 1]->PreInit((dir + "bounding.png").c_str());
				AppObjects[AppObjects.size() - 1]->Init(renderer, 100, 100, 400, 400);
				AppObjects[AppObjects.size() - 1]->id = 1;
				AppObjects[AppObjects.size() - 1]->x_m = AppObjects[0]->x_m - (AppObjects[AppObjects.size() - 1]->width / 2);
				AppObjects[AppObjects.size() - 1]->y_m = AppObjects[0]->y_m - (AppObjects[AppObjects.size() - 1]->height / 2);
				newObj->selectW = (AppObjects[AppObjects.size() - 1]->width / 2);
				newObj->selectH = (AppObjects[AppObjects.size() - 1]->height / 2);
			}
		}
		for (AppObject* object : AppObjects) {
			object->input(e);
		}
	}
}

editor::AppObjSelected editor::editor::Selected()
{
	for (unsigned int i = 0; i < AppObjects.size(); i++) {
		if (PlaceableBounding* obj = dynamic_cast<PlaceableBounding*>(AppObjects[i])) {
			if (AppObjects[i]->id == 1) {
				return AppObjSelected{ true, i};
			}
		}
	}
	return AppObjSelected{false, NULL};
}


string editor::PlaceableBounding::PrintReleventData()
{
	return string();
}
