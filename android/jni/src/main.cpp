


#include "all.h"
vector<string>	widget_names={"db/widgets/Black.conf"};
vector<string>	font_names={"db/fonts/DejaVuSans.ttf","db/fonts/simplex_.ttf","db/fonts/complex_.ttf"};

Scene *err_sc,*lin,*menu,*hscore_sc,*about_sc,*myNames_sc,*gm_play_sc,*gm_end_sc,*intro;




void BasicEvp(Scene *sc)
{
	switch(sc->ev->type)
	{
		case	Event::Closed	:
							
							break;
		case	Event::Resized	:
							
							
							sc->wd=(sc->ev->size.width);
							sc->ht=(sc->ev->size.height);
							
							break;
		case	Event::TouchBegan:
							
							
						//	break;
		
		case	Event::TouchMoved:
							
						//	break;
		case	Event::TouchEnded:
						//	sc->ev->touch.x*= sc->win->getDefaultView().getSize().x / sc->wd.getValue();
						//	sc->ev->touch.y*= sc->win->getDefaultView().getSize().y / sc->ht.getValue();
							
							break;
		
	}
}




std::shared_ptr<anim>	pBkgd,lBkgd;

void build_extra()
{
	pBkgd=std::make_shared<anim>();
	pBkgd->loadFromDir("db/anim0/",milliseconds(10));

	pBkgd->setLooping(true);
	pBkgd->play();
	pBkgd->setSmooth();
	
}




extern int fl1,fl2;

void runTest()
{
	int i=0;
	while(1)
	{
		i++;
//		printf("\t RUN TEST %D\n\n\n",i);
		printf("\t %d,%d,\t %s\n",fl1,fl2,fl1 && fl2?"OH SHIT\a":"0");
		sleep(milliseconds(100));
	}
}


RenderWindow* winp;


Thread	t1(	[]()
	{	
	//	sleep(milliseconds(5000));
		while(!winp)
			sleep(milliseconds(100));
		
		IamLoggedIn=0;
		RenderWindow &window=*winp;
		build_extra();		g
		pBkgd->setSize(Layout(winp->getSize().x,winp->getSize().y));
		
		lin=buildLogin(window);	g	//		lin->continuousRedraw=1; lin->drawfrq=2; setScene(lin);
			g//	return;
		menu=buildMenu(window);  g
		
		AnimatedPicture::Ptr pintro=static_pointer_cast<AnimatedPicture>(intro->guip->get("introAnim"));

		pintro->play();
		gm_play_sc=buildInGame(window);	g
		gm_end_sc=buildEndGame(window);	g
		myNames_sc=buildNameMgmt(window);	g
		hscore_sc=buildHighScore(window);	g

g
		sql_db_init();g
	/*	static*/ Thread	t2(me2ser_th);
	/*	static*/ Thread	t3(ser2me_th);
//		t2.launch();g
//		t3.launch();g


		while(pintro->isPlaying())
			sleep(milliseconds(100));
		
		if(IamLoggedIn)
			setScene(menu);
		else
			setScene(lin);
			
	});





int main()
{
	
	printf("Hey... : %s\n",getResourcePath().c_str());
	

	
#ifdef SFML_SYSTEM_ANDROID

	ActivityStates* states = getActivity(NULL);
	states->fullscreen=true;
	
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");

#else
//	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");
	sf::RenderWindow window(sf::VideoMode(400,600), "Hollywood Hunt");
#endif
	
	winp=&window;
	
	setGod(&window);
	

	err_sc=buildErrHandle(window);
	setScene(intro=buildIntro(window));

	t1.launch();
	
	while(!gm_play_sc)
		sleep(milliseconds(1000));
	setScene(gm_play_sc);
#ifdef SFML_SYSTEM_ANDROID
	PlayCurrentSceneFast();
#else
	PlayCurrentScene();
#endif	


//	PlayCurrentSceneFast();
	t1.terminate();
	return 0;
}


















