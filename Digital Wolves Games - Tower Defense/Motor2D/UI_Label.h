#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#include "j1Gui.h"

class UI_Image;
class UI_Text;

class UI_Label : public UI_Element
{
private:
	UI_Image* img;
	UI_Text* text;
};

#endif //__UI_LABEL_H__