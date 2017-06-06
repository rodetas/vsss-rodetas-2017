#ifndef TEST_H_
#define TEST_H_

#include "Header.h"
#include <gtkmm.h>

class Test {

private:
	Glib::RefPtr<Gtk::Application> app;
	Gtk::Window* window = nullptr;
	Gtk::Arrow* up_arrow = nullptr;
	Gtk::Arrow* down_arrow = nullptr;
	Gtk::Arrow* left_arrow = nullptr;
	Gtk::Arrow* right_arrow = nullptr;
	
	int program_state;

public:
	Test();	
	~Test();	
	int GUI();
	bool onKeyboard(GdkEventKey*);

};

#endif