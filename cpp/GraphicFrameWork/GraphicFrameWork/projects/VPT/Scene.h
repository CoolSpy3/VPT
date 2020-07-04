#pragma once
#include "../../Include.h"
#include "Packet.h"
#include "client.h"
#include <map>
#include "Utils.h"
#include "ResultId.h"

class Scene {
public:
	Scene();
	virtual void Init(AppInstance* instance);
	virtual void PostInit() {}
	virtual void LoadStaticComponents() = 0;
	virtual void LoadComponents() = 0;
	virtual void AttachListeners();
	virtual void Update();
	virtual void Input(SDL_Event e);
	virtual void Draw();
	virtual void ProcessPacket(Packet p) {}
	virtual Scene& Create() = 0;
	const int id;
	AppInstance* instance;
	ComponentManager cm;
	vector <AppObject*> Objects;
	vector <AppObject*> Overlays;
	bool needsCollisions = true;
	bool doneInitOnInit = true;
};

class StaticScene : public Scene {
public:
	virtual Scene& StaticCreate(Scene& classRef);
	virtual Scene& DoCreate() = 0;
	static map<string, Scene*>* staticScenes;
};

class DynamicScene : public Scene {
public:
	DynamicScene() { doneInitOnInit = false; }
	virtual void PostInit();
	virtual void PostInitD() {};
	virtual void BeginLoadingDynamicComponents() = 0;
};

class LoadingScreen : public StaticScene {
public:
	LoadingScreen() { needsCollisions = false; }
	virtual void LoadStaticComponents();
	virtual void LoadComponents() {}
	virtual void Update();
	virtual Scene& Create() { return StaticCreate(*this); }
	virtual Scene& DoCreate() { return *new LoadingScreen(); }
	double textSpeed;
	static Text* text;
	static LoadingSymbol* loadingSymbol;
};

class LoginScreen : public Scene {
public:
	virtual void LoadStaticComponents();
	virtual void LoadComponents();
	void login();
	void doLogin();
	virtual Scene& Create() { return *new LoginScreen(); }
	TextField* usernameField;
	TextField* passwordField;
	static Text* text;
	static SimpleButton* loginButton;
};

class StandardQuestion : public Scene {
public:
	StandardQuestion(string question) { question_m = question; }
	virtual void LoadStaticComponents();
	virtual void LoadComponents();
	virtual Scene& Create() { return *new StandardQuestion(question_m); }
	TextField* answer_box;
	Text* prompt;
	string question_m;
};

