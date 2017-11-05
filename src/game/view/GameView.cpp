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
    
    builder->get_widget("Button Time", button_side);
    button_side->signal_clicked().connect(sigc::mem_fun(this, &GameView::onButtonTime));
    
    builder->get_widget("Robot 0 Potency", spin_potency_robot0);
    spin_potency_robot0->signal_value_changed().connect(sigc::mem_fun(this, &GameView::onPotencyChangedRobot0));

    builder->get_widget("Robot 1 Potency", spin_potency_robot1);
    spin_potency_robot1->signal_value_changed().connect(sigc::mem_fun(this, &GameView::onPotencyChangedRobot1));

    builder->get_widget("Robot 2 Potency", spin_potency_robot2);
    spin_potency_robot2->signal_value_changed().connect(sigc::mem_fun(this, &GameView::onPotencyChangedRobot2));
    
    builder->get_widget("Robot 0 Curve", spin_curve_robot0);
    spin_curve_robot0->signal_value_changed().connect(sigc::mem_fun(this, &GameView::onCurveChangedRobot0));

    builder->get_widget("Robot 1 Curve", spin_curve_robot1);
    spin_curve_robot1->signal_value_changed().connect(sigc::mem_fun(this, &GameView::onCurveChangedRobot1));

    builder->get_widget("Robot 2 Curve", spin_curve_robot2);
    spin_curve_robot2->signal_value_changed().connect(sigc::mem_fun(this, &GameView::onCurveChangedRobot2));
    
    builder->get_widget("Popover Information", popover_information);
    builder->get_widget("Model Button Information", button_information);
    button_information->signal_clicked().connect( sigc::mem_fun(this, &GameView::onButtonInformation) );
        
    builder->get_widget("Radio Button Robot 0", radio_button_robot_0);
    radio_button_robot_0->signal_pressed().connect(sigc::mem_fun(this, &GameView::onRadioButton0));
    
    builder->get_widget("Radio Button Robot 1", radio_button_robot_1);
    radio_button_robot_1->signal_pressed().connect(sigc::mem_fun(this, &GameView::onRadioButton1));
    
    builder->get_widget("Radio Button Robot 2", radio_button_robot_2);
    radio_button_robot_1->signal_pressed().connect(sigc::mem_fun(this, &GameView::onRadioButton2));    
    
    builder->get_widget("Switch Robot", switch_robot);
    switch_robot->property_active().signal_changed().connect(sigc::mem_fun(this, &GameView::onSwitchRobot));
    onSwitchRobot();

    builder->get_widget("Label FPS", label_fps);    
    builder->get_widget("Label Transmission Erro", label_transmission);
    builder->get_widget("Label Robot 0", label_robot_0);
    builder->get_widget("Label Robot 1", label_robot_1);
    builder->get_widget("Label Robot 2", label_robot_2);
    builder->get_widget("Label Ball", label_ball);
    
    builder->get_widget("Menu Calibrate", menu_calibration);
    menu_calibration->signal_activate().connect(sigc::mem_fun(this, &GameView::onMenuCalibration));
    
    builder->get_widget("Menu Simulate", menu_simulator);
    menu_simulator->signal_activate().connect(sigc::mem_fun(this, &GameView::onMenuSimulator));
    
    builder->get_widget("Menu Upload Arduino", menu_arduino);
    menu_arduino->signal_activate().connect(sigc::mem_fun(this, &GameView::onMenuArduino));
    
    builder->get_widget("Menu Quit", menu_quit);
    menu_quit->signal_activate().connect(sigc::mem_fun(this, &GameView::onMenuQuit));
    
    GameDraw draw_robot;
    draw_robot.signal_button_press_event().connect(sigc::mem_fun(this, &GameView::onMouseClick));
    signal_draw_robot.connect(sigc::mem_fun(draw_robot, &GameDraw::setPosition));
    signal_press_event.connect(sigc::mem_fun(draw_robot, &GameDraw::onPressEvent));
    signal_draw_get_click.connect(sigc::mem_fun(draw_robot, &GameDraw::getButtonClick));

    builder->get_widget("Box", box);
    box->pack_start(draw_robot);
    
    window->show_all();
    
    sigc::slot<bool> control_slot = sigc::mem_fun(game_model, &GameModel::control);
    control_connection = Glib::signal_timeout().connect(control_slot, 32, Glib::PRIORITY_DEFAULT_IDLE);     

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
    
    if(game_model.getFps() > 24){
        label_fps->set_label("Fps: " + to_string(game_model.getFps()));
        label_fps->override_color(Gdk::RGBA("black"), Gtk::STATE_FLAG_NORMAL);
    } else {
        label_fps->set_markup("<b>Fps: " + to_string(game_model.getFps()) + "</b>");
        label_fps->override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_NORMAL);
    }

    onCurveChangedRobot0();
    onCurveChangedRobot1();
    onCurveChangedRobot2();

    onPotencyChangedRobot0();
    onPotencyChangedRobot1();
    onPotencyChangedRobot2();

    label_robot_0->set_label("Robot 0: " + to_string(game_model.getRobot(0)->getPosition().x) + " x " + to_string(game_model.getRobot(0)->getPosition().y));
    label_robot_1->set_label("Robot 1: " + to_string(game_model.getRobot(1)->getPosition().x) + " x " + to_string(game_model.getRobot(1)->getPosition().y));
    label_robot_2->set_label("Robot 2: " + to_string(game_model.getRobot(2)->getPosition().x) + " x " + to_string(game_model.getRobot(2)->getPosition().y));
    label_ball->set_label("Ball: " + to_string(game_model.getBall().getPosition().x) + " x " + to_string(game_model.getBall().getPosition().y));
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
    
    } else if(event->keyval == GDK_KEY_q){
        radio_button_robot_0->set_active(true);

    } else if(event->keyval == GDK_KEY_w){
        radio_button_robot_1->set_active(true);

    } else if(event->keyval == GDK_KEY_e){
        radio_button_robot_2->set_active(true);
        
    } else {
        game_model.manualRobotControl(STOPPED_MOVE, 0, 0);
    }

    return true;
}

bool GameView::onMouseClick(GdkEventButton* event){
    
    signal_press_event.emit(event);

    if(event->button == GDK_BUTTON_PRIMARY){
        if(switch_robot->get_active()){
            if(radio_button_robot_0->get_active()){
                game_model.setTargetOf(signal_draw_get_click.emit(), 0);

            } else if(radio_button_robot_1->get_active()){
                game_model.setTargetOf(signal_draw_get_click.emit(), 1);                

            } else if(radio_button_robot_2->get_active()){
                game_model.setTargetOf(signal_draw_get_click.emit(), 2);
                
            }
        }
    }
    return true;
}

bool GameView::deleteConnection(GdkEventAny* event){
    control_connection.disconnect();
    return false;
}

void GameView::onPotencyChangedRobot0(){
	game_model.setPotencyFactor(spin_potency_robot0->get_value(), 0);
}

void GameView::onPotencyChangedRobot1(){
	game_model.setPotencyFactor(spin_potency_robot1->get_value(), 1);
}

void GameView::onPotencyChangedRobot2(){
	game_model.setPotencyFactor(spin_potency_robot2->get_value(), 2);
}

void GameView::onCurveChangedRobot0(){
	game_model.setCurveFactor(spin_curve_robot0->get_value(), 0);
}

void GameView::onCurveChangedRobot1(){
    game_model.setCurveFactor(spin_curve_robot1->get_value(), 1);
}

void GameView::onCurveChangedRobot2(){
    game_model.setCurveFactor(spin_curve_robot2->get_value(), 2);
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
    game_model.changeGameSide();
}

void GameView::onButtonInformation() {
    popover_information->show_all();
    popover_information->set_visible(button_information->get_focus_on_click());
}

void GameView::onSwitchRobot() {
    if (!switch_robot->get_active()){
        radio_button_robot_0->set_state(Gtk::StateType::STATE_INSENSITIVE);
        radio_button_robot_1->set_state(Gtk::StateType::STATE_INSENSITIVE);
        radio_button_robot_2->set_state(Gtk::StateType::STATE_INSENSITIVE);
        game_model.setTargetFromScreen(false);
    } else {
        radio_button_robot_0->set_state(Gtk::StateType::STATE_NORMAL);
        radio_button_robot_1->set_state(Gtk::StateType::STATE_NORMAL);
        radio_button_robot_2->set_state(Gtk::StateType::STATE_NORMAL);
        game_model.setTargetFromScreen(true);
    }
}

void GameView::onRadioButton0() {
    
}

void GameView::onRadioButton1() {
    
}

void GameView::onRadioButton2() {
    
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