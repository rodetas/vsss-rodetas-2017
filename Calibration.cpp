#include "Calibration.h"

/*
 * class constructor
 */
Calibration::Calibration(){
    calibrationWasOpen = false;
    endCalibration = true;
    selectedTab = 0;
    resolutionCamera = {1920,1080};
    colorsHSV.resize(6);
    colorsRGB.resize(6);
    blobSize.resize(6);
    range.resize(6);   
    changedColor = false;

    getCalibration();
    camera = getCameraNumber();

    program_state = MENU;
}

/*
 * calibration main loop
 */ 
int Calibration::calibrate(){

    imageInitialize();

    std::thread calibration_thread(bind(&Calibration::GUI, this));

    while(endCalibration){

        imageWebCam();
        
        opencvImageBGRCuted = opencvTransformation(opencvImageBGR, angleImageRotation, pointCutField1, pointCutField2, imageSizeZoom);
        opencvImageHSV      = opencvColorSpace(opencvImageBGRCuted, cv::COLOR_BGR2HSV_FULL);
        opencvImageBinary   = opencvBinary(colorsHSV[selectedTab], opencvImageHSV);
        
        opencvImageCairo    = opencvColorSpace(opencvImageBGRCuted, cv::COLOR_BGR2RGB);
        opencvBGRtoRGB      = opencvColorSpace(opencvImageBGRCuted, cv::COLOR_BGR2RGBA);
        
    }

    manipulation.saveCalibration(colorsHSV, colorsRGB, blobSize, pointCutField1, pointCutField2, goal, angleImageRotation, cameraOn);
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

    blobSize[selectedTab] = blobRadius( opencvBinary(colorsHSV[selectedTab], opencvImageHSV) );
    
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

    hsvPoint = opencvImageHSV.at<cv::Vec3b>(pixelPoint.y-margin.y,pixelPoint.x-margin.x);
    rgbPoint = opencvImageBGRCuted.at<cv::Vec3b>(pixelPoint.y-margin.y,pixelPoint.x-margin.x);

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
        
        opencvImageBGR = cv::imread(imagePath);

        if(opencvImageBGR.empty()){
            emptyImage();
        }

    } else {

        if(!cam.isOpened()) { cam = cv::VideoCapture(camera); }

        if(cam.isOpened()){
            cam.set(CV_CAP_PROP_FRAME_WIDTH, resolutionCamera.x);
            cam.set(CV_CAP_PROP_FRAME_HEIGHT, resolutionCamera.y);
            cam >> opencvImageBGR;
            
            if(!opencvImageBGR.empty()){
                #ifdef CAMERACONFIG
                    configureCamera();
                #endif
            } else { emptyImage(); }

        } else { closeConectionCamera(); }
    }

    imageSize = opencvImageBGR.size();
}

/*
 * Method that receives and analyzes image from webcam
 */
void Calibration::imageWebCam(){

    if(cameraOn){
        cam >> opencvImageBGR; 
        if (opencvImageBGR.empty()){
            cout << "CRIAR TRATAMENTO DE ERRO PARA IMAGEM VAZIA" << endl;
        }
    }
}

void Calibration::imageCanCut(){

    if( pointCutField1.x > opencvImageBGR.cols || pointCutField1.y > opencvImageBGR.rows ||
        pointCutField2.x > opencvImageBGR.cols || pointCutField2.y > opencvImageBGR.rows ){
        pointCutFieldDefault();
    }

    if( pointCutField1.x == 0 && pointCutField1.y == 0 && pointCutField2.x == 0 && pointCutField2.y == 0) {
        pointCutFieldDefault();
    }
}

void Calibration::emptyImage(){
    endCalibration = true;
    cout << "Empty image from camera" << endl;
}

void Calibration::closeConectionCamera(){
    endCalibration = true;
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
    angleImageRotation = manipulation.getAngleImage();
    cameraOn = manipulation.getCameraOn();
    blobSize = manipulation.getBlobSize();

    hsvPoint[0] = colorsHSV[selectedTab].h[2];
    hsvPoint[1] = colorsHSV[selectedTab].s[2];
    hsvPoint[2] = colorsHSV[selectedTab].v[2];

  //  manipulation.showCalibration();

}

int Calibration::GUI(){
			
	app = Gtk::Application::create();

    set_title("Calibration");
    //set_icon_from_file("gtk.png");
    maximize();

    Glib::RefPtr<Gtk::AccelGroup> accel_map = Gtk::AccelGroup::create();
    add_accel_group(accel_map);

    Gtk::MenuBar menu_bar;

//////////////////////// NAVEGATION MENU /////////////////////////

    Gtk::MenuItem menu_navegation;
    menu_navegation.set_label("Navegation");

    Gtk::Menu subMenuNavigation;
    menu_navegation.set_submenu(subMenuNavigation);

    Gtk::MenuItem menu_play;
    menu_play.set_label("_Start Game");
    menu_play.set_use_underline(true);
    menu_play.add_accelerator("activate", accel_map, GDK_KEY_n, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE);
    menu_play.render_icon_pixbuf(Gtk::Stock::GO_FORWARD,Gtk::ICON_SIZE_LARGE_TOOLBAR);
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
    menu_quit.add_accelerator("activate", accel_map, GDK_KEY_q, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menu_quit.signal_activate().connect(sigc::mem_fun(this, &Calibration::onMenuQuit));
    subMenuNavigation.append(menu_quit);

/////////////////// FILE MENU //////////////////////////

    Gtk::MenuItem menu_file;
    menu_file.set_label("Calibration");

    Gtk::Menu subMenuFile;
    menu_file.set_submenu(subMenuFile);

    Gtk::ImageMenuItem menu_save(Gtk::Stock::SAVE);
    menu_save.set_label("Save Calibration");
    subMenuFile.append(menu_save);

    Gtk::ImageMenuItem menu_cut(Gtk::Stock::CUT);
    menu_cut.set_opacity(0.5);
    menu_cut.set_label("Cut Image");
    subMenuFile.append(menu_cut);

    Gtk::ImageMenuItem menu_reset(Gtk::Stock::CLEAR);
    menu_reset.set_label("Reset Values");
    subMenuFile.append(menu_reset);

    Gtk::ImageMenuItem menu_refresh(Gtk::Stock::REFRESH);
    menu_refresh.set_label("Refresh Devices");
    subMenuFile.append(menu_refresh);

    menu_bar.append(menu_navegation);
    menu_bar.append(menu_file);
    
/////////////////// HSV POPOVER //////////////////////////

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

    vector<Gtk::Scale> scale_HSV_popover(6);
        for (int i = 0; i < scale_HSV_popover.size(); i++){
            scale_HSV_popover[i].set_size_request(150,20);
            scale_HSV_popover[i].set_draw_value(false);
            scale_HSV_popover[i].set_range(0,100);
            scale_HSV_popover[i].set_value(50);
        }

    Gtk::Grid grid_HSV_popover;
        for (int i = 0; i < scale_HSV_popover.size(); i++){
            grid_HSV_popover.attach(text_HSV_popover[i], 0, i, 1, 1);
            grid_HSV_popover.attach(scale_HSV_popover[i], 1, i, 2, 1);
        }
    
    Gtk::Box box_HSV_popover;
        box_HSV_popover.set_border_width(20);
        box_HSV_popover.pack_start(grid_HSV_popover);

/////// declared in .h /////// 

    HSV_popover.set_relative_to(button_HSV_popover);
    HSV_popover.add(box_HSV_popover);

    button_HSV_popover.add_label("HSV Controls");
    button_HSV_popover.signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonHSV) );

//////////////////////////////

/////////////////// CAM POPOVER //////////////////////////

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
 
    vector<Gtk::Scale> scale_CAM_popover(6);
        for (int i = 0; i < scale_CAM_popover.size(); i++){
            scale_CAM_popover[i].set_size_request(150,20);
            scale_CAM_popover[i].set_draw_value(false);
            scale_CAM_popover[i].set_range(0,100);
            scale_CAM_popover[i].set_value(50);
        }
        //scale_CAM_popover[0].signal_value_changed().connect( sigc::mem_fun(this, &Calibration::onScaleCAM) );        

    Gtk::Grid grid_CAM_popover;
        for (int i = 0; i < scale_CAM_popover.size(); i++){
            grid_CAM_popover.attach(text_CAM_popover[i], 0, i, 1, 1);
            grid_CAM_popover.attach(scale_CAM_popover[i], 1, i, 2, 1);
        }
    
    Gtk::Box box_CAM_popover;
        box_CAM_popover.set_border_width(20);
        box_CAM_popover.pack_start(grid_CAM_popover);

/////// declared in .h /////// 

    CAM_popover.set_relative_to(button_CAM_popover);
    CAM_popover.add(box_CAM_popover);

    button_CAM_popover.add_label("CAM Controls");
    button_CAM_popover.signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonCAM) );

//////////////////////////////


    CairoCalibration draw_image;	
		sigc::connection draw_connection = Glib::signal_timeout().connect(sigc::bind< CairoCalibration* > ( sigc::mem_fun(this, &Calibration::setImage), &draw_image) , 50 );

    Gtk::RadioButton
        radio_button_image("Image"),
        radio_button_camera("Camera");
    Gtk::RadioButton::Group group = 
        radio_button_image.get_group();
        radio_button_camera.set_group(group);
    Gtk::Grid grid_radio_button;
        grid_radio_button.set_column_homogeneous(true);
        grid_radio_button.attach(radio_button_image, 0, 0, 1, 1);
        grid_radio_button.attach(radio_button_camera, 1, 0, 1, 1);

    Gtk::ComboBoxText combo_calibrate_select;
        combo_calibrate_select.set_size_request(200, -1);
        combo_calibrate_select.append("Player 0");
        combo_calibrate_select.append("Player 1");
        combo_calibrate_select.append("Player 2");
        combo_calibrate_select.append("Team");
        combo_calibrate_select.append("Opponent");
        combo_calibrate_select.append("Ball");
        combo_calibrate_select.set_active_text("Player 0");
        combo_calibrate_select.signal_show().connect(sigc::mem_fun(this, &Calibration::onCalibrateAccel));
        combo_calibrate_select.add_accelerator("", accel_map, GDK_KEY_Up, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);

    Gtk::Label text_rotate("Rotate");
    Gtk::Scale scale_rotate;
        scale_rotate.set_size_request(150,20);
        scale_rotate.set_draw_value(false);
        scale_rotate.set_range(0,360);
        scale_rotate.set_value(180);
    Gtk::Grid grid_rotate;
        grid_rotate.attach(text_rotate, 0, 0, 1, 1);
        grid_rotate.attach(scale_rotate, 1, 0, 1, 1);

    Gtk::HSeparator seperator1, seperator2, seperator3, seperator4;

    Gtk::Box right_box(Gtk::ORIENTATION_VERTICAL);
		right_box.set_spacing(30);
		right_box.set_border_width(20);
		right_box.pack_start(grid_radio_button, Gtk::PACK_SHRINK);
        right_box.pack_start(seperator1, Gtk::PACK_SHRINK);
		right_box.pack_start(combo_calibrate_select, Gtk::PACK_SHRINK);
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
	
    endCalibration = false;

	return program_state;
}

bool Calibration::setImage(CairoCalibration *c){
	c->setImage(opencvImageCairo);
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

void Calibration::onCalibrateAccel(){
    cout << "teste" << endl;
}