
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

#include "SceneHandler.hpp"
#include "sqlite3.h"


#include <cstdio>
#include <cstdlib>
#include <cstring>


#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>


#include <cctype>
#include <functional>
#include <cassert>



/*
char *strdup(const char *);
long stol(string );
*/

using namespace std;
using namespace sf;
using namespace tgui;
using namespace tgui::ext;




#ifndef  debugged_anim
#define  debugged_anim


#include <dirent.h>

#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/Android/Activity.hpp>
#include <android/asset_manager.h>
#include <android/native_activity.h>
#include <android/configuration.h>
#include <android/api-level.h>
#include <android/asset_manager_jni.h>

using namespace priv;

#endif


class anim:public AnimatedPicture
{
public	:
	
	void anim::setSmooth(bool smooth=true)
	{
		for(auto& texture : m_textures)
			texture.setSmooth(smooth);
	}
	
	void anim::setSize(const Layout& size)
	{
		Widget::setSize(size);
		for(auto& texture : m_textures)
		{
			texture.setSize(getSize());
		//	texture.setScale({ (float)getSize().x / (float)texture.getSize().x, (float)getSize().y / (float)texture.getSize().y});
		}
	}
	
	int loadFromDir(string basedir,sf::Time dur=milliseconds(50))
	{
		if(basedir=="")
			return -3;

		
#if defined (SFML_SYSTEM_ANDROID)
//	return -1;
		ActivityStates* states = getActivity(NULL);
		AAssetDir* dp;
		char *afn;
		
		if(*( basedir.end()-1)=='/')
			basedir.erase(basedir.end()-1);
		
		if(!(dp=AAssetManager_openDir(states->activity->assetManager, basedir.c_str())))
			return -1;
					
		while(afn=AAssetDir_getNextFileName(dp))
		{
//			printf("%s is being loaded\n",afn);
			addFrame(basedir+"/"+afn,dur);
		}
	
		AAssetDir_close(dp);


#else


		DIR *dp;
		struct dirent *dirx;
		
		if(!(dp=opendir((getResourcePath()+basedir).c_str())))		
			return -1;
			
		if(*( basedir.end()-1)!='/')
			basedir+='/';
			
	//	seekdir(dp,2);
		readdir(dp);	readdir(dp);
		
		while(dirx=readdir(dp))
		{
		//	puts(dirx->d_name);
			addFrame(basedir+dirx->d_name,dur);
		}
		closedir(dp);
#endif		
		return 1;
	}
	
};






#endif




extern Scene *curr_scene[5];

extern vector<string>	widget_names;
extern vector<string>	font_names;

extern Scene *err_sc,*lin,*menu,*hscore_sc,*about_sc,*myNames_sc,*gm_play_sc,*gm_end_sc,*intro;

extern std::shared_ptr<anim>	pBkgd,lBkgd;

extern vector<Button::Ptr>	myCBtnsp,myCBtnsl;
extern Grid::Ptr	myCBgrp,myCBgrl;
extern Label::Ptr gmLb,hud1;
extern TextBox::Ptr	FinalMsg;
extern HorizontalLayout::Ptr	Hollywood;
extern ListBox::Ptr	myNames;

//extern TextBox::Ptr	err_text;
extern ChatBox::Ptr	err_text;
extern Button::Ptr	ok;


extern vector<string>	gmlist[4];
extern string	base,current,answer;
extern int 	nor,now,tor,tow,status;
extern Clock	gmclk;


extern sqlite3 *hHunt_db;
extern Http		&ser;
extern int cnnctn;
extern int guestuserid;
extern int myuserid,clientid,ser_epoch,client_epoch,IamLoggedIn,ref_int_s2m,ref_int_m2s;

extern string hscore_tag;



std::string&	url_filter(std::string &s);
std::string geturl(sf::Http &http,sf::String url);
std::string geturl(sf::String host,sf::String url="");
std::stringstream	&fetchurl(sf::Http &http,sf::String url="");
std::stringstream	&fetchurl(sf::String host,sf::String url="");
FILE *fetchurl(sf::String host_with_url);
FILE *fetchurl_fp(sf::Http &http,sf::String url="");





void sql_db_init();
int RowReitriveCB(void *x,int argc,char **argv, char **colnm);

vector<string>	*get_name(char *fullname);
int add_name(char *fullname,int level=0);
int add_name(int id,char *fullname,int level,int userid,char *status);
int del_name(char *fullname);




vector<string>	*getParams(string pNm);
string getParam(string pNm);
int delParams(string pNm);
int addParam(string pNm,string pVl);
int setParam(string pNm,string pVl);

#ifndef	_setvar 
#define _setvar(x)	setParam(#x,to_string(x))
#endif	

#ifndef	_getvar 
#define _getvar(x)	(						\
	{									\
		string	_paramRet=getParam(#x);		\
		if(! _paramRet.empty())				\
			istringstream(_paramRet)>>(x);	\
		_paramRet.~string();				\
		(x);								\
	})
#endif	






int	login(string	email,	string pwd);
int	sign_up(string	email, string pwd, string  rpwd, string name,string mob, string sex);
int logout();
void me2ser_names();
void ser2me_th();
void me2ser_th();

void refresh_list();
void refreshScores();
void addScore(int score);

void BasicEvp(Scene *sc);

Scene *buildLogin(sf::RenderWindow &window);
Scene *buildMenu(sf::RenderWindow &window);
Scene *buildInGame(sf::RenderWindow &window);
Scene *buildEndGame(sf::RenderWindow &window);
Scene *buildNameMgmt(sf::RenderWindow &window);
Scene *buildHighScore(sf::RenderWindow &window);
Scene *buildErrHandle(sf::RenderWindow &window);
Scene *buildIntro(sf::RenderWindow &window);


void resetGameGui();
int tryAnswer(char ch);
void showStatus();
void startNewGame(int level=1);

string& solidify(string& s);
char *solid(string s);



/*
template <typename F, typename... A>
Button::Ptr	makeButton(string txt,F	fn,A... args);
*/

#ifndef	__mkBt
#define	__mkBt

template <typename F, typename... A>
Button::Ptr	makeButton(String txt,F	fn,A... args)
{
	Button::Ptr button = tgui::Button::create(widget_names[0]);
	button->setText(txt);
	button->connect("clicked",fn,  args...);
	return button;
}

#endif






#ifndef	__errHandle
#define	__errhandle



template<typename F, typename... A>
void showErr(String err,String btL,sf::Color color,F fn,A... args)
{
//	err_text->addText(err);
	err_text->addLine(err,color);
	
	ok->setText(btL);
	ok->disconnectAll();
	ok->connect("clicked",fn,args...);
	setScene(err_sc);
}


template<typename F, typename... A>
void showErr(String err,String btL,F fn,A... args)
{
	showErr(err,btL,sf::Color(255,0,0),fn,args...);
}

inline void showErr(String err="An error occured!!!",String btL="OK")
{
	showErr(err,btL, [&](){	setScene(curr_scene[1]);}	);
}


inline void showInfo(String err="Please Wait...",String btL="OK",sf::Color color=sf::Color(255,255,0))
{
	showErr(err,btL,color, [](){	setScene(curr_scene[1]);}	);
}

#endif

