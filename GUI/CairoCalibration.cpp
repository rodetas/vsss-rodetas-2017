#include "CairoCalibration.h"

CairoCalibration::CairoCalibration(){
    image = cv::Mat::zeros(1, 1, CV_64F);
}

bool CairoCalibration::on_draw (const Cairo::RefPtr<Cairo::Context> &c){

    Gtk::Allocation allocation = get_allocation();
        field_size.x = allocation.get_width();
        field_size.y = allocation.get_height();
    
    cv::resize(image, image, field_size, 0, 0, cv::INTER_LINEAR);

    Gdk::Cairo::set_source_pixbuf(c, Gdk::Pixbuf::create_from_data(image.data, Gdk::COLORSPACE_RGB, false, 8, image.cols, image.rows, image.step));
    c->paint();
    return true;
}

void CairoCalibration::setImage(cv::Mat opencv){
    image = opencv;
    queue_draw();
}