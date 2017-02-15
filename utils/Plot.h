#ifndef PLOT_H_
#define PLOT_H_

#include "../Header.h"
#include "../GUI/Cairo/DrawAreaPlot.h"
#include <gtkmm.h>


class Plot {

private:
	DrawAreaPlot graph;

	int i = 0;

public:
	Plot(string type, float y_max = 0, float y_min = 0, float x_max = 0, float x_min = 0);
	int GUI();
	bool updateData();
};

#endif