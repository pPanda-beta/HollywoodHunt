#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>

#include <SFML/OpenGL.hpp>

#if defined (SFML_SYSTEM_ANDROID)
    
    #define glFrustum	glFrustumf
    #define glClearDepth	glClearDepthf
#endif


#include <TGUI/TGUI.hpp>


using namespace std;
using namespace sf;
using namespace tgui;






class Scene
{
public:
	sf::RenderWindow *win;
	sf::Event		*ev;
	tgui::Gui		*gui,*guip,*guil;	
	
	sf::Thread	*evTh,*dsTh;
	
	sf::Mutex		mdr;
	sf::Clock		drclk;
	sf::Time		drinterval;
	
	void (*evF)	(Scene *);
	void (*dsF)	(Scene *);
	
	Layout1d wd,ht;
	int	ori;
	
	bool continuousRedraw =	false;
	float drawfrq=1000	;		//Hz
	
/*	Scene::*/Scene(sf::RenderWindow *windowp,tgui::Gui *gp,tgui::Gui *gl=NULL);
	void	/*Scene::*/redraw(int ms=250);
	void	/*Scene::*/onresize();
	void /*Scene::*/onresize(int width,int height);
};




#define g  fprintf(stderr,"Debug %s/\t::%d\t%d\n",__FILE__,__LINE__,rand()%1000);
#ifdef SFML_SYSTEM_ANDROID
	#include <android/log.h>
	#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_INFO, "tgui-activity", __VA_ARGS__))
	#define printf	LOGE
	#define fprintf(A,B...)	LOGE(B)
#endif


/*
#define onthread(A...)		\
{						\
	sf::Thread __th1(A);	\
	__th1.launch();		\
}
*/


#define onthread(A...)		({	(new Thread(A))->launch();	})





