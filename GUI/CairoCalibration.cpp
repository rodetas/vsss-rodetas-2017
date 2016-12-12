#include "CairoCalibration.h"

CairoCalibration::CairoCalibration(){
    image = cv::Mat::zeros(1, 1, CV_64F);
}

bool CairoCalibration::on_draw (const Cairo::RefPtr<Cairo::Context> &cairo_draw){

    Gtk::Allocation allocation = get_allocation();
        field_size.x = allocation.get_width();
        field_size.y = allocation.get_height();
    
    cv::resize(image, image, field_size, 0, 0, cv::INTER_LINEAR);

    Gdk::Cairo::set_source_pixbuf(cairo_draw,  
    Gdk::Pixbuf::create_from_data(image.data, Gdk::COLORSPACE_RGB, false, 8, image.cols, image.rows, image.step));

    cairo_draw->paint();

    return true;
}

bool CairoCalibration::on_button_press_event(GdkEventButton *event) {
    if( (event->type == GDK_BUTTON_PRESS) && (event->button == 1) )	{
		cout << "mouse click" << endl;
		return true;
	} 
	return false;
}

void CairoCalibration::setImage(cv::Mat opencv){
    image = opencv;
    queue_draw();
}