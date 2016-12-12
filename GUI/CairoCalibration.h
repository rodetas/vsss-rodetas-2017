#ifndef CAIRO_CALIBRATION_H_
#define CAIRO_CALIBRATION_H_

#include "../Header.h"
#include "../utils/Structs.h"
#include "../utils/OpenCV.h"
#include <gtkmm.h>

using namespace rodetas;

class CairoCalibration : public Gtk::DrawingArea {

private:
    cv::Mat image;
    Point field_size;

protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &c);
    virtual bool on_button_press_event(GdkEventButton* event);

public:
	CairoCalibration();
    void setImage(cv::Mat);
};

#endif