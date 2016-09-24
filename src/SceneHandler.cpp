

#include "SceneHandler.hpp"


Scene *curr_scene[5];
static sf::Thread *dis_th,*autodis_th;

static sf::RenderWindow *GOD_WIN;

int 	gfl=1,autodisfl,fl1,fl2;

sf::Mutex	guiStall,autodis_gate;



int setGod(sf::RenderWindow *gd)
{
	GOD_WIN=gd;
}






void autodis()
{
	Scene *sc;
	while(1)
	{
		autodis_gate.lock();
		autodis_gate.unlock();
		autodisfl=1;
		//curr_scene[0]->mdr.unlock();
		while((sc=curr_scene[0])->continuousRedraw)
		{//printf("\t\tAUDISSSSSSS\n");
			sleep(milliseconds((int)	1000.0f/sc->drawfrq));
			sc->mdr.unlock();
			sc->redraw(50);
		}
		autodisfl=0;
	}
}

void startautodis()
{
/*	if(curr_scene[0]->continuousRedraw)
			sleep(milliseconds((int)	1000.0f/curr_scene[0]->drawfrq));
	return ;

*/
#ifdef	SFML_SYSTEM_ANDROID	
	return;
#endif
	g
	if(!autodis_th)
		(autodis_th=new Thread(autodis))->launch();
	autodis_gate.unlock();
	sleep(milliseconds(100));
	while(!autodisfl)autodis_gate.lock();
	curr_scene[0]->mdr.unlock();
	printf("\tSTART AUTO DISPLAY RETURNED\n");
}



int setScene(Scene *sc)
{
//	printf("Setting \t %u,%u->%u\n",sc,curr_scene[0],curr_scene[1]);
	if(curr_scene[0])curr_scene[0]->mdr.unlock();	g
	if(sc!=curr_scene[0])
		curr_scene[1]=curr_scene[0];
	Scene *osc=curr_scene[0];
	curr_scene[0]=sc;
	
	if(osc)
		osc->mdr.unlock();
	sc->onresize();
	g	
	guiStall.unlock();
	if(dis_th)
	{
		
		sc->redraw(1000);
		sc->mdr.unlock();
		
		if(sc->continuousRedraw)
			startautodis();
		if(curr_scene[1])
		{
		//	printf("Openning lock for %u\n",curr_scene[1]);
			curr_scene[1]->mdr.unlock();
			
		}
	}
	
	return 0;
}









void drawWin(Scene **scp)
{
	Scene *sc;
	while((sc=*scp)->win->isOpen())	
	{	//	printf("CMP:: %d,%d\n",sc->drclk.getElapsedTime().asMilliseconds(),sc->drinterval.asMilliseconds());

#ifdef	SFML_SYSTEM_ANDROID	
		sleep(milliseconds(50));
#else	
		if(sc->continuousRedraw	&& !autodisfl)
			startautodis();
		if(sc->drclk.getElapsedTime()		>	sc->drinterval)
		{
		//	printf("Waiting for %u s lock...",sc);
			sc->mdr.lock();
			sc->mdr.unlock();
			
		}
		
#endif
//		if(!  sc->win->hasFocus())
//			continue;

		sc->win->clear();
		
		if(sc->dsF)
			sc->dsF(sc);
		
		sc->win->pushGLStates();
		
//#ifdef	SFML_SYSTEM_ANDROID
		guiStall.lock();		fl1=1;	//printf("\nDS\t");
//#endif
//
		sc->gui->draw();
							fl1=0;	//printf("DE\n");
//#ifdef	SFML_SYSTEM_ANDROID
		guiStall.unlock();
//#endif
	//
		sc->win->popGLStates();

		sc->win->display();
		
	}
	
}









void eventProcessor(Scene **scp)
{
	Scene *sc;
	while((sc=*scp)->win->isOpen() && sc->win->waitEvent(*(sc->ev)))
	{
						/*	sc->win->pollEvent(*(sc->ev));*/
		if (sc->ev->type == sf::Event::Closed)
                sc->win->close();
          else if (sc->ev->type == sf::Event::Resized)
          {g
          	sc->onresize(sc->ev->size.width, sc->ev->size.height);
		} 
	
		if(sc->evF)
          	sc->evF(sc);

//#ifdef	SFML_SYSTEM_ANDROID
		guiStall.lock();		fl2=1;
//#endif
//printf("\nES\t");
		sc->gui->handleEvent(*(sc->ev));  
							fl2=0;	//printf("EE\n");
//#ifdef	SFML_SYSTEM_ANDROID
		guiStall.unlock();
//#endif
			

   //       printf("EV:%d  ",sc->ev->type);
          sc->redraw();
	}
	
}





int PlayScene(Scene *sc)
{
	// later
	return 0;
}








int PlayCurrentScene()
{
	int result=0;
	
	guiStall.unlock();
	
	curr_scene[0]->win->setActive(false);
	(dis_th=new sf::Thread(drawWin,curr_scene))->launch();
//	curr_scene->mdr.lock();
//	Sleep(10);
	curr_scene[0]->redraw(1000);
	
/*	if(curr_scene[0]->continuousRedraw)
		startautodis();
*/	
	while(GOD_WIN->isOpen())
		eventProcessor(curr_scene);
		
		
	curr_scene[0]->mdr.unlock();
	guiStall.unlock();
//	dis_th->wait();g
	dis_th->terminate();
	dis_th->~Thread();
	if(autodis_th)
	{
		autodis_th->terminate();
		autodis_th->~Thread();
	}
	return result;	
}












int PlayCurrentSceneFast()
{
	Scene *sc;
	while(GOD_WIN->isOpen())
	{
		sc=curr_scene[0];
//		printf("PFAST %u\t%u\n",sc->win,sc->gui);
		while(sc->win->pollEvent(*(sc->ev)))
		{
			if (sc->ev->type == sf::Event::Closed)
               	sc->win->close();
          	else if (sc->ev->type == sf::Event::Resized)
          	{
          		sc->onresize(sc->ev->size.width, sc->ev->size.height);
			} 
			
			if(sc->evF)
          	//	onthread(sc->evF,sc); 
          		sc->evF(sc);
				
          	sc->gui->handleEvent(*(sc->ev));
		}
		
		
		sc->win->clear();
		
		if(sc->dsF)
			sc->dsF(sc);
		
		sc->win->pushGLStates();
		sc->gui->draw();
		sc->win->popGLStates();

		sc->win->display();
	}
	
	return 0;
}
