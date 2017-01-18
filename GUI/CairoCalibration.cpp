#include "CairoCalibration.h"

CairoCalibration::CairoCalibration(){
    pixel_color = {0,0};
    binary_image = false;
    opencv_image = cv::Mat::zeros(1, 1, CV_64F);
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::KEY_PRESS_MASK);
    set_can_focus(true);
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

bool CairoCalibration::on_button_press_event(GdkEventButton* event) {
    if(event->button == GDK_BUTTON_PRIMARY && !binary_image) {
        pixel_color = { event->x, event->y };
		return true;
	} 
	return false;
}

bool CairoCalibration::on_key_press_event(GdkEventKey* key_event){
    if (key_event->keyval == GDK_KEY_F1) {
        binary_image = !binary_image;
        return true;
    }
    return false;
}

void CairoCalibration::setImage(cv::Mat image, cv::Mat image_binary){
    if (binary_image){
       opencv_image = image_binary;
    } else {
       opencv_image = image;
    }
    queue_draw();
}

Point CairoCalibration::getPixelColor(){
    return changeCordinates(pixel_color, cairo_image_size, opencv_image_size);
}