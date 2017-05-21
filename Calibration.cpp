#include "Calibration.h"

Calibration::Calibration(){
    colorsHSV.resize(6);
    colorsRGB.resize(6);

    selected_player = 0;
    end_calibration = false;
    cairo_binary_image = false;
    program_state = MENU;

    getCalibration();
    setImage();
}

void Calibration::thread(){
    while(!end_calibration){
        setImage();
        cv:: Mat opencv_image_BGR_rotated  = rotateImage(getOpencvImageBGR(), getAngleImage());
        setOpenCVImageBGRCuted  ( cutImage( opencv_image_BGR_rotated, getPointCutField1(), getPointCutField2()));
        setOpenCVImageHSV       ( changeColorSpace( getOpencvImageBGRCuted(), cv::COLOR_BGR2HSV_FULL));
        setOpenCVImageCairo     ( changeColorSpace( getOpencvImageBGRCuted(), cv::COLOR_BGR2RGB));
        setOpenCVImageBinary    ( changeColorSpace( binarize(getOpencvImageHSV(), colorsHSV[selected_player]), cv::COLOR_GRAY2RGB));
        timer.framesPerSecond();
    }
}

void Calibration::updateColorPixel(Point pixel_point){
    hsvPoint = getOpencvImageHSV().at<cv::Vec3b>(pixel_point.y, pixel_point.x);
    colorsHSV[selected_player].setH(hsvPoint[H]);
    colorsHSV[selected_player].setS(hsvPoint[S]);
    colorsHSV[selected_player].setV(hsvPoint[V]);

    rgbPoint = getOpencvImageBGRCuted().at<cv::Vec3b>(pixel_point.y, pixel_point.x);
    colorsRGB[selected_player].r = rgbPoint[2];
    colorsRGB[selected_player].g = rgbPoint[1];
    colorsRGB[selected_player].b = rgbPoint[0];

    setPopoverHSVDefault();
}

void Calibration::getCalibration(){
    manipulation.loadCalibration();
    colorsHSV           = manipulation.getColorsHsv();
    colorsRGB           = manipulation.getColorsRgb();
    camera_config       = manipulation.loadCameraConfig();
    camera_initialize   = manipulation.getCameraOn();
    image_initialize    = !manipulation.getCameraOn();

    setAngleImage(manipulation.getAngleImage());
    setGoal(manipulation.getGoal());
    setPointCutField1(manipulation.getPointField1());
    setPointCutField2(manipulation.getPointField2());
    
    setCameraOn(manipulation.getCameraOn());

    hsvPoint[0] = colorsHSV[selected_player].h[2];
    hsvPoint[1] = colorsHSV[selected_player].s[2];
    hsvPoint[2] = colorsHSV[selected_player].v[2];

  //  manipulation.showCalibration();

}

int Calibration::GUI(){
			
    app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("GUI/Glade/Calibration.glade");
	
	builder->get_widget("Window Calibration", window);
    window->signal_key_press_event().connect(sigc::mem_fun(this, &Calibration::onKeyboard));
	
    builder->get_widget("Menu Play", menu_play);
    menu_play->signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuGame));
	
    builder->get_widget("Menu Simulator", menu_simulator);
    menu_simulator->signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuSimulator));
	
    builder->get_widget("Menu Arduino", menu_arduino);
    menu_arduino->signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuArduino));
	
    builder->get_widget("Menu Quit", menu_quit);
    menu_quit->signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuQuit));

    builder->get_widget("Menu Save Calibration", menu_save_calibration);
    //menu_save_calibration->signal_activate().connect(sigc::mem_fun(this, ));

    builder->get_widget("Menu Cut Image", menu_cut_image);
    //menu_cut_image->signal_activate().connect(sigc::mem_fun(this, ));

    builder->get_widget("Menu Reset Values", menu_reset_values);
    //menu_reset_values->signal_activate().connect(sigc::mem_fun(this, ));

    builder->get_widget("Menu Device 0", menu_device0);
    builder->get_widget("Menu Device 1", menu_device1);
    updateDevices();

    builder->get_widget("Menu Load Camera Configuration", menu_load_camera_config);
    //menu_load_camera_config->signal_activate().connect(sigc::mem_fun(this, ));

    builder->get_widget("Menu Default Configuration", menu_refresh);
    menu_refresh->signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuRefresh));

    builder->get_widget("Radio Button Image", radio_button_image);    
    radio_button_image->signal_pressed().connect(sigc::mem_fun(this, &Calibration::onRadioButtonImage));
    
    builder->get_widget("Radio Button Camera", radio_button_camera);    
    radio_button_camera->signal_pressed().connect(sigc::mem_fun(this, &Calibration::onRadioButtonCamera));
    radio_button_camera->set_active(getCameraOn());
    
    builder->get_widget("Combo Box Text Calibration", combo_choose_player);    
    //combo_choose_player->signal_changed().connect(sigc::mem_fun(this, &Calibration::onChoosePlayer));

    builder->get_widget("Popover HSV", popover_hsv);

    builder->get_widget("Button HSV", button_hsv_popover);
    button_hsv_popover->signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonHSV) );
    
    builder->get_widget("Scale H max", scale_hmax);
    scale_hmax->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleHMax) );

    builder->get_widget("Scale H min", scale_hmin);    
    scale_hmin->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleHMin) );
    
    builder->get_widget("Scale S max", scale_smax);
    scale_smax->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleSMax) );

    builder->get_widget("Scale S min", scale_smin);
    scale_smin->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleSMin) );

    builder->get_widget("Scale V max", scale_vmax);
    scale_vmax->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleVMax) );

    builder->get_widget("Scale V min", scale_vmin);
    scale_vmin->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleVMin) );

    builder->get_widget("Popover CAM", popover_cam);

    builder->get_widget("Button CAM", button_cam_popover);
    button_cam_popover->signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonCAM) );
    
    builder->get_widget("Scale Brightness", scale_brightness);
    scale_brightness->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMBrightness) );

    builder->get_widget("Scale Contrast", scale_contrast);    
    scale_contrast->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMContrast) );
    
    builder->get_widget("Scale Saturation", scale_saturation);
    scale_saturation->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMSaturation) );

    builder->get_widget("Scale Gain", scale_gain);
    scale_gain->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMGain) );

    builder->get_widget("Scale Sharpness", scale_sharpness);
    scale_sharpness->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMSharpness) );

    builder->get_widget("Scale Exposure", scale_exposure);
    scale_exposure->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMExposure) );

    builder->get_widget("Scale Rotate", scale_rotate);
    scale_rotate->signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleRotate) );
    
    builder->get_widget("Label Fps", label_fps);

    builder->get_widget("Box Global", box_global);
    box_global->pack_start(draw_area);

    draw_area.signal_button_press_event().connect( sigc::mem_fun(this, &Calibration::onMouseClick) );
	sigc::connection draw_connection = Glib::signal_timeout().connect( sigc::mem_fun(this, &Calibration::setInformations50MilliSec) , 50 );

    window->maximize();
    window->show_all();
    
    std::thread calibration_thread(&Calibration::thread, this);

	app->run(*window);

// Finishing Calibration
    draw_connection.disconnect();
    
    setEndCalibration(true);
    calibration_thread.join();
    cameraRelease();

    manipulation.saveCalibration(colorsHSV, colorsRGB, getPointCutField1(), getPointCutField2(), getGoal(), getAngleImage(), getCameraOn());
    manipulation.saveCameraConfig(camera_config);
    
    return program_state;
}

void Calibration::updateDevices(){
    string device;
    string name_device;

    device = executeCommand("uvcdynctrl -l | grep video0");
    if(device.size() != 0){
        name_device = device.substr(device.find("video")+9);
        menu_device0->set_label(name_device);
    }

    device = executeCommand("uvcdynctrl -l | grep video1");
    if(device.size() != 0){
        name_device = device.substr(device.find("video")+9);
        menu_device1->set_label(name_device);
    }
}

bool Calibration::setInformations50MilliSec(){
    if (cairo_binary_image){
        draw_area.setImage(getOpencvImageBinary());
    } else {
        draw_area.setImage(getOpencvImageCairo());
    }

    string txt = "Fps: " ;//+ to_string(threadfps);
	label_fps->set_label(txt);

	return true;
}

bool Calibration::onMouseClick(GdkEventButton* event){
    if(event->button == GDK_BUTTON_PRIMARY) {
        updateColorPixel( changeCordinates({event->x, event->y}, draw_area.getCairoImageSize(), getOpencvImageBGRCuted().size()) );
    }
    if(event->button == GDK_BUTTON_SECONDARY) {
        draw_area.setRectanglePoint({event->x, event->y});
    } 
    return true;
}

bool Calibration::onKeyboard(GdkEventKey* event){
    if (event->keyval == GDK_KEY_F1) {
        cairo_binary_image = !cairo_binary_image;
    }
    if (event->keyval == GDK_KEY_C || event->keyval == GDK_KEY_c) {
        setPointCutField1(changeCordinates(draw_area.getPointCut1(), draw_area.getCairoImageSize(), getOpencvImageBGR().size()));
        setPointCutField2(changeCordinates(draw_area.getPointCut2(), draw_area.getCairoImageSize(), getOpencvImageBGR().size()));
        draw_area.setRectangleInvisible();
    }
    if (event->keyval == GDK_KEY_G || event->keyval == GDK_KEY_g) {
        Point p1 = changeCordinates(draw_area.getPointCut1(), draw_area.getCairoImageSize(), getOpencvImageBGR().size());
        Point p2 = changeCordinates(draw_area.getPointCut2(), draw_area.getCairoImageSize(), getOpencvImageBGR().size());
        setGoal({abs(p1.x - p2.x), abs(p1.y - p2.y)});
        draw_area.setRectangleInvisible();
    }
    if (event->keyval == GDK_KEY_X || event->keyval == GDK_KEY_x) {
        setPointCutField1({0,0});
        setPointCutField2(getOpencvImageBGR().size());
    }
    return true;
}

void Calibration::onMenuGame(){
    program_state = GAME; window->close();
}

void Calibration::onMenuSimulator(){
    program_state = SIMULATOR; window->close();
}

void Calibration::onMenuArduino(){
    program_state = ARDUINO; window->close();
}

void Calibration::onMenuQuit(){
    program_state = MENU; window->close();
}

void Calibration::onButtonHSV() {
    popover_hsv->show_all();
    popover_hsv->set_visible(button_hsv_popover->get_focus_on_click());
}

void Calibration::onButtonCAM() {
    popover_cam->show_all();
    popover_cam->set_visible(button_cam_popover->get_focus_on_click());
}

void Calibration::onChoosePlayer(){
    selected_player = combo_choose_player->get_active_row_number();
    /*
    scale_HSV_popover[0].set_value(colorsHSV[selected_player].variationH_MAX);
    scale_HSV_popover[1].set_value(colorsHSV[selected_player].variationH_MIN);
    scale_HSV_popover[2].set_value(colorsHSV[selected_player].variationS_MAX);
    scale_HSV_popover[3].set_value(colorsHSV[selected_player].variationS_MIN);
    scale_HSV_popover[4].set_value(colorsHSV[selected_player].variationV_MAX);
    scale_HSV_popover[5].set_value(colorsHSV[selected_player].variationV_MIN);
    */
}

void Calibration::onScaleHMax(){
    colorsHSV[selected_player].variationH_MAX = scale_hmax->get_value();
    colorsHSV[selected_player].setH(colorsHSV[selected_player].h[MID]);
}

void Calibration::onScaleHMin(){
    colorsHSV[selected_player].variationH_MIN = scale_hmin->get_value();   
    colorsHSV[selected_player].setH(colorsHSV[selected_player].h[MID]);
}

void Calibration::onScaleSMax(){
    colorsHSV[selected_player].variationS_MAX = scale_smax->get_value();
    colorsHSV[selected_player].setS(colorsHSV[selected_player].s[MID]);
}

void Calibration::onScaleSMin(){
    colorsHSV[selected_player].variationS_MIN = scale_smin->get_value();
    colorsHSV[selected_player].setS(colorsHSV[selected_player].s[MID]);
}

void Calibration::onScaleVMax(){
    colorsHSV[selected_player].variationV_MAX = scale_vmax->get_value();
    colorsHSV[selected_player].setV(colorsHSV[selected_player].v[MID]);
}

void Calibration::onScaleVMin(){
    colorsHSV[selected_player].variationV_MIN = scale_vmin->get_value();
    colorsHSV[selected_player].setV(colorsHSV[selected_player].v[MID]);
}

void Calibration::onScaleRotate(){
    setAngleImage(scale_rotate->get_value());
}

void Calibration::onScaleCAMBrightness(){
   camera_config.brightness = scale_brightness->get_value();
   updateCameraValues(camera_config, camera_number);
}

void Calibration::onScaleCAMContrast(){
    camera_config.contrast = scale_contrast->get_value();
    rodetas::updateCameraValues(camera_config, camera_number);
}

void Calibration::onScaleCAMSaturation(){
    camera_config.saturation = scale_saturation->get_value();
    rodetas::updateCameraValues(camera_config, camera_number);
}

void Calibration::onScaleCAMGain(){
    camera_config.gain = scale_gain->get_value();
    rodetas::updateCameraValues(camera_config, camera_number);
}

void Calibration::onScaleCAMSharpness(){
    camera_config.sharpness = scale_sharpness->get_value();
    rodetas::updateCameraValues(camera_config, camera_number);
}

void Calibration::onScaleCAMExposure(){
    camera_config.exposure = scale_exposure->get_value();
    rodetas::updateCameraValues(camera_config, camera_number);
}

void Calibration::onRadioButtonImage(){
    if (!radio_button_image->get_active()){
        button_cam_popover->set_state(Gtk::StateType::STATE_INSENSITIVE);
        setCameraOn(false);
        radio_button_image->set_active(true);
        image_initialize = true;
    }
}

void Calibration::onRadioButtonCamera(){
    if (!radio_button_camera->get_active()){    
        button_cam_popover->set_state(Gtk::StateType::STATE_NORMAL);
        setCameraOn(true);
        radio_button_camera->set_active(true);        
        camera_initialize = true;
    }
}

void Calibration::onMenuRefresh(){
    camera_config.brightness = 128;
    camera_config.contrast = 128;
    camera_config.saturation = 128;
    camera_config.gain = 0;
    camera_config.sharpness = 128;
    camera_config.exposure = 300;

    setPopoverCamValues();
    updateCameraValues(camera_config, camera_number);
}

void Calibration::setPopoverHSVDefault(){
    scale_hmax->set_value(50);
	scale_hmin->set_value(50);
	scale_smax->set_value(50);
	scale_smin->set_value(50);
	scale_vmax->set_value(50);
	scale_vmin->set_value(50);
}

void Calibration::setPopoverCamValues(){
    scale_brightness->set_value(camera_config.brightness);
    scale_contrast->set_value(camera_config.contrast);
    scale_saturation->set_value(camera_config.saturation);
    scale_gain->set_value(camera_config.gain); 
    scale_sharpness->set_value(camera_config.sharpness);
    scale_exposure->set_value(camera_config.exposure);
}

void Calibration::setEndCalibration(bool b){
    std::lock_guard<std::mutex> lock(mutex);
    end_calibration = b;
}

bool Calibration::getEndCalibration(){
    std::lock_guard<std::mutex> lock(mutex);
    return end_calibration;
}

void Calibration::setPointCutField1(Point p){
    std::lock_guard<std::mutex> lock(mutex);    
    point_cut_field_1 = p;
}

Point Calibration::getPointCutField1(){
    std::lock_guard<std::mutex> lock(mutex);    
    return point_cut_field_1;
}

void Calibration::setPointCutField2(Point p){
    std::lock_guard<std::mutex> lock(mutex);    
    point_cut_field_2 = p;
}

Point Calibration::getPointCutField2(){
    std::lock_guard<std::mutex> lock(mutex);    
    return point_cut_field_2;
}

void Calibration::setGoal(Point g){
    std::lock_guard<std::mutex> lock(mutex);    
    goal = g;
}

Point Calibration::getGoal(){
    std::lock_guard<std::mutex> lock(mutex);    
    return goal;
}

void Calibration::setAngleImage(int a){
    std::lock_guard<std::mutex> lock(mutex);    
    angle_image = a;
}

int Calibration::getAngleImage(){
    std::lock_guard<std::mutex> lock(mutex);    
    return angle_image;
}

void Calibration::setCameraOn(bool b){
    std::lock_guard<std::mutex> lock(mutex);        
    camera_on = b;
}

bool Calibration::getCameraOn(){
    std::lock_guard<std::mutex> lock(mutex);        
    return camera_on;
}

void Calibration::setOpenCVImageBGR(cv::Mat i){
    std::lock_guard<std::mutex> lock(mutex);    
    opencv_image_BGR = i;
}

cv::Mat Calibration::getOpencvImageBGR(){
    std::lock_guard<std::mutex> lock(mutex);    
    return opencv_image_BGR;
}

void Calibration::setOpenCVImageBGRCuted(cv::Mat i){
    std::lock_guard<std::mutex> lock(mutex);    
    opencv_image_BGR_cuted = i;
}

cv::Mat Calibration::getOpencvImageBGRCuted(){
    std::lock_guard<std::mutex> lock(mutex);    
    return opencv_image_BGR_cuted;
}

void Calibration::setOpenCVImageHSV(cv::Mat i){
    std::lock_guard<std::mutex> lock(mutex);    
    opencv_image_HSV = i;
}

cv::Mat Calibration::getOpencvImageHSV(){
    std::lock_guard<std::mutex> lock(mutex);    
    return opencv_image_HSV;
}

void Calibration::setOpenCVImageCairo(cv::Mat i){
    std::lock_guard<std::mutex> lock(mutex);    
    opencv_image_cairo = i;
}

cv::Mat Calibration::getOpencvImageCairo(){
    std::lock_guard<std::mutex> lock(mutex);    
    return opencv_image_cairo;
}

void Calibration::setOpenCVImageBinary(cv::Mat i){
    std::lock_guard<std::mutex> lock(mutex);    
    opencv_image_binary = i;
}

cv::Mat Calibration::getOpencvImageBinary(){
    std::lock_guard<std::mutex> lock(mutex);    
    return opencv_image_binary;
}