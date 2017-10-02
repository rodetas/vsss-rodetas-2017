#include "GameView.h"

GameView::GameView(){
    game_model.setCaller(this);
    program_state = MENU;
}

GameView::~GameView(){
}

int GameView::GUI() {
    
    auto app = Gtk::Application::create();
    auto builder = Gtk::Builder::create_from_file("game/view/Game.glade");
    
    builder->get_widget("Window Control", window);
    window->signal_key_press_event().connect(sigc::mem_fun(this, &GameView::onKeyboard), false);
    window->signal_key_release_event().connect(sigc::mem_fun(this, &GameView::onKeyboard), false);
    window->signal_delete_event().connect(sigc::mem_fun(this, &GameView::deleteConnection), false);
    window->maximize();
    
    builder->get_widget("Button Paused", button_play); 
    button_play->signal_clicked().connect(sigc::mem_fun(this, &GameView::onButtonPlay));
    
    builder->get_widget("Button Penalty", button_penalty); 
    //button_penalty->signal_clicked().connect(sigc::mem_fun(this, &GameView::));
    
    builder->get_widget("Button Time", button_side);
    button_side->signal_clicked().connect(sigc::mem_fun(this, &GameView::onButtonTime));
    
    builder->get_widget("Selector Potency", spin_potency);
    spin_potency->signal_value_changed().connect(sigc::mem_fun(this, &GameView::onPotencyChanged));
    
    builder->get_widget("Selector Curve", spin_curve);
    spin_curve->signal_value_changed().connect(sigc::mem_fun(this, &GameView::onCurveChanged));
    
    builder->get_widget("Label FPS", label_fps);
    
    builder->get_widget("Transmission Erro", label_transmission);
    
    builder->get_widget("Menu Calibrate", menu_calibration);
    menu_calibration->signal_activate().connect(sigc::mem_fun(this, &GameView::onMenuCalibration));
    
    builder->get_widget("Menu Simulate", menu_simulator);
    menu_simulator->signal_activate().connect(sigc::mem_fun(this, &GameView::onMenuSimulator));
    
    builder->get_widget("Menu Upload Arduino", menu_arduino);
    menu_arduino->signal_activate().connect(sigc::mem_fun(this, &GameView::onMenuArduino));
    
    builder->get_widget("Menu Quit", menu_quit);
    menu_quit->signal_activate().connect(sigc::mem_fun(this, &GameView::onMenuQuit));
    
    GameDraw draw_robot;
    signal_draw_robot.connect(sigc::mem_fun(draw_robot, &GameDraw::setPosition));

    builder->get_widget("Box", box);
    box->pack_start(draw_robot);
    
    window->show_all();
    
    sigc::slot<bool> control_slot = sigc::mem_fun(game_model, &GameModel::control);
    control_connection = Glib::signal_timeout().connect(control_slot, 33, Glib::PRIORITY_DEFAULT_IDLE);     

    app->run(*window);

    return program_state;
}

void GameView::updateScreen(){

    signal_draw_robot.emit(game_model.getTeam(), game_model.getOpponent(), game_model.getBall());

	if(game_model.getConnectionStatus() == false){
		label_transmission->set_visible(true);
	} else {
		label_transmission->set_visible(false);
	}
	
	label_fps->set_label("Fps: " + to_string(game_model.getFps()));
}

bool GameView::onKeyboard(GdkEventKey* event){

    if(event->type == GDK_KEY_RELEASE){
        game_model.manualRobotControl(STOPPED_MOVE, 0, 0);
    
    } else if (event->keyval == GDK_KEY_Left) {
        game_model.manualRobotControl(LEFT_MOVE, 150, 150);

    } else if (event->keyval == GDK_KEY_Right) {
        game_model.manualRobotControl(RIGHT_MOVE, 150, 150);
    
    } else if (event->keyval == GDK_KEY_Up) {
        game_model.manualRobotControl(FORWARD_MOVE, 150, 150);

    } else if (event->keyval == GDK_KEY_Down) {
        game_model.manualRobotControl(BACK_MOVE, 150, 150);

    } else if(event->keyval == GDK_KEY_space) {
        button_play->set_active(!button_play->get_active());
        
    } else if(event->keyval == GDK_KEY_Escape){
        onMenuQuit();
    
    } else {
        game_model.manualRobotControl(STOPPED_MOVE, 0, 0);
    }

    return true;
}

bool GameView::deleteConnection(GdkEventAny* event){
    control_connection.disconnect();
    return false;
}

void GameView::onPotencyChanged(){
	game_model.setPotencyFactor(spin_potency->get_value());
}

void GameView::onCurveChanged(){
	game_model.setCurveFactor(spin_curve->get_value());
}

void GameView::onButtonPlay(){
	if(button_play->get_active()){
		button_play->set_label("Playing");
	} else {
		button_play->set_label("Paused");
        game_model.manualRobotControl(STOPPED_MOVE, 0, 0);
	}

    game_model.setPlay(button_play->get_active());
}

void GameView::onButtonTime(){
	if(button_side->get_active()){
		button_side->set_label("2º time");
    } else {
		button_side->set_label("1º time");
	}

    game_model.setSide(button_side->get_active());
}

void GameView::onMenuCalibration(){
    program_state = CALIBRATION; window->close();
}

void GameView::onMenuSimulator(){
    program_state = SIMULATOR; window->close();
}

void GameView::onMenuArduino(){
    program_state = ARDUINO; window->close();
}

void GameView::onMenuQuit(){
    program_state = MENU; window->close();
}