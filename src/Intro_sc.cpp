






#include "all.h"




Scene *buildIntro(sf::RenderWindow &window)
{
	Gui *gp=new Gui(window);
	gp->setGlobalFont(font_names[0]);
	Scene *sc=new Scene(&window,gp);
	
	
	
	auto pintro=std::make_shared<anim>();
/*	pintro->addFrame("db/anim1/intro1.jpg",milliseconds(100));
	pintro->addFrame("db/anim1/intro2.jpg",milliseconds(500));
	pintro->addFrame("db/anim1/intro3.jpg",milliseconds(500));
	pintro->addFrame("db/anim1/intro4.jpg",milliseconds(500));
	pintro->addFrame("db/anim1/intro5.jpg",milliseconds(500));
	pintro->addFrame("db/anim1/intro6.jpg",milliseconds(500));
	pintro->addFrame("db/anim1/intro7.jpg",milliseconds(500));
	pintro->addFrame("db/anim1/intro3.jpg",milliseconds(500));
*/
	pintro->loadFromDir("db/anim1/",milliseconds(2500));
	
	
//	pintro->setLooping(1);
	pintro->setSize(bindSize(*gp));
	pintro->setSmooth();
	
//	pintro->setSize(bindWidth(*gp)*1.8,bindHeight(*gp)*1.2);
//	vl1->add(pintro);
	gp->add(pintro,"introAnim");
	pintro->setSize(bindSize(*gp));
	pintro->connect("Clicked",[=]()
		{
			if(pintro->isPlaying())
				return;
			if(IamLoggedIn)
				setScene(menu);
			else
				setScene(lin);
		});
	
//	gp->add(vl1);
	pintro->connect("Focused",[=](){ pintro->play();	});
	
	return sc;
}
