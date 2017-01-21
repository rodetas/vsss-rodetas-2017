#include "CairoCalibration.h"

CairoCalibration::CairoCalibration(){   
    add_events(Gdk::BUTTON_PRESS_MASK);

    pixel_color = {0,0};
    opencv_image = cv::Mat::zeros(1, 1, CV_64F);
}

bool CairoCalibration::on_draw (const Cairo::RefPtr<Cairo::Context> &cairo_draw){

    Gtk::Allocation allocation = get_allocation();
        cairo_image_size.x = allocation.get_width();
        cairo_image_size.y = allocation.get_height();
        opencv_image_size  = opencv_image.size();
    
    cv::Mat opencv_image_resize;
    cv::resize(opencv_image, opencv_image_resize, cairo_image_size, 0, 0, cv::INTER_LINEAR);

    Glib::RefPtr<Gdk::Pixbuf> pixbuf =  Gdk::Pixbuf::create_from_data( opencv_image_resize.data, Gdk::COLORSPACE_RGB, false, 8, opencv_image_resize.cols, opencv_image_resize.rows, opencv_image_resize.step);
    Gdk::Cairo::set_source_pixbuf(cairo_draw, pixbuf);

    cairo_draw->paint();

    return true;
}

void CairoCalibration::setImage(cv::Mat image){
    opencv_image = image;
    queue_draw();
}

Point CairoCalibration::getCairoImageSize(){
    return cairo_image_size;
}