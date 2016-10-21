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

    while(!endCalibration){

        imageWebCam();
        
        opencvImageBGRCuted = opencvTransformation(opencvImageBGR, angleImageRotation, pointCutField1, pointCutField2, imageSizeZoom);
        opencvImageHSV      = opencvColorSpace(opencvImageBGRCuted, cv::COLOR_BGR2HSV_FULL);
        opencvImageBinary   = opencvBinary(colorsHSV[selectedTab], opencvImageHSV);
        opencvBGRtoRGB      = opencvColorSpace(opencvImageBGRCuted, cv::COLOR_BGR2RGBA);

        gui.imageOpencvToSfml(opencvBGRtoRGB , opencvImageBinary);
        gui.GUI();
        
    }

    endCalibration = false;
    manipulation.saveCalibration(colorsHSV, colorsRGB, blobSize, pointCutField1, pointCutField2, goal, angleImageRotation, cameraOn);
    cam.release();
    gui.closeWindow();

    //  manipulation.showCalibration();
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
