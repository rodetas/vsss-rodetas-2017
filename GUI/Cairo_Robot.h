#ifndef FIELD_H_
#define FIELD_H_

#include "../Header.h"
#include "../utils/Structs.h"

#include <gtkmm.h>

class Cairo_Robot : public Gtk::DrawingArea {

private:
    const int number_robot = 6;
    int color_team_size;
    int color_player_size;

    vector<rod::Object> robot;
    Point field_size;
    Point image_size = {640,480};
    Point goal_size  = {50,100};
    
    template <typename typePoint>
    typePoint changeCoordinate(typePoint point){
        point.x = float(point.x) * float(field_size.x) / float(image_size.x);
        point.y = float(point.y) * float(field_size.y) / float(image_size.y);

        return point;
    }

protected:

    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &c){
      
        Gtk::Allocation allocation = get_allocation();
            field_size.x = allocation.get_width();
            field_size.y = allocation.get_height();

        color_team_size = field_size.x * 0.05;
        color_player_size = field_size.x * 0.02;

        for (int i = 0; i < robot.size(); i++) {

            Point r = { changeCoordinate(robot[i]).x, changeCoordinate(robot[i]).y };
            
            // team colors rectangle        
            c->save();
                c->rectangle(r.x, r.y, color_team_size, color_team_size);
                c->set_source_rgb(robot[i].color_team.r, robot[i].color_team.g, robot[i].color_team.b);
                c->fill_preserve();
            c->restore();
            c->stroke ();
            
            // player colors rectangle            
            c->save();
                c->rectangle(r.x, r.y, color_player_size, color_player_size);    
                c->set_source_rgb(robot[i].color_player.r, robot[i].color_player.g, robot[i].color_player.b);
                c->fill_preserve();
            c->restore();
            c->stroke ();
        }
        
        // draw field
        c->save();
            c->set_source_rgb(0, 0.0, 1.0);
            c->set_line_width(15.0);
        
            c->move_to(0, field_size.y/2);
            c->line_to(0,  goal_size.y/2);
            c->line_to(goal_size.x, goal_size.y/2);
            c->line_to(goal_size.x, 0);
            c->line_to(field_size.x - goal_size.x, 0);
            c->line_to(field_size.x - goal_size.x, field_size.y - goal_size.y/2);
            c->line_to(field_size.x, field_size.y - goal_size.y/2);
            c->line_to(field_size.x, goal_size.y/2);
            //c->line_to(0, 0);

        c->restore();
        c->stroke ();

        return true;
    }

public:

	//Cairo_Robot(rod::Object o_calibrated){
	Cairo_Robot(){
        robot.resize(number_robot);
        //r = r_calibrated;
        //field_size
    }

    void setPosition(vector<rod::Object> o){
        
        for (int i = 0; i < robot.size(); i++){
            robot[i].x = o[i].x;
            robot[i].y = o[i].y;
            //r[i].angle = o[i].angle;
        }

        queue_draw();
    }
};

#endif