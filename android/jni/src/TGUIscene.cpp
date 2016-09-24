

#include "TGUIscene.hpp"

static 	int	redrawreq;

void Scene::redraw(int	ms)
{
	drinterval=milliseconds(ms);
	drclk.restart();
	


	if(redrawreq)
		return;
	
	redrawreq=1;
	mdr.unlock();
	sleep(milliseconds(10));
	mdr.lock();
	redrawreq=0;
}





void Scene::onresize()
{
	if(guil && (ori = wd.getValue()>ht.getValue()))
		gui=guil;
	else
		gui=guip;

	wd=tgui::bindWidth(*gui);
	ht=tgui::bindHeight(*gui);
	gui->setView(win->getView());
}



void Scene::onresize(int width,int height)
{
	if(guil && (ori= width>height))
		gui=guil;
	else
		gui=guip;
	
	cout<<"Resized to "<<width<<"X"<<height<<"\n";
	
	sf::View view = win->getView();
	view.setSize(width, height);
     view.setCenter(width/2, height/2);
     win->setView(view);
    
	gui->setView(win->getView());
/*	gui->setView( sf::View{{0, 0, (float)width, (float)height}} );
	
	wd.~Layout1d();
	ht.~Layout1d();
	
	wd=tgui::bindWidth(*gui);
	ht=tgui::bindHeight(*gui);
*/	
	
//	cout<<wd.getValue()<<","<<ht.getValue()<<"\n";
}



Scene::Scene(sf::RenderWindow *windowp,tgui::Gui *gp,tgui::Gui *gl)
{
	win=windowp;
	guip=gp;
	guil=gl;
	
	evF=dsF=NULL;
	ev=new sf::Event ;

	guip->setWindow(*win);
	if(guil)
		guil->setWindow(*win);
	else
		gui=guip;
	
	wd=(win->getSize().x);
	ht=(win->getSize().y);

	onresize();
}
