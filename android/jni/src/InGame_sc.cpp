#include "all.h"

vector<Button::Ptr>	myCBtnsp,myCBtnsl;
Grid::Ptr	myCBgrp,myCBgrl;
VerticalLayout::Ptr	vl1;
HorizontalLayout::Ptr	Hollywood;

Label::Ptr gmLb,hud1;

void resetGameGui()
{
	int i=0;
	myCBgrp->removeAllWidgets();
	for(auto w:myCBtnsp)
	{
	//	myCBgrp->add(w);
		myCBgrp->addWidget(w,i/3,i%3	/*,Borders(1.5)*/);
		i++;
	}
	myCBgrp->updateWidgets();

	
	myCBgrl->removeAllWidgets();
	i=0;
	for(auto w:myCBtnsl)
	{
	//	myCBgrl->add(w);
		myCBgrl->addWidget(w,i/7,i%7);
		i++;
	}
	myCBgrl->updateWidgets();
	
	
	for(auto &w:Hollywood->getWidgets())
		w->show();
//	myCBgrp->getWidgets()
	vl1->setRatio(myCBgrp,2.2);
}




void removeCBtn(int i)
{
	printf("I got %d\n",i);
	myCBgrp->remove(myCBtnsp[i]);
	myCBgrl->remove(myCBtnsl[i]);
}




void buildGameGui()
{
	
	int i=0;
	for(char ch='A';ch<='Z';ch++)
	{
		if(strchr("AEIOU",ch))
			continue;
		
		Button::Ptr	bt=Button::create(widget_names[0]);
		bt->setText(ch);
		bt->setSize(bindWidth(myCBgrp)/3,bindHeight(myCBgrp)/7);
		bt->connect("clicked",[=]()
				{
					removeCBtn(i);
					tryAnswer(ch);
				}	);

		myCBtnsp.push_back(bt);
		
		bt=Button::copy(bt);
		bt->setSize(bindWidth(myCBgrl)/7,bindHeight(myCBgrl)/3);
		myCBtnsl.push_back(bt);
		
		i++;
	}
	
	resetGameGui();
}





void drFrame(Scene *sc)
{
	showStatus();
}





Scene *buildInGame(sf::RenderWindow &window)
{
	Gui *gp=new Gui(window);
	gp->setGlobalFont(font_names[0]);
	Scene *sc=new Scene(&window,gp);
	
	myCBgrp=Grid::create();
	myCBgrl=Grid::create();	
	
	vl1=VerticalLayout::create();
	auto vl2=VerticalLayout::create();
		
	vl1->setPosition(bindWidth(*gp)*0.1,bindHeight(*gp)*0.02);
	vl1->setSize(bindWidth(*gp)*0.8,bindHeight(*gp)*0.96);
	
	
	
	gmLb=Label::create(widget_names[0]);

	gmLb->setText("ABCD");
	gmLb->setTextColor(sf::Color(255,155,0,255));
	gmLb->setBackgroundColor(sf::Color(0,0,0,155));
	gmLb->setTextSize(35*((480*800)/(window.getSize().x * window.getSize().y)));
	gmLb->setTextStyle(sf::Text::Bold);

	
	hud1=Label::create(widget_names[0]);

	hud1->setText("AB\nC\nD");
	hud1->setTextColor(sf::Color(255,0,0,255));
	hud1->setBackgroundColor(sf::Color(0,0,255,155));
	hud1->setTextSize(20*((480*800)/(window.getSize().x * window.getSize().y)));
	
	
	Hollywood=HorizontalLayout::create();
	
	for(int i=0;i<9;i++)
	{
		Button::Ptr	bt=Button::create(widget_names[0]);
		bt->setText("HOLLYWOOD"[i]);
		Hollywood->add(bt);
	}
	
	vl1->add(Hollywood);
	vl1->add(hud1);
	vl1->add(gmLb);
	vl1->add(myCBgrp);vl1->addSpace(.1);
	vl1->add(makeButton("Back", [&](){ setScene(menu); }));
	
	vl1->setRatio(Hollywood,.2);
	vl1->setRatio(hud1,.52);
	vl1->setRatio(gmLb,1.2);
	vl1->setRatio(myCBgrp,2.2);
	vl1->setRatio(5,.2);
	
	for(int i=vl1->getWidgets().size(); --i>0;)
		vl1->insertSpace(i,.15);
	

//	MessageBox::Ptr	msg=MessageBox::create(widget_names[0]);
	
//	vl1->add(myCBgrl);
	gp->add(vl1);
	
	gp->setGlobalFont(font_names[0]);
/*	vl2->setPosition(bindWidth(*gp)*0.2,bindHeight(*gp)*0.2);
	vl2->setSize(bindWidth(*gp)*0.6,bindHeight(*gp)*0.6);
	

	for(int i=vl2->getWidgets().size(); --i>0;)
		vl2->insertSpace(i,.3);
	
	vl2->hide();
	*/
//	gp->add(vl2);
	
/*	
	auto picPortrait =std::make_shared<anim>();
	picPortrait->loadFromDir("db/anim0/",milliseconds(10));
	
	picPortrait->setLooping(true);
	picPortrait->play();
	picPortrait->setSmooth();
	
	picPortrait->setSize(bindSize(*gp));
	gp->add(picPortrait, "BKGDPortrait");
	picPortrait->moveToBack();
*/
	gp->add(pBkgd);
	pBkgd->moveToBack();
	vl1->connect("SizeChanged",[=](){	pBkgd->setSize(gp->getSize());	});

	buildGameGui();
	myCBgrp->setTransparency(230);
	sc->evF=BasicEvp;
	sc->dsF=drFrame;
#ifndef SFML_SYSTEM_ANDROID
	sc->continuousRedraw=1;
	sc->drawfrq=1;
#endif
	return sc;
}
