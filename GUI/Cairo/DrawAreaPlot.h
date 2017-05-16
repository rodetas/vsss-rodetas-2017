#ifndef DRAW_AREA_PLOT_H_
#define DRAW_AREA_PLOT_H_

#include "../../Header.h"
#include "../../utils/Structs.h"
#include <gtkmm.h>

using namespace rodetas;

class DrawAreaPlot : public Gtk::DrawingArea {

private:
    Point2i max;
    Point2i min;
    deque<float> x;
    deque<float> y;
    
    int n_points_screen;
    int position;

protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &c);

public:
	DrawAreaPlot();
    void initializeTemporalGraph(float, float);
    void initializeNoTemporalGraph(float, float, float, float);
    void updateTemporalGraph(float);
    void updateNoTemporalGraph(float, float);
};

#endif