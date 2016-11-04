#ifndef FIELD_H_
#define FIELD_H_

#include "../Header.h"
#include "../utils/Structs.h"

#include <gtkmm.h>

class Cairo_Robot : public Gtk::DrawingArea {

private:
    vector<rod::Object> robot;
    const int number_robot = 6;

protected:

    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &c){
      
        double color_team_size = 40;
        double color_player_size = 15;

        for (int i = 0; i < robot.size(); i++) {
            
            // defines the position and angle on the field
            //c->translate(r[i].x, r[i].y);
            //c->rotate_degrees(r[i].angle);
            //c->translate(-color_team_size/2 , -color_team_size/2 );

            // team colors rectangle        
            c->save();
                c->rectangle(robot[i].x, robot[i].y, color_team_size, color_team_size);
                c->set_source_rgb(robot[i].color_team.r, robot[i].color_team.g, robot[i].color_team.b);
                c->fill_preserve();
            c->restore();
            c->stroke ();
            
            // player colors rectangle            
            c->save();
                c->rectangle(robot[i].x, robot[i].y, color_player_size, color_player_size);    
                c->set_source_rgb(robot[i].color_player.r, robot[i].color_player.g, robot[i].color_player.b);
                c->fill_preserve();
            c->restore();
            c->stroke ();
        }

        return true;
    }

public:

	//Cairo_Robot(rod::Object o_calibrated){
	Cairo_Robot(){
        robot.resize(number_robot);
        //r = r_calibrated;
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