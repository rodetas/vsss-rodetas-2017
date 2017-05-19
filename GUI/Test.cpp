#include "Test.h"

Test::Test(){
}

Test::~Test(){
}

int Test::GUI(){

	program_state = MENU;
			
	app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("GUI/Glade/Test.glade");
	
	builder->get_widget("Window Test", window);
	builder->get_widget("Arrow Up", up_arrow);
	builder->get_widget("Arrow Down", down_arrow);
	builder->get_widget("Arrow Left", left_arrow);
	builder->get_widget("Arrow Right", right_arrow);

	window->signal_key_press_event().connect(sigc::mem_fun(this, &Test::onKeyboard), false);
	window->signal_key_release_event().connect(sigc::mem_fun(this, &Test::onKeyboard), false);

	app->run(*window);

	return program_state;
}


bool Test::onKeyboard(GdkEventKey* event){
	
	if(event->type == GDK_KEY_RELEASE){
		left_arrow->set_opacity(0.2);
		right_arrow->set_opacity(0.2);
		up_arrow->set_opacity(0.2);	
		down_arrow->set_opacity(0.2);	

	} else if (event->keyval == GDK_KEY_Left) {
		left_arrow->set_opacity(1);

    } else if (event->keyval == GDK_KEY_Right) {
		right_arrow->set_opacity(1);

	} else if (event->keyval == GDK_KEY_Up) {
		up_arrow->set_opacity(1);

	} else if (event->keyval == GDK_KEY_Down) {
		down_arrow->set_opacity(1);
	}

    return true;
}