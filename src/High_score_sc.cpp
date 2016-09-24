
#include "all.h"

ListBox::Ptr	scoreList;

string hscore_tag="HighScores"+to_string(myuserid);


void refreshScores()
{
	hscore_tag="HighScores"+to_string(myuserid);
	
//	cout<<"\n\n Loading scores for "<<hscore_tag<<"\n";
	
	scoreList->removeAllItems();
	
	auto	vs=getParams(hscore_tag);
	
	if(!vs)
		return;
	
	sort(vs->begin(),vs->end(),	[](string a, string b)
	{
		long x,y;
		stringstream(a)>>x;
		stringstream(b)>>y;
		
		return y>x;
     	//return stol(b) > stol(a);   
	});
	
	reverse(vs->begin(),vs->end());
	
	for(auto score:*vs)
		scoreList->addItem(score);
	
	delete vs;
}


void addScore(int score)
{
	hscore_tag="HighScores"+to_string(myuserid);
	addParam(hscore_tag,to_string(score));
	refreshScores();
}




Scene *buildHighScore(sf::RenderWindow &window)
{
	Gui *gp=new Gui(window);
	Gui *gl=new Gui(window);
	gl->setGlobalFont(font_names[0]);
	gp->setGlobalFont(font_names[0]);
	
	Scene *sc=new Scene(&window,gp);
	
	srand(time(0));
//	Button::Ptr bk=makeButton("Back",setScene,menu);
	Button::Ptr bk=makeButton("Back", [&](){ setScene(menu); });

//	Button::Ptr bk=makeButton("Back",addScore,rand()%1000);
	Button::Ptr rs=makeButton("Reset", [&]()
		{
			hscore_tag="HighScores"+to_string(myuserid);
			delParams(hscore_tag);
			scoreList->removeAllItems();	
		}	);
	
	scoreList=ListBox::create(widget_names[0]);
	
	scoreList->setPosition(bindWidth(*gp)*0.1,bindHeight(*gp)*0.2);
	scoreList->setSize(bindWidth(*gp)*0.8,bindHeight(*gp)*0.5);
	
	
	
	bk->setPosition(bindLeft(scoreList),bindBottom(scoreList)*1.05);
	bk->setSize(bindWidth(scoreList)/2,bindHeight(scoreList)*0.1);
	
	rs->setPosition(bindRight(bk),bindBottom(scoreList)*1.05);
	rs->setSize(bindWidth(scoreList)/2,bindHeight(scoreList)*0.1);
	
	
	gp->add(bk);
	gp->add(rs);
	gp->add(scoreList);
	
	
	
	scoreList->setTransparency(55);
	
	
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
	bk->connect("SizeChanged",[=](){	pBkgd->setSize(gp->getSize());	});
	
	return sc;
}




