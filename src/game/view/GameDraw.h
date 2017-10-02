#ifndef DRAW_AREA_CONTROL_H_
#define DRAW_AREA_CONTROL_H_

#include "../../Header.h"
#include "../../utils/Ball.h"
#include "../../utils/Robot.h"
#include "../../utils/Commons.h"
#include "../../utils/Manipulation.h"

#include <gtkmm.h>

class GameDraw : public Gtk::DrawingArea {

private:
    int color_team_size;
    int color_player_size;
    int ball_size;

    Manipulation manipulation;

    Ball ball;
	vector<Robot> robot_team;
    vector<Robot> robot_opponent;
    
    vector<rodetas::Rgb> colors_rgb;

    Point field_size;
    Point image_size;
    Point goal_size;
    
    template <typename typePoint>
    typePoint changeCoordinate(typePoint point){
        point.x = float(point.x) * float(field_size.x) / float(image_size.x);
        point.y = float(point.y) * float(field_size.y) / float(image_size.y);

        return point;
    }

protected:

    bool on_draw (const Cairo::RefPtr<Cairo::Context> &c) override;

public:

    GameDraw();
    virtual ~GameDraw();
    void setPosition(vector<Robot>, vector<Robot>, Ball);
};

#endif