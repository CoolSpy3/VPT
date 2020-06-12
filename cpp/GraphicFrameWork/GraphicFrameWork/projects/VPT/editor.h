#include <iostream>
#include "../../Instance.h"
#include <vector>
#include "../../Component.h"
#include <cstdlib> 
#include <ctime> 
namespace editor {
	struct AppData
	{
		const char* win_name = "Editor";
		int w = 1000;
		int h = 700;
	};
	
	class PlaceableBounding : public AppObject{
	public:
		PlaceableBounding() { };
		virtual string PrintReleventData();
		int collideid;
	};

	class PlaceableButton : public PlaceableBounding {
	public:
		PlaceableButton() { };
		string PrintReleventData() {
			return "Button," + to_string(x_m) + "," + to_string(y_m) + "," + to_string(width) + "," + to_string(height);
		};
	};

	struct TextBoxData {
		int textsize = 30;
		int x_offset = 125;
		int y_offset = 25;
		int w = 500;
		int h = 100;
	};
	class editor : public AppInstance {
	public:
		editor() {};
		void Init(const char* window_title, int w, int h);
		void Draw();
		void Update();
		void Input();
		void Loop();
	private:
		int UpdateVal;
		bool ButtontbClicked;
		TextBox* tb;
	};
	
}