#include "CalibrationView.h"

CalibrationView::CalibrationView(){
    calibration_model.setCaller(this);
    program_state = MENU;
    cairo_binary_image = false;
}

CalibrationView::~CalibrationView(){
    calibration_model.saveParameters();
    calibration_model.cameraRelease();
    update_image_connection.disconnect();   
}

int CalibrationView::GUI(){
			
    app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("calibration/Calibration.glade");
	
	builder->get_widget("Window Calibration", window);
    window->signal_key_press_event().connect(sigc::mem_fun(this, &CalibrationView::onKeyboard));
	window->maximize();
    
    builder->get_widget("Menu Play", menu_play);
    menu_play->signal_activate().connect(sigc::mem_fun(this, &CalibrationView::onMenuGame));
	
    builder->get_widget("Menu Simulator", menu_simulator);
    menu_simulator->signal_activate().connect(sigc::mem_fun(this, &CalibrationView::onMenuSimulator));
	
    builder->get_widget("Menu Arduino", menu_arduino);
    menu_arduino->signal_activate().connect(sigc::mem_fun(this, &CalibrationView::onMenuArduino));
	
    builder->get_widget("Menu Quit", menu_quit);
    menu_quit->signal_activate().connect(sigc::mem_fun(this, &CalibrationView::onMenuQuit));

    builder->get_widget("Menu Save Calibration", menu_save_calibration);
    menu_save_calibration->signal_activate().connect(sigc::mem_fun(this, &CalibrationView::onSaveCalibration));

    builder->get_widget("Menu Cut Image", menu_cut_image);
    menu_cut_image->signal_activate().connect(sigc::mem_fun(this, &CalibrationView::onCutImage));

    builder->get_widget("Menu Reset Values", menu_reset_values);
    //menu_reset_values->signal_activate().connect(sigc::mem_fun(this, ));

    builder->get_widget("Menu Device 0", menu_device0);
    builder->get_widget("Menu Device 1", menu_device1);
    updateMenuDevice();

    builder->get_widget("Menu Load Camera Configuration", menu_load_camera_config);
    //menu_load_camera_config->signal_activate().connect(sigc::mem_fun(this, ));

    builder->get_widget("Menu Default Configuration", menu_default);
    menu_default->signal_activate().connect(sigc::mem_fun(this, &CalibrationView::onMenuDefault));

    builder->get_widget("Radio Button Image", radio_button_image);    
    radio_button_image->signal_pressed().connect(sigc::mem_fun(this, &CalibrationView::onRadioButtonImage));
    
    builder->get_widget("Radio Button Camera", radio_button_camera);    
    radio_button_camera->signal_pressed().connect(sigc::mem_fun(this, &CalibrationView::onRadioButtonCamera));
    radio_button_camera->set_active(calibration_model.getCameraOn());

    builder->get_widget("Button Cut", button_cut_mode);
    button_cut_mode->signal_clicked().connect( sigc::mem_fun(this, &CalibrationView::onButtonCutMode) );

    builder->get_widget("Popover HSV", popover_hsv);

    builder->get_widget("Button HSV", button_hsv_popover);
    button_hsv_popover->signal_clicked().connect( sigc::mem_fun(this, &CalibrationView::onButtonHSV) );
    
    builder->get_widget("Scale H max", scale_hmax);
    scale_hmax->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleHMax) );

    builder->get_widget("Scale H min", scale_hmin);    
    scale_hmin->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleHMin) );
    
    builder->get_widget("Scale S max", scale_smax);
    scale_smax->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleSMax) );

    builder->get_widget("Scale S min", scale_smin);
    scale_smin->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleSMin) );

    builder->get_widget("Scale V max", scale_vmax);
    scale_vmax->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleVMax) );

    builder->get_widget("Scale V min", scale_vmin);
    scale_vmin->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleVMin) );

    builder->get_widget("Combo Box Text Calibration", combo_choose_player);    
    combo_choose_player->signal_changed().connect(sigc::mem_fun(this, &CalibrationView::onChoosePlayer));

    builder->get_widget("Popover CAM", popover_cam);

    builder->get_widget("Button CAM", button_cam_popover);
    button_cam_popover->signal_clicked().connect( sigc::mem_fun(this, &CalibrationView::onButtonCAM) );
    
    builder->get_widget("Scale Brightness", scale_brightness);
    scale_brightness->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleCAMBrightness) );

    builder->get_widget("Scale Contrast", scale_contrast);    
    scale_contrast->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleCAMContrast) );
    
    builder->get_widget("Scale Saturation", scale_saturation);
    scale_saturation->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleCAMSaturation) );

    builder->get_widget("Scale Gain", scale_gain);
    scale_gain->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleCAMGain) );

    builder->get_widget("Scale Sharpness", scale_sharpness);
    scale_sharpness->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleCAMSharpness) );

    builder->get_widget("Scale Exposure", scale_exposure);
    scale_exposure->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleCAMExposure) );
    
    builder->get_widget("Scale Rotate", scale_rotate);
    scale_rotate->signal_value_changed().connect( sigc::mem_fun(this, &CalibrationView::onScaleRotate) );
    
    builder->get_widget("Label Fps", label_fps);

    builder->get_widget("Box Global", box_global);
    box_global->pack_start(draw_area);

    window->show_all();
    
    draw_area.signal_button_press_event().connect( sigc::mem_fun(this, &CalibrationView::onMouseClick) );
    update_image_connection = Glib::signal_timeout().connect(sigc::mem_fun(calibration_model, &CalibrationModel::updateFrame), 33, Glib::PRIORITY_DEFAULT_IDLE); 

    /* Interface Initialize - Sliders and Buttons */
        setScaleValueHSV(calibration_model.getColorHsv());

        if (!calibration_model.getCameraOn()){
            button_cam_popover->set_state(Gtk::StateType::STATE_INSENSITIVE);
        }
    /* ------------------------------------------ */
    
	app->run(*window);

    return program_state;
}

void CalibrationView::updateScreen(){
    if (cairo_binary_image){
        draw_area.setImage(calibration_model.getScreenBinaryImage());
    } else {
        draw_area.setImage(calibration_model.getScreenImage());
    }
    
    string s = "Fps: " + to_string(calibration_model.getFps());
	label_fps->set_label(s);
}

void CalibrationView::updateMenuDevice(){
    menu_device0->set_label(calibration_model.getDeviceName(0));
    menu_device1->set_label(calibration_model.getDeviceName(1));
}

void CalibrationView::notify(string s){
    if (s == "setScaleValueHSV") {
        Hsv c = calibration_model.getColorHsv();        
        setScaleValueHSV(c);
    }

    if (s == "setScaleValueHSVDefault"){
        Hsv c; c.defaultValue();
        setScaleValueHSV(c);
    }

    if (s == "updateScreen") {
        updateScreen();
    }
}

bool CalibrationView::onKeyboard(GdkEventKey* event){
    if (event->keyval == GDK_KEY_F1) {
        cairo_binary_image = !cairo_binary_image;
    }
    if (event->keyval == GDK_KEY_C || event->keyval == GDK_KEY_c) {
        onCutImage();
    }
    if (event->keyval == GDK_KEY_G || event->keyval == GDK_KEY_g) {
        onCutGoal();        
    }
    if (event->keyval == GDK_KEY_X || event->keyval == GDK_KEY_x) {
        PointCut cut;
            cut.first = {0,0};
            cut.second = draw_area.getCairoImageSize();
        calibration_model.setCutPoint(cut, draw_area.getCairoImageSize());
    }
    return true;    
}

bool CalibrationView::onMouseClick(GdkEventButton* event){
    if(event->button == GDK_BUTTON_PRIMARY) {
        calibration_model.updateColorPixel({event->x, event->y}, draw_area.getCairoImageSize());
    }
    return true;
}

void CalibrationView::onButtonCutMode(){
    draw_area.setCutMode(button_cut_mode->get_active());
}

void CalibrationView::onCutImage(){
    calibration_model.setCutPoint(draw_area.getCutPoint(), draw_area.getCairoImageSize());
    draw_area.setRectangleInvisible();
}

void CalibrationView::onCutGoal(){
    calibration_model.setCutGoal(draw_area.getCutPoint(), draw_area.getCairoImageSize());
    draw_area.setRectangleInvisible();
}

void CalibrationView::onScaleHMax(){
    calibration_model.setScaleHSV("hmax", scale_hmax->get_value());
}

void CalibrationView::onScaleHMin(){
    calibration_model.setScaleHSV("hmin", scale_hmin->get_value());
}

void CalibrationView::onScaleSMax(){
    calibration_model.setScaleHSV("smax", scale_smax->get_value());
}

void CalibrationView::onScaleSMin(){
    calibration_model.setScaleHSV("smin", scale_smin->get_value());
}

void CalibrationView::onScaleVMax(){
    calibration_model.setScaleHSV("vmax", scale_vmax->get_value());
}

void CalibrationView::onScaleVMin(){
    calibration_model.setScaleHSV("vmin", scale_vmin->get_value());
}

void CalibrationView::onScaleCAMBrightness(){
    calibration_model.setScaleCam("brightness", scale_brightness->get_value());
}

void CalibrationView::onScaleCAMContrast(){
    calibration_model.setScaleCam("contrast", scale_contrast->get_value());
}

void CalibrationView::onScaleCAMSaturation(){
    calibration_model.setScaleCam("saturation", scale_saturation->get_value());
}

void CalibrationView::onScaleCAMGain(){
    calibration_model.setScaleCam("gain", scale_gain->get_value());
}

void CalibrationView::onScaleCAMSharpness(){
    calibration_model.setScaleCam("sharpness", scale_sharpness->get_value());
}

void CalibrationView::onScaleCAMExposure(){
    calibration_model.setScaleCam("exposure", scale_exposure->get_value());
}

void CalibrationView::onButtonHSV() {
    popover_hsv->show_all();
    popover_hsv->set_visible(button_hsv_popover->get_focus_on_click());
}

void CalibrationView::onButtonCAM() {
    popover_cam->show_all();
    popover_cam->set_visible(button_cam_popover->get_focus_on_click());
}

void CalibrationView::onRadioButtonImage(){
    if (!radio_button_image->get_active()){
        button_cam_popover->set_state(Gtk::StateType::STATE_INSENSITIVE);
        calibration_model.setCameraOn(false);               
    }
}

void CalibrationView::onRadioButtonCamera(){
    if (!radio_button_camera->get_active()){    
        button_cam_popover->set_state(Gtk::StateType::STATE_NORMAL);
        calibration_model.setCameraOn(true);
    }
}

void CalibrationView::onChoosePlayer(){
    calibration_model.setSelectedPlayer(combo_choose_player->get_active_row_number());
}

void CalibrationView::onScaleRotate(){
    calibration_model.setAngleImage(scale_rotate->get_value());
}

void CalibrationView::onMenuDefault(){    
    CameraConfiguration c = calibration_model.getDefaultCameraValues();

    scale_brightness->set_value(c.brightness);
    scale_contrast->set_value(c.contrast);
    scale_saturation->set_value(c.saturation);
    scale_gain->set_value(c.gain); 
    scale_sharpness->set_value(c.sharpness);
    scale_exposure->set_value(c.exposure);
}

void CalibrationView::setScaleValueHSV(Hsv c){
    scale_hmax->set_value(c.variationH_MAX);
    scale_hmin->set_value(c.variationH_MIN);
    scale_smax->set_value(c.variationS_MAX);
    scale_smin->set_value(c.variationS_MIN);
    scale_vmax->set_value(c.variationV_MAX);
    scale_vmin->set_value(c.variationV_MIN);
}

void CalibrationView::onMenuGame(){
    program_state = GAME; window->close();
}

void CalibrationView::onMenuSimulator(){
    program_state = SIMULATOR; window->close();
}

void CalibrationView::onMenuArduino(){
    program_state = ARDUINO; window->close();
}

void CalibrationView::onMenuQuit(){
    program_state = MENU; window->close();
}

void CalibrationView::onSaveCalibration(){
    calibration_model.saveParameters();
}