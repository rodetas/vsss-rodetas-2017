#include "OpenCV.h"

OpenCV::OpenCV(){

}

/*
 * Method that binarize the image with opencv
 */
cv::Mat OpenCV::opencvBinary(Hsv color, cv::Mat image){

    cv::inRange(image, cv::Scalar(color.h[0], color.s[0], color.v[0]), cv::Scalar(color.h[1], color.s[1], color.v[1]), image); 
    cv::medianBlur(image, image, 3);

    return image;
}

/*
 * Method to convert color space
 */
cv::Mat OpenCV::opencvColorSpace(cv::Mat image, int code){

    cv::Mat newColorSpace;
    cv::cvtColor(image, newColorSpace, code);

    return newColorSpace;
}

/*
 * Method that cuts, rotates and resize
 */
cv::Mat OpenCV::opencvTransformation(cv::Mat image, int angle, cv::Point pointCutField1, cv::Point pointCutField2, cv::Point size){

    cv::Mat imageRotated;
    cv::warpAffine( image, imageRotated, cv::getRotationMatrix2D( cv::Point(image.cols/2, image.rows/2), angle - 180, 1), imageRotated.size() );
    cv::Mat cutImage = imageRotated( cv::Rect(pointCutField1, pointCutField2) );

    if (size.x != 0 && size.y != 0){
    	cv::resize(cutImage, cutImage, size, 0, 0, cv::INTER_LINEAR);
    }

    return cutImage;
}

/*
 * Method for find contours in binarized image
 */
BlobsContours OpenCV::blobContour(cv::Mat image, float blobSize){

    BlobsContours blobContours;
    
    vector< vector<cv::Point> > contours;
    vector<cv::Vec4i>           hierarchy;

    cv::findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    vector< float>              radius (contours.size());
    vector< cv::Point2f>        center (contours.size());    
    vector< vector<cv::Point> > contours_poly (contours.size());
    
    for(int i = 0; i < contours.size(); i++){
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        cv::minEnclosingCircle(contours_poly[i], center[i], radius[i] );
    
        if (radius[i] > blobSize * 0.7){

            Point2f cutPoint1 = cv::Point( center[i].x - radius[i], center[i].y - radius[i] );
            Point2f cutPoint2 = cv::Point( center[i].x + radius[i], center[i].y + radius[i] );

            if (cutPoint1.x < 0) cutPoint1.x = 0;
            if (cutPoint1.y < 0) cutPoint1.y = 0;
            if (cutPoint2.x < 0) cutPoint2.x = 0;
            if (cutPoint2.y < 0) cutPoint2.y = 0;
            if (cutPoint1.x > image.cols) cutPoint1.x = image.cols;
            if (cutPoint1.y > image.rows) cutPoint1.y = image.rows;
            if (cutPoint2.x > image.cols) cutPoint2.x = image.cols;
            if (cutPoint2.y > image.rows) cutPoint2.y = image.rows;

            blobContours.center.push_back(center[i]);
            blobContours.radius.push_back(radius[i]);
            blobContours.cutPoint1.push_back(cutPoint1);
            blobContours.cutPoint2.push_back(cutPoint2);
        }
    }

    return blobContours;
}

float OpenCV::blobRadius(cv::Mat image){

    float blobRadius = 0;
    
    vector< vector<cv::Point> > contours;
    vector<cv::Vec4i>           hierarchy;

    cv::findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    vector< float>              radius (contours.size());
    vector< cv::Point2f>        center (contours.size());    
    vector< vector<cv::Point> > contours_poly (contours.size());
    
    for(int i = 0; i < contours.size(); i++){
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        cv::minEnclosingCircle(contours_poly[i], center[i], radius[i] );
        
        if (blobRadius < radius[i]){
            blobRadius = radius[i];
        }

    }

    return blobRadius;
}