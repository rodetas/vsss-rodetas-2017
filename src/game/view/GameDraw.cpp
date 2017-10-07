#include "GameDraw.h"

GameDraw::GameDraw(){
    colors_rgb = manipulation.getColorsRgbCairo();
    image_size = manipulation.getImageSize();
    goal_size  = manipulation.getGoal();
}

GameDraw::~GameDraw(){
}

void GameDraw::setPosition(vector<Robot> _team, vector<Robot> _opponent, Ball _ball){    
    robot_team = _team;
    robot_opponent = _opponent;
    ball = _ball;

    queue_draw();
}

bool GameDraw::on_draw (const Cairo::RefPtr<Cairo::Context> &c){

    try{
        Gtk::Allocation allocation = get_allocation();
            field_size.x = allocation.get_width();
            field_size.y = allocation.get_height();

            goal_size.x = field_size.x*(0.05);
            goal_size.y = field_size.y*(0.3);

        color_team_size = field_size.x * 0.03;
        color_player_size = field_size.x * 0.01;
        ball_size = field_size.x * 0.008;
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
        
    /*** TEAM ***/
        for (int i = 0; i < robot_team.size(); i++) {
            
            if (!robot_team[i].isNull()) {

                Point r = { changeCoordinate(robot_team[i].getPosition()).x, changeCoordinate(robot_team[i].getPosition()).y };
                // team colors rectangle        
                c->save();
                    c->translate(r.x + color_team_size/2, r.y + color_team_size/2);
                    c->rotate_degrees(robot_team[i].getAngle() - 90);
                    c->translate(-r.x - color_team_size/2, -r.y - color_team_size/2);
                    c->rectangle(r.x, r.y, color_team_size, color_team_size);
                    c->set_source_rgb(colors_rgb[TEAM].r, colors_rgb[TEAM].g, colors_rgb[TEAM].b);
                    c->fill_preserve();
                c->restore();
                c->stroke ();
                
                // player colors rectangle            
                c->save();
                    c->translate(r.x + color_team_size/2, r.y + color_team_size/2);
                    c->rotate_degrees(robot_team[i].getAngle() - 90);
                    c->translate(-r.x - color_team_size/2, -r.y - color_team_size/2);
                    c->rectangle(r.x, r.y, color_player_size, color_player_size);    
                    c->set_source_rgb(colors_rgb[i].r, colors_rgb[i].g, colors_rgb[i].b);
                    c->fill_preserve();
                    
                c->restore();
                c->stroke ();
            }
        }
    
    /*** OPPONENT ***/
        for (int i = 0; i < robot_opponent.size(); i++) {

            if (!robot_opponent[i].isNull()) {

                Point r = { changeCoordinate(robot_opponent[i].getPosition()).x, changeCoordinate(robot_opponent[i].getPosition()).y };
                    
                // opponent colors rectangle        
                c->save();
                    c->rectangle(r.x, r.y, color_team_size, color_team_size);
                    c->set_source_rgb(colors_rgb[OPPONENT].r, colors_rgb[OPPONENT].g, colors_rgb[OPPONENT].b);
                    c->fill_preserve();
                c->restore();
                c->stroke ();
            }
        }

    /*** BALL ***/
        if (!ball.isNull()) {    

            Point r = { changeCoordinate(ball.getPosition()).x, changeCoordinate(ball.getPosition()).y };

            // ball
            c->save();
                c->arc(r.x, r.y, ball_size, 0, 2*CV_PI);
                c->set_source_rgb(colors_rgb[BALL].r, colors_rgb[BALL].g, colors_rgb[BALL].b);
                c->fill_preserve();
            c->restore();
            c->stroke ();
        }

    } catch(const std::exception& ex) {
        cout << "EXCEPTION: " << ex.what() << "in GameDraw:on_draw " << endl;
    }

    return true;
}
