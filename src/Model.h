#ifndef MODEL_H_
#define MODEL_H_

#include "Header.h"

class View;
class Model {

public:
	Model();
    ~Model();
    
    void setCaller(View*);
    
private:
    View* caller;
};

#include "View.h"

#endif