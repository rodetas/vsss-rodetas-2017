#include "CalibrationView.h"

CalibrationView::CalibrationView(){
    calibration_model.setCaller(this);
    program_state = MENU;
    cairo_binary_image = false;
}

CalibrationView::~CalibrationView(){
}

int CalibrationView::GUI(){
			
    app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("calibration/Calibration.glade");
	
	builder->get_widget("Window Calibration", window);
    //window->signal_key_press_event().connect(sigc::mem_fun(this, &Calibration::onKeyboard));
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
    //menu_cut_image->signal_activate().connect(sigc::mem_fun(this, &Calibration::onCutImage));

    builder->get_widget("Menu Reset Values", menu_reset_values);
    //menu_reset_values->signal_activate().connect(sigc::mem_fun(this, ));

    builder->get_widget("Menu Device 0", menu_device0);
    builder->get_widget("Menu Device 1", menu_device1);
    //updateDevices();

    builder->get_widget("Menu Load Camera Configuration", menu_load_camera_config);
    //menu_load_camera_config->signal_activate().connect(sigc::mem_fun(this, ));

    builder->get_widget("Menu Default Configuration", menu_refresh);
    //menu_refresh->signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuRefresh));

    builder->get_widget("Radio Button Image", radio_button_image);    
    //radio_button_image->signal_pressed().connect(sigc::mem_fun(this, &Calibration::onRadioButtonImage));
    
    builder->get_widget("Radio Button Camera", radio_button_camera);    
    //radio_button_camera->signal_pressed().connect(sigc::mem_fun(this, &Calibration::onRadioButtonCamera));
    //radio_button_camera->set_active(getCameraOn());

    builder->get_widget("Popover HSV", popover_hsv);

    builder->get_widget("Button HSV", button_hsv_popover);
    //button_hsv_popover->signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonHSV) );
    
    builder->get_widget("Scale H max", scale_hmax);
    //scale_hmax->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleHMax) );

    builder->get_widget("Scale H min", scale_hmin);    
    //scale_hmin->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleHMin) );
    
    builder->get_widget("Scale S max", scale_smax);
    //scale_smax->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleSMax) );

    builder->get_widget("Scale S min", scale_smin);
    //scale_smin->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleSMin) );

    builder->get_widget("Scale V max", scale_vmax);
    //scale_vmax->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleVMax) );

    builder->get_widget("Scale V min", scale_vmin);
    //scale_vmin->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleVMin) );

    builder->get_widget("Combo Box Text Calibration", combo_choose_player);    
    //combo_choose_player->signal_changed().connect(sigc::mem_fun(this, &Calibration::onChoosePlayer));
    //onChoosePlayer();

    builder->get_widget("Popover CAM", popover_cam);

    builder->get_widget("Button CAM", button_cam_popover);
    //button_cam_popover->signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonCAM) );
    //if (!getCameraOn()) button_cam_popover->set_state(Gtk::StateType::STATE_INSENSITIVE);    
    
    builder->get_widget("Scale Brightness", scale_brightness);
    //scale_brightness->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMBrightness) );

    builder->get_widget("Scale Contrast", scale_contrast);    
    //scale_contrast->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMContrast) );
    
    builder->get_widget("Scale Saturation", scale_saturation);
    //scale_saturation->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMSaturation) );

    builder->get_widget("Scale Gain", scale_gain);
    //scale_gain->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMGain) );

    builder->get_widget("Scale Sharpness", scale_sharpness);
    //scale_sharpness->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMSharpness) );

    builder->get_widget("Scale Exposure", scale_exposure);
    //scale_exposure->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMExposure) );
    //setPopoverCamValues();
    
    builder->get_widget("Scale Rotate", scale_rotate);
    //scale_rotate->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleRotate) );
    
    builder->get_widget("Label Fps", label_fps);

    builder->get_widget("Box Global", box_global);
    box_global->pack_start(draw_area);

    //draw_area.signal_button_press_event().connect( sigc::mem_fun(this, &Calibration::onMouseClick) );

    window->show_all();

    
    screen_connection.connect(sigc::mem_fun(this, &CalibrationView::updateScreen));
    hsv_default_connection.connect(sigc::mem_fun(*this, &CalibrationView::setPopoverHSVDefault));

    sigc::connection update_image_connection = Glib::signal_timeout().connect(sigc::mem_fun(calibration_model, &CalibrationModel::updateFrame), 33); 

	app->run(*window);

    update_image_connection.disconnect();
    
    /*
    cameraRelease();
    onSaveCalibration();
    manipulation.saveCameraConfig(getCameraConfig());
    */

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

bool CalibrationView::onMouseClick(GdkEventButton* event){
    if(event->button == GDK_BUTTON_PRIMARY) {
        calibration_model.updateColorPixel({event->x, event->y}, draw_area.getCairoImageSize());
    }
    if(event->button == GDK_BUTTON_SECONDARY) {
        draw_area.setRectanglePoint({event->x, event->y});
    } 
    return true;
}

void CalibrationView::setPopoverHSVDefault(){
    scale_hmax->set_value(50);
	scale_hmin->set_value(50);
	scale_smax->set_value(50);
	scale_smin->set_value(50);
	scale_vmax->set_value(50);
	scale_vmin->set_value(50);
}

void CalibrationView::notify(string s){
    if (s == "setPopoverHSVDefault")
        hsv_default_connection.emit();

    if (s == "updateScreen"){
        screen_connection.emit();
    }
}