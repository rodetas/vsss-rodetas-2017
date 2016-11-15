#include "Calibration.h"

/*
 * class constructor
 */
Calibration::Calibration(){
    end_calibration = false;
    
    selectedTab = 0;
    resolutionCamera = {1920,1080};
    colorsHSV.resize(6);
    colorsRGB.resize(6);
    blobSize.resize(6);
    range.resize(6);   

    changedColor = false;

    getCalibration();
    camera = getCameraNumber();

    selected_player = 0;
    program_state = MENU;

    initCameraConfig();
}

/*
 * calibration main loop
 */ 
int Calibration::calibrate(){

    imageInitialize();

    std::thread calibration_thread(bind(&Calibration::GUI, this));

    while(!end_calibration){
        imageWebCam();
        
        opencv_image_BGR_cuted  = opencvTransformation(opencv_image_BGR, angle_image, pointCutField1, pointCutField2, imageSizeZoom);
        opencv_image_HSV        = opencvColorSpace(opencv_image_BGR_cuted, cv::COLOR_BGR2HSV_FULL);
        opencv_image_cairo      = opencvColorSpace(opencv_image_BGR_cuted, cv::COLOR_BGR2RGB);
        opencv_image_binary     = opencvBinary(colorsHSV[selectedTab], opencv_image_HSV);
    }

    manipulation.saveCalibration(colorsHSV, colorsRGB, blobSize, pointCutField1, pointCutField2, goal, angle_image, cameraOn);
    cam.release();

    calibration_thread.detach();
    return program_state;
}

void Calibration::updateRGB(){
    colorsRGB[selectedTab].r = rgbPoint[2];
    colorsRGB[selectedTab].g = rgbPoint[1];
    colorsRGB[selectedTab].b = rgbPoint[0];
}

/*  
 * update treated image color that is showing to the user
 */
void Calibration::updateHSV(){

    colorsHSV[selectedTab].setH(hsvPoint[0]);
    colorsHSV[selectedTab].setS(hsvPoint[1]);
    colorsHSV[selectedTab].setV(hsvPoint[2]);

    blobSize[selectedTab] = blobRadius( opencvBinary(colorsHSV[selectedTab], opencv_image_HSV) );
    
    changedColor = false;
}

/*  
 * update the slider value in GUI
 */
void Calibration::updateSlider(){

    hsvPoint[0] = colorsHSV[selectedTab].h[2];
    hsvPoint[1] = colorsHSV[selectedTab].s[2];
    hsvPoint[2] = colorsHSV[selectedTab].v[2];

    colorsHSV[selectedTab].variationH_MAX = range[H_MAX];
    colorsHSV[selectedTab].variationS_MAX = range[S_MAX];
    colorsHSV[selectedTab].variationV_MAX = range[V_MAX];
    colorsHSV[selectedTab].variationH_MIN = range[H_MIN];
    colorsHSV[selectedTab].variationS_MIN = range[S_MIN];
    colorsHSV[selectedTab].variationV_MIN = range[V_MIN];

    if(changedColor) {   
        updateHSV();
    }
}

/*  
 * update the value of the clicked pixel
 */
void Calibration::updateColorPixel(Point pixelPoint){

    hsvPoint = opencv_image_HSV.at<cv::Vec3b>(pixelPoint.y-margin.y,pixelPoint.x-margin.x);
    rgbPoint = opencv_image_BGR_cuted.at<cv::Vec3b>(pixelPoint.y-margin.y,pixelPoint.x-margin.x);

    if(changedColor) 
        updateHSV();
}
/*  
 * updated hsvPoint for the current color
 */
void Calibration::updateTab(){

    hsvPoint[0] = colorsHSV[selectedTab].h[2];
    hsvPoint[1] = colorsHSV[selectedTab].s[2];
    hsvPoint[2] = colorsHSV[selectedTab].v[2];

    range[H_MAX] = colorsHSV[selectedTab].variationH_MAX;
    range[S_MAX] = colorsHSV[selectedTab].variationS_MAX;
    range[V_MAX] = colorsHSV[selectedTab].variationV_MAX;
    range[H_MIN] = colorsHSV[selectedTab].variationH_MIN;
    range[S_MIN] = colorsHSV[selectedTab].variationS_MIN;
    range[V_MIN] = colorsHSV[selectedTab].variationV_MIN;

    if(changedColor)
        updateHSV();
}

/*
 * Method that recognize blobs to draw
 */
sf::VertexArray Calibration::drawBlobs(cv::Mat image){
    
    sf::VertexArray pointSFML(sf::Points);

    for (int i = 0; i < image.rows ; i++){
        for (int j = 0; j < image.cols ; j++){

            int pointOpenCV = (int)image.at<uchar>(i, j);

            if (pointOpenCV != 0){
                pointSFML.append(sf::Vertex(sf::Vector2f(j + 0.5f + margin.x, i + 0.5f + margin.y), sf::Color(255,0,0,255)));
            }
        }
    }

    return pointSFML;
}

/*
 * Method to convert zoom cordinates in original image cordinates
 */
Point Calibration::changeCordinates(Point pointZoom){
    Point pointOriginal;

        pointOriginal.x = float(pointZoom.x) * float(imageSize.x) / float(imageSizeZoom.x);
        pointOriginal.y = float(pointZoom.y) * float(imageSize.y) / float(imageSizeZoom.y);
        
    return pointOriginal;
}

/*
 * Methods to initialize image
 */
void Calibration::imageInitialize(){

    if(!cameraOn){
        if(cam.isOpened()) { cam.release(); }
        
        opencv_image_BGR = cv::imread(imagePath);

        if(opencv_image_BGR.empty()){
            emptyImage();
        }

    } else {

        if(!cam.isOpened()) { cam = cv::VideoCapture(camera); }

        if(cam.isOpened()){
            cam.set(CV_CAP_PROP_FRAME_WIDTH, resolutionCamera.x);
            cam.set(CV_CAP_PROP_FRAME_HEIGHT, resolutionCamera.y);
            cam >> opencv_image_BGR;
            
            if(!opencv_image_BGR.empty()){
                #ifdef CAMERACONFIG
                    configureCamera();
                #endif
            } else { emptyImage(); }

        } else { closeConectionCamera(); }
    }

    imageSize = opencv_image_BGR.size();
}

/*
 * Method that receives and analyzes image from webcam
 */
void Calibration::imageWebCam(){

    if(cameraOn){
        cam >> opencv_image_BGR; 
        if (opencv_image_BGR.empty()){
            cout << "CRIAR TRATAMENTO DE ERRO PARA IMAGEM VAZIA" << endl;
        }
    }
}

void Calibration::imageCanCut(){

    if( pointCutField1.x > opencv_image_BGR.cols || pointCutField1.y > opencv_image_BGR.rows ||
        pointCutField2.x > opencv_image_BGR.cols || pointCutField2.y > opencv_image_BGR.rows ){
        pointCutFieldDefault();
    }

    if( pointCutField1.x == 0 && pointCutField1.y == 0 && pointCutField2.x == 0 && pointCutField2.y == 0) {
        pointCutFieldDefault();
    }
}

void Calibration::emptyImage(){
    cout << "Empty image from camera" << endl;
}

void Calibration::closeConectionCamera(){
    cout << "Conection with camera failed" << endl;
}

void Calibration::pointCutFieldDefault(){
    pointCutField1 = {0,0};
    pointCutField2 = imageSize;
}

void Calibration::getCalibration(){
    manipulation.loadCalibration();
    colorsHSV = manipulation.getColorsHsv();
    colorsRGB = manipulation.getColorsRgb();

    pointCutField1 = manipulation.getPointField1();
    pointCutField2 = manipulation.getPointField2();
    goal = manipulation.getGoal();
    angle_image = manipulation.getAngleImage();
    cameraOn = manipulation.getCameraOn();
    blobSize = manipulation.getBlobSize();

    hsvPoint[0] = colorsHSV[selectedTab].h[2];
    hsvPoint[1] = colorsHSV[selectedTab].s[2];
    hsvPoint[2] = colorsHSV[selectedTab].v[2];

  //  manipulation.showCalibration();

}

void Calibration::GUI(){
			
	app = Gtk::Application::create();

    set_title("Calibration");
    set_icon_from_file("files/images/logo-rodetas.png");
    maximize();

    Glib::RefPtr<Gtk::AccelGroup> accel_map = Gtk::AccelGroup::create();
    add_accel_group(accel_map);

    vector<Glib::ustring> path;
    path.push_back("/usr/share/ubuntu/icons");

    Glib::RefPtr<Gtk::IconTheme> theme = Gtk::IconTheme::get_default();
    theme->set_search_path(path);

    std::vector< Glib::ustring > vec = theme->list_icons();

    for(int i=0 ; i<vec.size() ; i++){
       // cout << vec[i] << endl;
    }

    Gtk::MenuBar menu_bar;

//////////////////////// NAVEGATION MENU /////////////////////////

    Gtk::MenuItem menu_navegation;
    menu_navegation.set_label("Navegation");

    Gtk::Menu subMenuNavigation;
    menu_navegation.set_submenu(subMenuNavigation);

    Gtk::MenuItem menu_play;

    menu_play.set_label("_Start Game");
    menu_play.set_use_underline(true);
    menu_play.drag_source_set_icon(theme->load_icon("gtk-apply", 10));
    menu_play.add_accelerator("activate", accel_map, GDK_KEY_n, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE);
    menu_play.show();

    menu_play.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuGame));
    subMenuNavigation.append(menu_play);

    Gtk::MenuItem menu_calibration;
    menu_calibration.set_label("_Calibrate");
    menu_calibration.set_use_underline(true);
    menu_calibration.add_accelerator("activate", accel_map, GDK_KEY_c, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menu_calibration.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuCalibration));
    subMenuNavigation.append(menu_calibration);

    Gtk::MenuItem menu_simulator;
    menu_simulator.set_label("S_imulate");
    menu_simulator.set_use_underline(true);
    menu_simulator.add_accelerator("activate", accel_map, GDK_KEY_s, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menu_simulator.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuSimulator));
    subMenuNavigation.append(menu_simulator);

    Gtk::MenuItem menu_arduino;
    menu_arduino.set_label("_Upload Arduino");
    menu_arduino.set_use_underline(true);
    menu_arduino.add_accelerator("activate", accel_map, GDK_KEY_u, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menu_arduino.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuArduino));
    subMenuNavigation.append(menu_arduino);

    Gtk::SeparatorMenuItem separator;
    subMenuNavigation.append(separator);

    Gtk::MenuItem menu_quit;
    menu_quit.set_label("_Quit");
    menu_quit.set_use_underline(true);
    menu_quit.add_accelerator("activate", accel_map, GDK_KEY_Escape, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE);
    menu_quit.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuQuit));
    subMenuNavigation.append(menu_quit);

    menu_bar.append(menu_navegation);

/////////////////// FILE MENU //////////////////////////

    Gtk::MenuItem menu_file;
    menu_file.set_label("Calibration");

    Gtk::Menu subMenuFile;
    menu_file.set_submenu(subMenuFile);

    Gtk::ImageMenuItem menu_save(Gtk::Stock::SAVE);
    menu_save.set_state(Gtk::StateType::STATE_INSENSITIVE);
    menu_save.set_label("Save Calibration");
    subMenuFile.append(menu_save);

    Gtk::ImageMenuItem menu_cut(Gtk::Stock::CUT);
    menu_cut.set_state(Gtk::StateType::STATE_INSENSITIVE);
    menu_cut.set_label("Cut Image");
    subMenuFile.append(menu_cut);

    Gtk::ImageMenuItem menu_reset(Gtk::Stock::CLEAR);
    menu_reset.set_state(Gtk::StateType::STATE_INSENSITIVE);
    menu_reset.set_label("Reset Values");
    subMenuFile.append(menu_reset);

    menu_bar.append(menu_file);

///////////////////// CAMERA MENU ////////////////////////////////////

    Gtk::MenuItem menu_camera;
    menu_camera.set_label("Camera");

    Gtk::Menu subMenuCamera;
    menu_camera.set_submenu(subMenuCamera);

    updateDevices();

    //menu_device0.drag_source_set_icon("edit-delete");
    subMenuCamera.append(menu_device0);
    subMenuCamera.append(menu_device1);

    Gtk::ImageMenuItem menu_load_camera_config(Gtk::Stock::OPEN);
    menu_load_camera_config.set_label("Load Camera Configuration");
    menu_load_camera_config.set_state(Gtk::StateType::STATE_INSENSITIVE);
    subMenuCamera.append(menu_load_camera_config);

    Gtk::MenuItem menu_refresh;
    menu_refresh.set_label("Refresh Device");
    menu_refresh.set_state(Gtk::StateType::STATE_INSENSITIVE);
    menu_refresh.add_accelerator("activate", accel_map, GDK_KEY_F5, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE); //116 -> f5
    subMenuCamera.append(menu_refresh);

    menu_bar.append(menu_camera);

/////////////////// RADIO BUTTON - SET DEVICE ///////////////////////

    radio_button_image.set_label("Image");
    radio_button_camera.set_label("Camera");

    Gtk::RadioButton::Group group;
        radio_button_image.signal_pressed().connect(sigc::mem_fun(this, &Calibration::onRadioButtonImage));
        radio_button_camera.signal_pressed().connect(sigc::mem_fun(this, &Calibration::onRadioButtonCamera));

        radio_button_image.set_group(group);
        radio_button_camera.set_group(group);

    Gtk::Grid grid_radio_button;
        grid_radio_button.set_column_homogeneous(true);
        grid_radio_button.attach(radio_button_image, 0, 0, 1, 1);
        grid_radio_button.attach(radio_button_camera, 1, 0, 1, 1);

/////////////////// COMBO BOX - SELECT Player /////////////////////////

        combo_choose_player.set_size_request(200, -1);
        combo_choose_player.append("Player 0");
        combo_choose_player.append("Player 1");
        combo_choose_player.append("Player 2");
        combo_choose_player.append("Team");
        combo_choose_player.append("Opponent");
        combo_choose_player.append("Ball");
        combo_choose_player.set_active_text("Player 0");
        combo_choose_player.signal_changed().connect(sigc::mem_fun(this, &Calibration::onChoosePlayer));
        combo_choose_player.add_accelerator(
                "popup", accel_map, 32, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE); // 32 -> Space bar


/////////////////// BOX - SELECT HSV //////////////////////////

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

    HSV_popover.set_position(Gtk::PositionType::POS_BOTTOM);
    HSV_popover.set_relative_to(button_HSV_popover);
    HSV_popover.add(box_HSV_popover);

    button_HSV_popover.add_label("HSV Controls");
    button_HSV_popover.signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonHSV) );

/////////////////// BOX - CONFIG CAMERA //////////////////////////

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
 
    scale_CAM_popover.resize(6);
        for (int i = 0; i < scale_CAM_popover.size(); i++){
            scale_CAM_popover[i].set_size_request(150,20);
            scale_CAM_popover[i].set_draw_value(false);
            scale_CAM_popover[i].set_range(0,100);
        }

        scale_CAM_popover[0].set_value(camera_config.brightness);
        scale_CAM_popover[0].set_range(-64,64);
        scale_CAM_popover[1].set_value(camera_config.contrast);
        scale_CAM_popover[2].set_value(camera_config.saturation);
        scale_CAM_popover[3].set_value(camera_config.gain);
        scale_CAM_popover[4].set_value(camera_config.sharpness);
        scale_CAM_popover[5].set_value(camera_config.exposure);
        scale_CAM_popover[5].set_range(50,10000);

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

    button_CAM_popover.add_label("CAM Controls");
    button_CAM_popover.signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonCAM) );

    if(radio_button_image.get_active()){
        button_CAM_popover.set_state(Gtk::StateType::STATE_INSENSITIVE);
    }

//////////////////////// SCALE ROTATE IMAGE ///////////////////////

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

/////////////////// DRAW IMAGE //////////////////////////

    CairoCalibration draw_image;	
		sigc::connection draw_connection = Glib::signal_timeout().connect(sigc::bind< CairoCalibration* > ( sigc::mem_fun(this, &Calibration::setImage), &draw_image) , 50 );

///////////////////////// CONTAINERS /////////////////////////////

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
        draw_box.pack_start(draw_image);  
    
    Gtk::Box under_menu_box(Gtk::ORIENTATION_HORIZONTAL);
		under_menu_box.pack_start(draw_box);    
        under_menu_box.pack_start(right_box, false, false, 20);

    Gtk::Box global_box(Gtk::ORIENTATION_VERTICAL);
		global_box.set_border_width(0);
        global_box.pack_start(menu_bar, Gtk::PACK_SHRINK);
        global_box.pack_start(under_menu_box);
		
    add(global_box);
    show_all();

  	app->run(*this);
    
    end_calibration = true;
}

void Calibration::initCameraConfig(){
    string value;
    value = executeCommand("uvcdynctrl -g brightness");
    camera_config.brightness = stoi(value);
    value = executeCommand("uvcdynctrl -g contrast");
    camera_config.contrast = stoi(value);
    value = executeCommand("uvcdynctrl -g saturation");
    camera_config.saturation = stoi(value);

    value = executeCommand("uvcdynctrl -g gain");
    if(value.compare("ERROR: Unknown control specified.\n") != 0){
        camera_config.gain = stoi(value);
    }

    value = executeCommand("uvcdynctrl -g sharpness");
    camera_config.sharpness = stoi(value);
    value = executeCommand("uvcdynctrl -g 'exposure (absolute)'");
    camera_config.exposure = stoi(value);
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

bool Calibration::setImage(CairoCalibration *c){
	c->setImage(opencv_image_cairo);
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
    cout << selected_player << endl;
    HSV_popover.show_all();
}

void Calibration::onScaleCAMBrightness(){
    cout << scale_CAM_popover[0].get_value() << endl;
}

void Calibration::onScaleCAMContrast(){
    cout << scale_CAM_popover[1].get_value() << endl;
}

void Calibration::onScaleCAMSaturation(){
    cout << scale_CAM_popover[2].get_value() << endl;
}

void Calibration::onScaleCAMGain(){
    cout << scale_CAM_popover[3].get_value() << endl;
}

void Calibration::onScaleCAMSharpness(){
    cout << scale_CAM_popover[4].get_value() << endl;
}

void Calibration::onScaleCAMExposure(){
    cout << scale_CAM_popover[5].get_value() << endl;
}

void Calibration::onScaleHMax(){
    cout << scale_HSV_popover[0].get_value() << endl;
}

void Calibration::onScaleHMin(){
    cout << scale_HSV_popover[1].get_value() << endl;
}

void Calibration::onScaleSMax(){
    cout << scale_HSV_popover[2].get_value() << endl;
}

void Calibration::onScaleSMin(){
    cout << scale_HSV_popover[3].get_value() << endl;
}

void Calibration::onScaleVMax(){
    cout << scale_HSV_popover[4].get_value() << endl;
}

void Calibration::onScaleVMin(){
    cout << scale_HSV_popover[5].get_value() << endl;
}

void Calibration::onScaleRotate(){
    angle_image = scale_rotate.get_value();
}

void Calibration::onRadioButtonImage(){
    button_CAM_popover.set_state(Gtk::StateType::STATE_INSENSITIVE);
}

void Calibration::onRadioButtonCamera(){
    button_CAM_popover.set_state(Gtk::StateType::STATE_NORMAL);
}
