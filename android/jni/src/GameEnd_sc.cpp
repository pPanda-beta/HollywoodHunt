
#include "all.h"
TextBox::Ptr	FinalMsg;


Scene *buildEndGame(sf::RenderWindow &window)
{
	Gui *gp=new Gui(window);
	gp->setGlobalFont(font_names[0]);
	Scene *sc=new Scene(&window,gp);
	
	
	auto vl1=VerticalLayout::create();
	auto vl2=VerticalLayout::create();
		
	vl1->setPosition(bindWidth(*gp)*0.1,bindHeight(*gp)*0.02);
	vl1->setSize(bindWidth(*gp)*0.8,bindHeight(*gp)*0.96);
	
	FinalMsg=TextBox::create(widget_names[0]);
	
	FinalMsg->addText("safdsf");
	FinalMsg->setTextSize(35*((480*800)/(window.getSize().x * window.getSize().y)));
	FinalMsg->setReadOnly();
	vl1->add(FinalMsg);
	
	gp->add(vl1);
	
	
	vl1->add(makeButton("Main Menu", [&](){ setScene(menu); }));
	
	
	vl1->setRatio(FinalMsg,5);
	
	for(int i=vl1->getWidgets().size(); --i>0;)
		vl1->insertSpace(i,.15);
	
	auto picPortrait = tgui::Picture::create("db/Background-Portrait.png");
	picPortrait->setSize(bindSize(*gp));
	gp->add(picPortrait, "BKGDPortrait");
	picPortrait->moveToBack();
	
	
	
	return sc;
}




