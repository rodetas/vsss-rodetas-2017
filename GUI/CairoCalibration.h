#ifndef CAIRO_CALIBRATION_H_
#define CAIRO_CALIBRATION_H_

#include "../Header.h"
#include "../utils/Structs.h"
#include "../utils/OpenCV.h"
#include <gtkmm.h>

using namespace rodetas;

class CairoCalibration : public Gtk::DrawingArea {

private:
    cv::Mat opencv_image;
    Point cairo_image_size;
    Point opencv_image_size;
    Point pixel_color;

    bool binary_image;

protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &c);
    virtual bool on_button_press_event(GdkEventButton* event);
    virtual bool on_key_press_event(GdkEventKey* key_event);

public:
	CairoCalibration();
    void setImage(cv::Mat, cv::Mat);
    Point getPixelColor();
};

#endif