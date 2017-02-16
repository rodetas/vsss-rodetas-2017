#include "DrawAreaCalibration.h"

DrawAreaCalibration::DrawAreaCalibration(){   
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    add_events(Gdk::BUTTON3_MOTION_MASK);
    
     
    pixel_color = {0,0};
    rectangle_size = {0, 0};
    rectangle_point = {0, 0};
    opencv_image = cv::Mat::zeros(1, 1, CV_64F);
}

bool DrawAreaCalibration::on_draw (const Cairo::RefPtr<Cairo::Context> &cairo_draw){

    Gtk::Allocation allocation = get_allocation();
        cairo_image_size.x = allocation.get_width();
        cairo_image_size.y = allocation.get_height();
        opencv_image_size  = opencv_image.size();
    
    cv::Mat opencv_image_resize;
    cv::resize(opencv_image, opencv_image_resize, cairo_image_size, 0, 0, cv::INTER_LINEAR);

    Glib::RefPtr<Gdk::Pixbuf> pixbuf =  Gdk::Pixbuf::create_from_data( opencv_image_resize.data, Gdk::COLORSPACE_RGB, false, 8, opencv_image_resize.cols, opencv_image_resize.rows, opencv_image_resize.step);
    Gdk::Cairo::set_source_pixbuf(cairo_draw, pixbuf);
    cairo_draw->paint();

    cairo_draw->set_source_rgb(1,1,1);
    cairo_draw->set_line_width(3); 
    cairo_draw->rectangle(rectangle_point.x, rectangle_point.y, rectangle_size.x, rectangle_size.y);    
    cairo_draw->stroke();

    return true;
}

void DrawAreaCalibration::setImage(cv::Mat image){
    opencv_image = image;
    queue_draw();
}

Point DrawAreaCalibration::getCairoImageSize(){
    return cairo_image_size;
}

// Mouse button press event
bool DrawAreaCalibration::on_button_press_event(GdkEventButton* event) {
    if(event->button == 3) {
        rectangle_point = {event->x, event->y};
    } 
    return true;
}

bool DrawAreaCalibration::on_button_release_event (GdkEventButton* event){
    if(event->button == 3) {
        rectangle_size = {event->x - rectangle_point.x, event->y - rectangle_point.y};
    }
    return true;
}

bool DrawAreaCalibration::on_motion_notify_event (GdkEventMotion* event){
    rectangle_size = {event->x - rectangle_point.x, event->y - rectangle_point.y};
    queue_draw(); 
}