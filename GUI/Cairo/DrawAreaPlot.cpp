#include "DrawAreaPlot.h"

DrawAreaPlot::DrawAreaPlot(){
    n_points_screen = 80;
    position = 0;
}

bool DrawAreaPlot::on_draw (const Cairo::RefPtr<Cairo::Context> &cr){

    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    cr->scale(width, height);
    cr->set_line_width(0.005);

    for (int i = 0; i < y.size(); i++){
        cr->line_to(x[i], y[i]*0.5);
        //cr->rel_curve_to(x[i], y[i]*0.2, x[i+1], y[i+1]*0.2, x[i+2], y[i+2]*0.2);
    }
    cr->stroke();
    
    return true;
}

void DrawAreaPlot::initializeTemporalGraph(float y_max, float y_min){
    max.y = y_max;
    min.y = y_min;

    for (float i = 0; i < n_points_screen; i++){
        x.push_back(i/n_points_screen);
    }
}

void DrawAreaPlot::initializeNoTemporalGraph(float y_max, float y_min, float x_max, float x_min){
    max = {x_max, y_max};
    min = {x_min, y_min};    
}

void DrawAreaPlot::updateTemporalGraph(float y_new){
   
    if (y.size() == n_points_screen){
        y.push_back( (1 - (y_new/max.y)) );
        y.pop_front();
    } else {
        y.push_back( (1 - (y_new/max.y)) );
    }
    queue_draw();
}

void DrawAreaPlot::updateNoTemporalGraph(float x_new, float y_new){
   
    if (y.size() == n_points_screen){
        //x.push_back( x_new/max.x );         x.pop_front();
        //y.push_back( 1 - (y_new/max.y) );   y.pop_front();
    } else {
        x.push_back( x_new/max.x );
        y.push_back( 1 - (y_new/max.y) );
    }
    queue_draw();
}