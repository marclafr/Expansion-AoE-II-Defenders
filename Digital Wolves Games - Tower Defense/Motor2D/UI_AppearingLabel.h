#ifndef __UI_APPEARING_LABEL_H__
#define __UI_APPEARING_LABEL_H__

#include "j1Timer.h"
#include "UI_Label.h"

class UI_AppearingLabel : public UI_Label
{
private:
	j1Timer time_on_screen;
};

#endif //__UI_APPEARING_LABEL_H__