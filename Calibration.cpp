#include "Calibration.h"

/*
 * class constructor
 */
Calibration::Calibration(){
    calibrationWasOpen = false;
    endCalibration = false;
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

    openWindow();

    std::thread calibration_thread(bind(&Calibration::GUI, this));

    while(!endCalibration){

        imageWebCam();
        
        opencvImageBGRCuted = opencvTransformation(opencvImageBGR, angleImageRotation, pointCutField1, pointCutField2, imageSizeZoom);
        opencvImageHSV      = opencvColorSpace(opencvImageBGRCuted, cv::COLOR_BGR2HSV_FULL);
        opencvImageBinary   = opencvBinary(colorsHSV[selectedTab], opencvImageHSV);
        
        opencvImageCairo    = opencvColorSpace(opencvImageBGRCuted, cv::COLOR_BGR2RGB);
        opencvBGRtoRGB      = opencvColorSpace(opencvImageBGRCuted, cv::COLOR_BGR2RGBA);

        gui.imageOpencvToSfml(opencvBGRtoRGB , opencvImageBinary);
        gui.GUI();
        
    }

    endCalibration = false;
    manipulation.saveCalibration(colorsHSV, colorsRGB, blobSize, pointCutField1, pointCutField2, goal, angleImageRotation, cameraOn);
    cam.release();
    gui.closeWindow();

    calibration_thread.detach();
    return program_state;
}

/*
 * verify if calibration window has already openned
 */
void Calibration::openWindow(){

    imageInitialize();

    if (!calibrationWasOpen){
        gui.createWindow();
        calibrationWasOpen = true;

    } else {
        gui.reopenWindow();
    }

    imageCanCut();
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

void Calibration::setGUICalibration(Calibration *c){
    gui.setGUI(c);
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
    set_icon_from_file("gtk.png");
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
    menu_play.add_accelerator("activate", accel_map, GDK_KEY_n, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
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
    
    vector<Gtk::Label> text_HSV(6);
        for (int i = 0; i < text_HSV.size(); i++){
            text_HSV[i].set_alignment(Gtk::ALIGN_START);
        }
        text_HSV[0].set_label("H max:");
        text_HSV[1].set_label("H min:");
        text_HSV[2].set_label("S max:");
        text_HSV[3].set_label("S min:");
        text_HSV[4].set_label("V max:");
        text_HSV[5].set_label("V min:");

    vector<Gtk::Scale> scale_HSV(6);
        for (int i = 0; i < scale_HSV.size(); i++){
            scale_HSV[i].set_size_request(150,20);
            scale_HSV[i].set_draw_value(false);
            scale_HSV[i].set_range(0,100);
            scale_HSV[i].set_value(50);
        }

    Gtk::Grid grid_pop_over;
        for (int i = 0; i < scale_HSV.size(); i++){
            grid_pop_over.attach(text_HSV[i], 0, i, 1, 1);
            grid_pop_over.attach(scale_HSV[i], 1, i, 2, 1);
        }
    
    Gtk::Box box_pop_over;
        box_pop_over.set_border_width(20);
        box_pop_over.pack_start(grid_pop_over);

////////////// declared in .h ////////////// 
    pop_menu.set_relative_to(btn_hsv);
    pop_menu.add(box_pop_over);

    btn_hsv.add_label("HSV Control");
    btn_hsv.signal_clicked().connect( sigc::mem_fun(this, &Calibration::onButtonHSV) );
///////////////////////////////////////////

    CairoCalibration draw_image;	
		sigc::connection draw_connection = Glib::signal_timeout().connect(sigc::bind< CairoCalibration* > ( sigc::mem_fun(this, &Calibration::setImage), &draw_image) , 50 );

    Gtk::RadioButton
        radio_button_image("Image"),
        radio_button_camera("Camera");

    Gtk::RadioButton::Group group = 
        radio_button_image.get_group();
        radio_button_camera.set_group(group);

    Gtk::ComboBoxText combo;
        combo.set_size_request(200, -1);

    Gtk::Scale scale_rotate;
        scale_rotate.set_size_request(150,20);
        scale_rotate.set_draw_value(false);
        scale_rotate.set_range(0,360);
        scale_rotate.set_value(180);

    Gtk::Label text_rotate("Rotate");

    Gtk::Separator separator1, separator2, separator3;

    Gtk::Grid grid;
        grid.set_row_spacing(40);
        grid.set_border_width(20);

        grid.attach(radio_button_image, 0, 0, 1, 1);
        grid.attach(radio_button_camera, 1, 0, 1, 1);
        grid.attach(separator1, 0, 1, 2, 1);
        grid.attach(combo, 0, 2, 2, 1);
        grid.attach(separator2, 0, 3, 2, 1);      
        grid.attach(btn_hsv, 0, 4, 2, 1);
        grid.attach(separator3, 0, 5, 2, 1);
        grid.attach(text_rotate, 0, 6, 1, 1);
        grid.attach(scale_rotate, 1, 6, 1, 1);
        
    Gtk::Box box_draw;
        box_draw.set_border_width(20);
        box_draw.pack_start(draw_image);  
    
    Gtk::Box box_under_menu(Gtk::ORIENTATION_HORIZONTAL);
		box_under_menu.pack_start(box_draw);    
        box_under_menu.pack_start(grid, false, false, 20);

    Gtk::Box box(Gtk::ORIENTATION_VERTICAL);
		box.set_border_width(0);
        box.pack_start(menu_bar, Gtk::PACK_SHRINK);
        box.pack_start(box_under_menu);
		
    add(box);
    show_all();

  	app->run(*this);
	
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
    bool active = btn_hsv.get_focus_on_click();
    pop_menu.show_all();
    pop_menu.set_visible(active);
}