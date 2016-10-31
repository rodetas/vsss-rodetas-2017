#ifndef FIELD_H_
#define FIELD_H_

#include "../Header.h"
#include "../utils/Structs.h"

#include <gtkmm.h>

class Cairo_Robot : public Gtk::DrawingArea {

private:
    Robot_Draw r;

protected:
    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &c){
        r.x = 200;
        r.y = 200;
        r.angle = 50;

        double color_team_size = 50;
        double color_player_size = 20;

        // defines the position and angle on the field
        c->translate(r.x, r.y);
        c->rotate_degrees(r.angle);
        c->translate(-color_team_size/2 , -color_team_size/2 );

        // team colors rectangle        
        c->save();
            c->rectangle(0, 0, color_team_size, color_team_size);
            c->set_source_rgb(r.color_team.r, r.color_team.g, r.color_team.b);
            c->fill_preserve();
        c->restore();    
        c->stroke ();
        
        // player colors rectangle            
        c->save();
            c->rectangle(0, 0, color_player_size, color_player_size);    
            c->set_source_rgb(r.color_player.r, r.color_player.g, r.color_player.b);
            c->fill_preserve();
        c->restore();
        c->stroke ();

        return true;
    }

public:
	Cairo_Robot(){
    }
};

#endif