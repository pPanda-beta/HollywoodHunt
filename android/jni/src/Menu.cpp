



#include "all.h"





Scene *buildMenu(sf::RenderWindow &window)
{
	Gui *gp=new Gui(window);
	gp->setGlobalFont(font_names[0]);
	Scene *sc=new Scene(&window,gp);
	
	
	auto vl1=VerticalLayout::create();
	auto vl2=VerticalLayout::create();
	auto about=Picture::create("db/Slide5.png");
	
	vl1->setPosition(bindWidth(*gp)*0.2,bindHeight(*gp)*0.05);
	vl1->setSize(bindWidth(*gp)*0.6,bindHeight(*gp)*0.9);
	
	vl1->add(makeButton("New Game", [=](){ vl2->show();	vl1->hide(); }));
	vl1->add(makeButton("High Scores",[&](){ refreshScores();	setScene(hscore_sc); }));
	vl1->add(makeButton("Log Out", [&]()
		{g
			logout();g

		}));


	vl1->add(makeButton("My Names",[&](){ refresh_list(); setScene(myNames_sc); }));
	vl1->add(makeButton("Exit", [=](){ about->show(); }));
	
	
	for(int i=vl1->getWidgets().size(); --i;)
		vl1->insertSpace(i,.3);
	
	gp->add(vl1);
	
	
	gp->setGlobalFont(font_names[0]);
	vl2->setPosition(bindWidth(*gp)*0.2,bindHeight(*gp)*0.2);
	vl2->setSize(bindWidth(*gp)*0.6,bindHeight(*gp)*0.6);
	
	vl2->add(makeButton("Easy", [&](){  startNewGame(0);   }));
	vl2->add(makeButton("Medium", [&](){ startNewGame(1);    }));
	vl2->add(makeButton("Hard", [&](){ startNewGame(2);    }));
	vl2->add(makeButton("Expert", [&](){ startNewGame(3);   }));
	vl2->add(makeButton("Back", [=](){ vl1->show();	vl2->hide(); }));
	
	for(int i=vl2->getWidgets().size(); --i>0;)
		vl2->insertSpace(i,.3);
	
	vl2->hide();
	
	gp->add(vl2);
	
	vl1->setTransparency(150);
	vl2->setTransparency(200);
	
	
	
	about->hide();
	about->setSize(bindSize(*gp));
	about->connect("Clicked",[](){ extern Thread	t1; t1.terminate(); exit(0); });
	gp->add(about,"about");
//	std::shared_ptr<MessageBox> msg=MessageBox::create(widget_names[0]);
	
	
	
	auto picPortrait =std::make_shared<anim>();
	picPortrait->loadFromDir("db/anim5/",milliseconds(100));
	
	picPortrait->setLooping(true);
	picPortrait->play();
	picPortrait->setSmooth();
	
	picPortrait->setSize(bindSize(*gp));
	gp->add(picPortrait, "BKGDPortrait");
	picPortrait->moveToBack();

	sc->evF=BasicEvp;
	return sc;
}
