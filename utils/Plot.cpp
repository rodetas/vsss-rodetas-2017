#include "Plot.h"

Plot::Plot(string type, float y_max, float y_min, float x_max, float x_min){
	if (type == "TEMPORAL"){
		graph.initializeTemporalGraph(y_max, y_min);
	} else if (type == "NO_TEMPORAL"){
		graph.initializeNoTemporalGraph(y_max, y_min, x_max, x_min);
	}
}

int Plot::GUI() {
	
	Glib::RefPtr<Gtk::Application> app;
		app = Gtk::Application::create();

	Gtk::Window window;	
		window.set_title("Rodetas");
		window.set_default_size(600, 400);

	sigc::connection draw_connection = Glib::signal_timeout().connect( sigc::mem_fun(this, &Plot::updateData) , 10 );

	window.add(graph);
	window.show_all();
	
  	app->run(window);

	draw_connection.disconnect();

	return MENU;
}

bool Plot::updateData(){
	graph.updateNoTemporalGraph(i*0.01, i*0.01);
	i++;
	return true;
}