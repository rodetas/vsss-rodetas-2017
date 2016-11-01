#ifndef FIELD_H_
#define FIELD_H_

#include "../Header.h"
#include "../utils/Structs.h"

#include <gtkmm.h>

class Cairo_Robot : public Gtk::DrawingArea {

private:
    Robot_Draw r;

protected:

    sigc::connection m_connection_timeout;

    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &c){
      
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

     bool on_timeout(){

        //r.angle += 2;
        queue_draw();

        return true;
    }

	Cairo_Robot(){
        r.x = 200;
        r.y = 200;
        r.angle = 50;

        m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Cairo_Robot::on_timeout), 50 );
        //m_connection_timeout = signal_on_show().connect( sigc::mem_fun(this, &Cairo_Robot::on_timeout));

    }

    void setNaoSei(int i){
        r.angle += i;
    }

};

#endif