#include "Calibration.h"

Calibration::Calibration(){
    colorsHSV.resize(6);
    colorsRGB.resize(6);

    selected_player = 0;
    end_calibration = false;
    cairo_binary_image = false;
    program_state = MENU;    

    camera = getCameraNumber();

    getCalibration();    
}

int Calibration::calibrate(){

    imageInitialize();

    std::thread calibration_thread(bind(&Calibration::GUI, this));

    while(!end_calibration){
        imageWebCam();
        opencv_image_BGR_cuted  = opencvTransformation(opencv_image_BGR, angle_image, pointCutField1, pointCutField2);
        opencv_image_HSV        = opencvColorSpace(opencv_image_BGR_cuted, cv::COLOR_BGR2HSV_FULL);
        opencv_image_cairo      = opencvColorSpace(opencv_image_BGR_cuted, cv::COLOR_BGR2RGB);
        opencv_image_binary     = opencvColorSpace( opencvBinary(colorsHSV[selected_player], opencv_image_HSV), cv::COLOR_GRAY2RGB);
    }

    manipulation.saveCalibration(colorsHSV, colorsRGB, pointCutField1, pointCutField2, goal, angle_image, camera_on);
    cam.release();

    calibration_thread.detach();
    return program_state;
}

void Calibration::updateColorPixel(Point pixel_point){
    hsvPoint = opencv_image_HSV.at<cv::Vec3b>(pixel_point.y, pixel_point.x);
    colorsHSV[selected_player].setH(hsvPoint[H]);
    colorsHSV[selected_player].setS(hsvPoint[S]);
    colorsHSV[selected_player].setV(hsvPoint[V]);

    rgbPoint = opencv_image_BGR_cuted.at<cv::Vec3b>(pixel_point.y, pixel_point.x);
    colorsRGB[selected_player].r = rgbPoint[2];
    colorsRGB[selected_player].g = rgbPoint[1];
    colorsRGB[selected_player].b = rgbPoint[0];
}

void Calibration::imageInitialize(){

    if(camera_on){
        cam = cv::VideoCapture(camera);
        usleep(10000); //time to camera answer
        if(cam.isOpened()){
            cam >> opencv_image_BGR;
        } else {
            cout << "Conection with camera failed" << endl;
        }


        initCameraConfig();
    } else {
        if(cam.isOpened()){ 
            cam.release();
        }

        opencv_image_BGR = cv::imread(imagePath);

        if(opencv_image_BGR.empty()){
            cout << "Problem to load image from computer" << endl;
        }
    }
}

void Calibration::imageWebCam(){
    if(camera_on && cam.isOpened()){
        cam >> opencv_image_BGR;
    }
}

void Calibration::getCalibration(){
    manipulation.loadCalibration();
    colorsHSV = manipulation.getColorsHsv();
    colorsRGB = manipulation.getColorsRgb();

    pointCutField1 = manipulation.getPointField1();
    pointCutField2 = manipulation.getPointField2();
    goal = manipulation.getGoal();
    angle_image = manipulation.getAngleImage();
    camera_on = manipulation.getCameraOn();

    hsvPoint[0] = colorsHSV[selected_player].h[2];
    hsvPoint[1] = colorsHSV[selected_player].s[2];
    hsvPoint[2] = colorsHSV[selected_player].v[2];

  //  manipulation.showCalibration();

}

void Calibration::GUI(){
			
	app = Gtk::Application::create();

    Glib::RefPtr<Gtk::AccelGroup> accel_map = Gtk::AccelGroup::create();

    Gtk::Window window;	
        window.set_title("Calibration");
        window.set_icon_from_file("files/images/logo-rodetas.png");
        window.maximize();
        window.add_accel_group(accel_map);
        window.signal_key_press_event().connect(sigc::mem_fun(this, &Calibration::onKeyboard));


///////////////////////// NAVEGATION MENU /////////////////////////

    Gtk::MenuItem menu_play;
        menu_play.set_label("_Start Game");
        menu_play.set_use_underline(true);
        menu_play.add_accelerator("activate", accel_map, GDK_KEY_n, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE);
        menu_play.show();
        menu_play.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuGame));

    Gtk::MenuItem menu_calibration;
        menu_calibration.set_label("_Calibrate");
        menu_calibration.set_use_underline(true);
        menu_calibration.add_accelerator("activate", accel_map, GDK_KEY_c, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
        menu_calibration.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuCalibration));

    Gtk::MenuItem menu_simulator;
        menu_simulator.set_label("S_imulate");
        menu_simulator.set_use_underline(true);
        menu_simulator.add_accelerator("activate", accel_map, GDK_KEY_s, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
        menu_simulator.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuSimulator));

    Gtk::MenuItem menu_arduino;
        menu_arduino.set_label("_Upload Arduino");
        menu_arduino.set_use_underline(true);
        menu_arduino.add_accelerator("activate", accel_map, GDK_KEY_u, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
        menu_arduino.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuArduino));

    Gtk::MenuItem menu_quit;
        menu_quit.set_label("_Quit");
        menu_quit.set_use_underline(true);
        menu_quit.add_accelerator("activate", accel_map, GDK_KEY_Escape, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE);
        menu_quit.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuQuit));

    Gtk::SeparatorMenuItem separator;    

    Gtk::MenuItem menu_navegation;
    Gtk::Menu subMenuNavigation;    
        menu_navegation.set_label("Navegation");
        menu_navegation.set_submenu(subMenuNavigation);
        subMenuNavigation.append(menu_play);
        subMenuNavigation.append(menu_calibration);
        subMenuNavigation.append(menu_simulator);
        subMenuNavigation.append(menu_arduino);
        subMenuNavigation.append(separator);
        subMenuNavigation.append(menu_quit);
   

///////////////////////// FILE MENU /////////////////////////

    Gtk::ImageMenuItem menu_save(Gtk::Stock::SAVE);
        menu_save.set_state(Gtk::StateType::STATE_INSENSITIVE);
        menu_save.set_label("Save Calibration");

    Gtk::ImageMenuItem menu_cut(Gtk::Stock::CUT);
        menu_cut.set_state(Gtk::StateType::STATE_INSENSITIVE);
        menu_cut.set_label("Cut Image");

    Gtk::ImageMenuItem menu_reset(Gtk::Stock::CLEAR);
        menu_reset.set_state(Gtk::StateType::STATE_INSENSITIVE);
        menu_reset.set_label("Reset Values");

    Gtk::MenuItem menu_file;
    Gtk::Menu subMenuFile;
        menu_file.set_label("Calibration");
        menu_file.set_submenu(subMenuFile);
        subMenuFile.append(menu_save);
        subMenuFile.append(menu_cut);
        subMenuFile.append(menu_reset);


///////////////////////// CAMERA MENU /////////////////////////

    Gtk::ImageMenuItem menu_load_camera_config(Gtk::Stock::OPEN);
        menu_load_camera_config.set_label("Load Camera Configuration");
        menu_load_camera_config.set_state(Gtk::StateType::STATE_INSENSITIVE);

    Gtk::MenuItem menu_refresh;
        menu_refresh.set_label("Refresh Device");
        menu_refresh.set_state(Gtk::StateType::STATE_INSENSITIVE);
        menu_refresh.add_accelerator("activate", accel_map, GDK_KEY_F5, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE); //116 -> f5
    
    Gtk::MenuItem menu_camera;    
    Gtk::Menu subMenuCamera;
        menu_camera.set_label("Camera");
        menu_camera.set_submenu(subMenuCamera);
        updateDevices();
        subMenuCamera.append(menu_device0);
        subMenuCamera.append(menu_device1);
        subMenuCamera.append(menu_load_camera_config);
        subMenuCamera.append(menu_refresh);


///////////////////////// MENU BAR /////////////////////////

    Gtk::MenuBar menu_bar;
        menu_bar.append(menu_navegation);
        menu_bar.append(menu_file);
        menu_bar.append(menu_camera);


///////////////////////// RADIO BUTTON - SET DEVICE /////////////////////////
    
    // initialized on .h
    radio_button_image.set_label("Image");
    radio_button_camera.set_label("Camera");

    Gtk::RadioButton::Group group;
        radio_button_image.signal_pressed().connect(sigc::mem_fun(this, &Calibration::onRadioButtonImage));
        radio_button_camera.signal_pressed().connect(sigc::mem_fun(this, &Calibration::onRadioButtonCamera));

        radio_button_image.set_group(group);
        radio_button_camera.set_group(group);
        
        radio_button_camera.set_active(camera_on);

    Gtk::Grid grid_radio_button;
        grid_radio_button.set_column_homogeneous(true);
        grid_radio_button.attach(radio_button_image, 0, 0, 1, 1);
        grid_radio_button.attach(radio_button_camera, 1, 0, 1, 1);


///////////////////////// COMBO BOX - SELECT Player /////////////////////////
        
        // initialized on .h
        combo_choose_player.set_size_request(200, -1);
        combo_choose_player.append("Player 0");
        combo_choose_player.append("Player 1");
        combo_choose_player.append("Player 2");
        combo_choose_player.append("Team");
        combo_choose_player.append("Opponent");
        combo_choose_player.append("Ball");
        combo_choose_player.set_active_text("Player 0");
        combo_choose_player.signal_changed().connect(sigc::mem_fun(this, &Calibration::onChoosePlayer));
        combo_choose_player.add_accelerator("popup", accel_map, 32, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE); // 32 -> Space bar


///////////////////////// BOX - SELECT HSV /////////////////////////

    vector<Gtk::Label> text_HSV_popover(6);
        for (int i = 0; i < text_HSV_popover.size(); i++){
            text_HSV_popover[i].set_alignment(Gtk::ALIGN_START);
        }
        text_HSV_popover[0].set_label("H max:");
        text_HSV_popover[1].set_label("H min:");
        text_HSV_popover[2].set_label("S max:");
        text_HSV_popover[3].set_label("S min:");
        text_HSV_popover[4].set_label("V max:");
        text_HSV_popover[5].set_label("V min:");
    
    // initialized on .h
    scale_HSV_popover.resize(6);
        for (int i = 0; i < scale_HSV_popover.size(); i++){
            scale_HSV_popover[i].set_size_request(150,20);
            scale_HSV_popover[i].set_draw_value(false);
            scale_HSV_popover[i].set_range(0,100);
            scale_HSV_popover[i].set_value(50);
        }
        scale_HSV_popover[0].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleHMax) );        
        scale_HSV_popover[1].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleHMin) );        
        scale_HSV_popover[2].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleSMax) );        
        scale_HSV_popover[3].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleSMin) );        
        scale_HSV_popover[4].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleVMax) );        
        scale_HSV_popover[5].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleVMin) );         

    Gtk::Grid grid_HSV_popover;
        for (int i = 0; i < scale_HSV_popover.size(); i++){
            grid_HSV_popover.attach(text_HSV_popover[i], 0, i, 1, 1);
            grid_HSV_popover.attach(scale_HSV_popover[i], 1, i, 2, 1);
        }
    
    Gtk::Box box_HSV_popover;
        box_HSV_popover.set_border_width(20);
        box_HSV_popover.pack_start(grid_HSV_popover);

    HSV_popover.set_relative_to(button_HSV_popover);
    HSV_popover.add(box_HSV_popover);

    // initialized on .h
    button_HSV_popover.add_label("HSV Controls");
    button_HSV_popover.signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonHSV) );


///////////////////////// BOX - CONFIG CAMERA /////////////////////////

    vector<Gtk::Label> text_CAM_popover(6);
        for (int i = 0; i < text_CAM_popover.size(); i++){
            text_CAM_popover[i].set_alignment(Gtk::ALIGN_START);
        }
        text_CAM_popover[0].set_label("Brightness:");
        text_CAM_popover[1].set_label("Contrast:");
        text_CAM_popover[2].set_label("Saturation:");
        text_CAM_popover[3].set_label("Gain:");
        text_CAM_popover[4].set_label("Sharpness:");
        text_CAM_popover[5].set_label("Exposure:");   
    
    // initialized on .h
    scale_CAM_popover.resize(6);
        for (int i = 0; i < scale_CAM_popover.size(); i++){
            scale_CAM_popover[i].set_size_request(150,20);
            scale_CAM_popover[i].set_draw_value(false);
            scale_CAM_popover[i].set_range(0,100);
        }

        scale_CAM_popover[0].set_value(camera_config.brightness);
        scale_CAM_popover[0].set_range(0,255);
        scale_CAM_popover[1].set_value(camera_config.contrast);
        scale_CAM_popover[1].set_range(0,255);
        scale_CAM_popover[2].set_value(camera_config.saturation);
        scale_CAM_popover[2].set_range(0,255);
        scale_CAM_popover[3].set_value(camera_config.gain);
        scale_CAM_popover[3].set_range(0,255);        
        scale_CAM_popover[4].set_value(camera_config.sharpness);
        scale_CAM_popover[4].set_range(0,255);
        scale_CAM_popover[5].set_value(camera_config.exposure);
        scale_CAM_popover[5].set_range(3,2047);

        scale_CAM_popover[0].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMBrightness) );        
        scale_CAM_popover[1].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMContrast) );        
        scale_CAM_popover[2].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMSaturation) );        
        scale_CAM_popover[3].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMGain) );        
        scale_CAM_popover[4].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMSharpness) );        
        scale_CAM_popover[5].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAMExposure) );         

    Gtk::Grid grid_CAM_popover;
        for (int i = 0; i < scale_CAM_popover.size(); i++){
            grid_CAM_popover.attach(text_CAM_popover[i], 0, i, 1, 1);
            grid_CAM_popover.attach(scale_CAM_popover[i], 1, i, 2, 1);
        }
    
    Gtk::Box box_CAM_popover;
        box_CAM_popover.set_border_width(20);
        box_CAM_popover.pack_start(grid_CAM_popover);

    CAM_popover.set_relative_to(button_CAM_popover);
    CAM_popover.add(box_CAM_popover);

    // initialized on .h
    button_CAM_popover.add_label("CAM Controls");
    button_CAM_popover.signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonCAM) );

    if(radio_button_image.get_active()){
        button_CAM_popover.set_state(Gtk::StateType::STATE_INSENSITIVE);
    }


///////////////////////// SCALE ROTATE IMAGE /////////////////////////

    Gtk::Label text_rotate("Rotate");
        scale_rotate.set_adjustment(Gtk::Adjustment::create(1.0, 0.0, 360.0, 0.5, 1.0, 2.0));
        scale_rotate.set_size_request(150,20);
        scale_rotate.set_value(180);
        scale_rotate.signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleRotate) );
        scale_rotate.set_draw_value(true);
        scale_rotate.set_digits(1);
        
    Gtk::Grid grid_rotate;
        grid_rotate.attach(text_rotate, 0, 0, 1, 1);
        grid_rotate.attach(scale_rotate, 1, 0, 1, 1);


///////////////////////// DRAW IMAGE /////////////////////////
    draw_area.signal_button_press_event().connect( sigc::mem_fun(this, &Calibration::onMouseClick) );
	sigc::connection draw_connection = Glib::signal_timeout().connect( sigc::mem_fun(this, &Calibration::updateScreen) , 50 );
    
///////////////////////// CONTAINERS /////////////////////////

    Gtk::HSeparator seperator1, seperator2, seperator3, seperator4;

    Gtk::Box right_box(Gtk::ORIENTATION_VERTICAL);
		right_box.set_spacing(30);
		right_box.set_border_width(20);
		right_box.pack_start(grid_radio_button, Gtk::PACK_SHRINK);
        right_box.pack_start(seperator1, Gtk::PACK_SHRINK);
		right_box.pack_start(combo_choose_player, Gtk::PACK_SHRINK);
        right_box.pack_start(seperator2, Gtk::PACK_SHRINK);
		right_box.pack_start(button_HSV_popover, Gtk::PACK_SHRINK);
        right_box.pack_start(seperator3, Gtk::PACK_SHRINK);
        right_box.pack_start(button_CAM_popover, Gtk::PACK_SHRINK);
        right_box.pack_start(seperator4, Gtk::PACK_SHRINK);      
		right_box.pack_start(grid_rotate, Gtk::PACK_SHRINK);

    Gtk::Box draw_box;
        draw_box.set_border_width(20);
        draw_box.pack_start(draw_area);  
    
    Gtk::Box under_menu_box(Gtk::ORIENTATION_HORIZONTAL);
		under_menu_box.pack_start(draw_box);    
        under_menu_box.pack_start(right_box, false, false, 20);

    Gtk::Box global_box(Gtk::ORIENTATION_VERTICAL);
		global_box.set_border_width(0);
        global_box.pack_start(menu_bar, Gtk::PACK_SHRINK);
        global_box.pack_start(under_menu_box);
		
    window.add(global_box);
    
    window.show_all();

  	app->run(window);
    
    draw_connection.disconnect();
    end_calibration = true;
}

void Calibration::initCameraConfig(){
    string value;

    value = executeCommand("uvcdynctrl -g brightness");
    if(value.compare("ERROR: Unknown control specified.\n") != 0){
        camera_config.brightness = stoi(value);
    }

    value = executeCommand("uvcdynctrl -g contrast");
    if(value.compare("ERROR: Unknown control specified.\n") != 0){
        camera_config.contrast = stoi(value);
    }

    value = executeCommand("uvcdynctrl -g saturation");
    if(value.compare("ERROR: Unknown control specified.\n") != 0){
        camera_config.saturation = stoi(value);
    }

    value = executeCommand("uvcdynctrl -g gain");
    if(value.compare("ERROR: Unknown control specified.\n") != 0){
        camera_config.gain = stoi(value);
    }

    value = executeCommand("uvcdynctrl -g sharpness");
    if(value.compare("ERROR: Unknown control specified.\n") != 0){
        camera_config.sharpness = stoi(value);
    }

    value = executeCommand("uvcdynctrl -g 'exposure (absolute)'");
    if(value.compare("ERROR: Unknown control specified.\n") != 0){
        camera_config.exposure = stoi(value);
    }
}

void Calibration::updateDevices(){
    string device;
    string name_device;

    device = executeCommand("uvcdynctrl -l | grep video0");
    if(device.size() != 0){
        name_device = device.substr(device.find("video")+9);
        menu_device0.set_label(name_device);
        menu_device0.set_use_stock(true);
        //vec_devices.push_back(menu_aux);
    }

    device = executeCommand("uvcdynctrl -l | grep video1");
    if(device.size() != 0){
        name_device = device.substr(device.find("video")+9);
        menu_device1.set_label(name_device);
        //vec_devices.push_back(menu_aux2);
    }
}

bool Calibration::updateScreen(){
    cv::Mat clone_image;
    if (cairo_binary_image){
        clone_image = opencv_image_binary.clone();
    } else {
        clone_image = opencv_image_cairo.clone();
    }
	draw_area.setImage(clone_image);

	return true;
}

bool Calibration::onMouseClick(GdkEventButton* event){
    if(event->button == GDK_BUTTON_PRIMARY) {
        updateColorPixel( changeCordinates({event->x, event->y}, draw_area.getCairoImageSize(), opencv_image_BGR.size()) );
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
        pointCutField1 = changeCordinates(draw_area.getPointCut1(), draw_area.getCairoImageSize(), opencv_image_BGR.size());
        pointCutField2 = changeCordinates(draw_area.getPointCut2(), draw_area.getCairoImageSize(), opencv_image_BGR.size());
        draw_area.setRectangleInvisible();
    }
    if (event->keyval == GDK_KEY_X || event->keyval == GDK_KEY_x) {
        pointCutField1 = {0,0};
        pointCutField2 = opencv_image_BGR.size();
    }
    return true;
}

void Calibration::onMenuGame(){
    program_state = GAME; app->quit();
}

void Calibration::onMenuCalibration(){
    program_state = CALIBRATION; app->quit();
}

void Calibration::onMenuSimulator(){
    program_state = SIMULATOR; app->quit();
}

void Calibration::onMenuArduino(){
    program_state = ARDUINO; app->quit();
}

void Calibration::onMenuQuit(){
    program_state = MENU; app->quit();
}

void Calibration::onButtonHSV() {
    HSV_popover.show_all();
    HSV_popover.set_visible(button_HSV_popover.get_focus_on_click());
}

void Calibration::onButtonCAM() {
    CAM_popover.show_all();
    CAM_popover.set_visible(button_CAM_popover.get_focus_on_click());
}

void Calibration::onChoosePlayer(){
    selected_player = combo_choose_player.get_active_row_number();
    scale_HSV_popover[0].set_value(colorsHSV[selected_player].variationH_MAX);
    scale_HSV_popover[1].set_value(colorsHSV[selected_player].variationH_MIN);
    scale_HSV_popover[2].set_value(colorsHSV[selected_player].variationS_MAX);
    scale_HSV_popover[3].set_value(colorsHSV[selected_player].variationS_MIN);
    scale_HSV_popover[4].set_value(colorsHSV[selected_player].variationV_MAX);
    scale_HSV_popover[5].set_value(colorsHSV[selected_player].variationV_MIN);
    //HSV_popover.show_all();
}

void Calibration::onScaleHMax(){
    colorsHSV[selected_player].variationH_MAX = scale_HSV_popover[0].get_value();
    colorsHSV[selected_player].setH(colorsHSV[selected_player].h[MID]);
}

void Calibration::onScaleHMin(){
    colorsHSV[selected_player].variationH_MIN = scale_HSV_popover[1].get_value();   
    colorsHSV[selected_player].setH(colorsHSV[selected_player].h[MID]);
}

void Calibration::onScaleSMax(){
    colorsHSV[selected_player].variationS_MAX = scale_HSV_popover[2].get_value();
    colorsHSV[selected_player].setS(colorsHSV[selected_player].s[MID]);
}

void Calibration::onScaleSMin(){
    colorsHSV[selected_player].variationS_MIN = scale_HSV_popover[3].get_value();
    colorsHSV[selected_player].setS(colorsHSV[selected_player].s[MID]);
}

void Calibration::onScaleVMax(){
    colorsHSV[selected_player].variationV_MAX = scale_HSV_popover[4].get_value();
    colorsHSV[selected_player].setV(colorsHSV[selected_player].v[MID]);
}

void Calibration::onScaleVMin(){
    colorsHSV[selected_player].variationV_MIN = scale_HSV_popover[5].get_value();
    colorsHSV[selected_player].setV(colorsHSV[selected_player].v[MID]);
}

void Calibration::onScaleRotate(){
    angle_image = scale_rotate.get_value();
}

void Calibration::onRadioButtonImage(){
    if (!radio_button_image.get_active()){
        button_CAM_popover.set_state(Gtk::StateType::STATE_INSENSITIVE);
        camera_on = false;
        imageInitialize();    
    }
}

void Calibration::onRadioButtonCamera(){
    if (!radio_button_camera.get_active()){    
        button_CAM_popover.set_state(Gtk::StateType::STATE_NORMAL);
        camera_on = true;
        imageInitialize();
    }
}

void Calibration::onScaleCAMBrightness(){
    executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Brightness' " + to_string(scale_CAM_popover[0].get_value()));
}

void Calibration::onScaleCAMContrast(){
    executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Contrast' " + to_string(scale_CAM_popover[1].get_value()));
}

void Calibration::onScaleCAMSaturation(){
    executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Saturation' " + to_string(scale_CAM_popover[2].get_value()));
}

void Calibration::onScaleCAMGain(){
    executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Gain' " + to_string(scale_CAM_popover[3].get_value()));
}

void Calibration::onScaleCAMSharpness(){
    executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Sharpness' " + to_string(scale_CAM_popover[4].get_value()));
}

void Calibration::onScaleCAMExposure(){
    executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Exposure (Absolute)' " + to_string(scale_CAM_popover[5].get_value()));
}