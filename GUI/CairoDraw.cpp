#include "CairoDraw.h"

CairoDraw::CairoDraw(){
    robot.resize(number_robot);
    colors_rgb.resize(6);

    colors_rgb = manipulation.getColorsRgbCairo();
}

void CairoDraw::setPosition(vector<rod::Object> o){
    
    for (int i = 0; i < robot.size(); i++){
        robot[i].x = o[i].x;
        robot[i].y = o[i].y;
        //r[i].angle = o[i].angle;
    }

    queue_draw();
}

bool CairoDraw::on_draw (const Cairo::RefPtr<Cairo::Context> &c){
      
        Gtk::Allocation allocation = get_allocation();
            field_size.x = allocation.get_width();
            field_size.y = allocation.get_height();

            goal_size.x = field_size.x*(0.05);
            goal_size.y = field_size.y*(0.3);

        color_team_size = field_size.x * 0.05;
        color_player_size = field_size.x * 0.02;
        int line_field_width = 4;
        int corner_size = field_size.x*(0.05);

    //draw field        
        c->move_to(line_field_width                 ,  field_size.y/2);
        c->line_to(line_field_width                 ,  field_size.y/2 - goal_size.y/2);
        c->line_to(goal_size.x                      ,  field_size.y/2 - goal_size.y/2);
        c->line_to(goal_size.x                      ,  line_field_width);
        c->line_to(field_size.x - goal_size.x       ,  line_field_width);
        c->line_to(field_size.x - goal_size.x       ,  field_size.y/2 - goal_size.y/2);
        c->line_to(field_size.x - line_field_width  ,  field_size.y/2 - goal_size.y/2);
        c->line_to(field_size.x - line_field_width  ,  field_size.y/2 + goal_size.y/2);
        c->line_to(field_size.x - goal_size.x       ,  field_size.y/2 + goal_size.y/2);
        c->line_to(field_size.x - goal_size.x       ,  field_size.y - line_field_width);
        c->line_to(goal_size.x                      ,  field_size.y - line_field_width);
        c->line_to(goal_size.x                      ,  field_size.y/2 + goal_size.y/2);
        c->line_to(line_field_width                 ,  field_size.y/2 + goal_size.y/2);
        c->close_path();
    //center line
        c->move_to(field_size.x/2 , line_field_width);
        c->line_to(field_size.x/2 , field_size.y - line_field_width);
        c->begin_new_sub_path();
        c->arc(field_size.x/2 , field_size.y/2, field_size.y*(0.1), 0, 2*M_PI);
    //left goal area
        c->move_to(goal_size.x     , field_size.y/2 - int((goal_size.y/2)*1.5));
        c->line_to(goal_size.x*(2) , field_size.y/2 - int((goal_size.y/2)*1.5));
        c->line_to(goal_size.x*(2) , field_size.y/2 + int((goal_size.y/2)*1.5));
        c->line_to(goal_size.x     , field_size.y/2 + int((goal_size.y/2)*1.5));
        c->close_path();
        c->begin_new_sub_path();
        c->arc(goal_size.x*(2) , field_size.y/2, goal_size.y*(0.2), 3*M_PI/2, M_PI/2);
    //right goal area
        c->move_to(field_size.x - goal_size.x     , field_size.y/2 - int((goal_size.y/2)*1.5));
        c->line_to(field_size.x - goal_size.x*(2) , field_size.y/2 - int((goal_size.y/2)*1.5));
        c->line_to(field_size.x - goal_size.x*(2) , field_size.y/2 + int((goal_size.y/2)*1.5));
        c->line_to(field_size.x - goal_size.x     , field_size.y/2 + int((goal_size.y/2)*1.5));
        c->close_path();
        c->begin_new_sub_path();
        c->arc(field_size.x - goal_size.x*(2) , field_size.y/2, goal_size.y*(0.2), M_PI/2, 3*M_PI/2);
    //corners
        c->move_to(goal_size.x                              , corner_size);
        c->line_to(goal_size.x + corner_size                , line_field_width);
        c->move_to(field_size.x - goal_size.x               , corner_size);
        c->line_to(field_size.x - goal_size.x - corner_size , line_field_width);
        c->move_to(goal_size.x                              , field_size.y - corner_size);
        c->line_to(goal_size.x + corner_size                , field_size.y - line_field_width);
        c->move_to(field_size.x - goal_size.x               , field_size.y - corner_size);
        c->line_to(field_size.x - goal_size.x - corner_size , field_size.y - line_field_width);
    //faults X
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x , field_size.y*(0.2) - corner_size/4);
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x , field_size.y*(0.2) + corner_size/4);
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + corner_size/4, field_size.y*(0.2));
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x - corner_size/4, field_size.y*(0.2));
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x , field_size.y/2 - corner_size/4);
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x , field_size.y/2 + corner_size/4);
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + corner_size/4, field_size.y/2);
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x - corner_size/4, field_size.y/2);
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x , field_size.y*(0.8) - corner_size/4);
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x , field_size.y*(0.8) + corner_size/4);
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + corner_size/4, field_size.y*(0.8));
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x - corner_size/4, field_size.y*(0.8));
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 , field_size.y*(0.2) - corner_size/4);
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 , field_size.y*(0.2) + corner_size/4);
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 + corner_size/4, field_size.y*(0.2));
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 - corner_size/4, field_size.y*(0.2));
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 , field_size.y/2 - corner_size/4);
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 , field_size.y/2 + corner_size/4);
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 + corner_size/4, field_size.y/2);
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 - corner_size/4, field_size.y/2);
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 , field_size.y*(0.8) - corner_size/4);
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 , field_size.y*(0.8) + corner_size/4);
        c->move_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 + corner_size/4, field_size.y*(0.8));
        c->line_to((field_size.x/2 - goal_size.x)/2 + goal_size.x + field_size.x/2 - corner_size/4, field_size.y*(0.8));

    //configure colors, lines and fills
        c->set_source_rgb(0, 0, 0);
        c->set_line_width(line_field_width); 
        c->stroke ();

    //draw robots
        c->set_line_width(2);        
        for (int i = 0; i < 3; i++) {

            Point r = { changeCoordinate(robot[i]).x, changeCoordinate(robot[i]).y };
            
            // team colors rectangle        
            c->save();
                c->rectangle(r.x, r.y, color_team_size, color_team_size);
                c->set_source_rgb(colors_rgb[3].r, colors_rgb[3].g, colors_rgb[3].b);
                c->fill_preserve();
            c->restore();
            c->stroke ();
            
            // player colors rectangle            
            c->save();
                c->rectangle(r.x, r.y, color_player_size, color_player_size);    
                c->set_source_rgb(colors_rgb[i].r, colors_rgb[i].g, colors_rgb[i].b);
                c->fill_preserve();
            c->restore();
            c->stroke ();
        }

         for (int i = 3; i < 6; i++) {

            Point r = { changeCoordinate(robot[i]).x, changeCoordinate(robot[i]).y };
            
            // team colors rectangle        
            c->save();
                c->rectangle(r.x, r.y, color_team_size, color_team_size);
                c->set_source_rgb(colors_rgb[4].r, colors_rgb[4].g, colors_rgb[4].b);
                c->fill_preserve();
            c->restore();
            c->stroke ();
        }


    
        return true;
    }