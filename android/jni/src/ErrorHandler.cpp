
#include "all.h"


//TextBox::Ptr	err_text;
ChatBox::Ptr	err_text;

Button::Ptr	ok;




Scene *buildErrHandle(sf::RenderWindow &window)
{
	Gui *gp=new Gui(window);
	Gui *gl=new Gui(window);
	gl->setGlobalFont(font_names[0]);
	gp->setGlobalFont(font_names[0]);
	
	Scene *sc=new Scene(&window,gp);
	
//	err_text=TextBox::create(widget_names[0]);
	err_text=ChatBox::create(widget_names[0]);
	
	err_text->setPosition(bindWidth(*gp)*0.1,bindHeight(*gp)*0.1);
	err_text->setSize(bindWidth(*gp)*0.8,bindHeight(*gp)*0.7);
//	err_text->setReadOnly();
	err_text->setTextSize(25*((480*800)/(window.getSize().x * window.getSize().y)));
	
	ok=Button::create();
	
	ok->setPosition(bindLeft(err_text),bindBottom(err_text)*1.05);
	ok->setSize(bindWidth(err_text),bindHeight(err_text)*0.1);
	
	
	
	gp->add(err_text);
	gp->add(ok);
	
	return sc;
}




