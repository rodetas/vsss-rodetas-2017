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
    return MENU;
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

	Gtk::Window window;	
		window.maximize();
		window.set_title("Calibration");

    CairoCalibration draw_image;	
		sigc::connection draw_connection = Glib::signal_timeout().connect(sigc::bind< CairoCalibration* > ( sigc::mem_fun(this, &Calibration::setImage), &draw_image) , 50 );
    
    Gtk::Label 
        text_rotate("Rotation"), 
        text_H_max("H max: "), 
        text_H_min("H min: "),
        text_S_max("S max: "), 
        text_S_min("S min: "),
        text_V_max("V max: "), 
        text_V_min("V min: "),
        text_player0,
        text_player1,
        text_player2,
        text_team,
        text_opponent,
        text_ball;


    Gtk::Scale scale_rotate, scale_H_max, scale_S_max, scale_V_max;
        scale_rotate.set_range(0,360);
        scale_rotate.set_value(180);
        scale_H_max.set_range(0,50);
        scale_H_max.set_value(25);
        scale_S_max.set_range(0,50);
        scale_S_max.set_value(25);
        scale_V_max.set_range(0,50);
        scale_V_max.set_value(25);

    Gtk::Stack stack;
        stack.add(text_player0, "player_0", "Player 0");
        stack.add(text_player1, "player_1", "Player 1");
        stack.add(text_player2, "player_2", "Player 2");
        stack.add(text_team, "team", "Team");
        stack.add(text_opponent, "opponent", "Opponent");
        stack.add(text_ball, "ball", "Ball");
    
    Gtk::StackSwitcher stack_switcher;
        stack_switcher.set_stack(stack);

    Gtk::Grid grid;
        grid.set_valign(Gtk::ALIGN_CENTER);
        grid.set_halign(Gtk::ALIGN_CENTER);

        grid.attach(text_rotate,1,0,1,1);
        grid.attach(scale_rotate,2,0,3,1);

        grid.attach(text_H_max,0,1,1,1);
        grid.attach(scale_H_max,1,1,1,1);

        grid.attach(text_S_max,2,1,1,1);
        grid.attach(scale_S_max,3,1,1,1);

        grid.attach(text_V_max,4,1,1,1);
        grid.attach(scale_V_max,5,1,1,1);

        grid.attach(stack_switcher,0,2,6,1);
        grid.attach(stack,0,3,6,1);

    Gtk::Box box(Gtk::ORIENTATION_VERTICAL);
		box.set_border_width(20);
		box.pack_start(grid, false, false, 20);        
		box.pack_start(draw_image);

    window.add(box);
    window.show_all();

  	app->run(window);
	
	return MENU;
}

bool Calibration::setImage(CairoCalibration *c){
	c->setImage(opencvImageCairo);
	return true;
} 