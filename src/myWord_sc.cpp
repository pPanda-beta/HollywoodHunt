
#include "all.h"

ListBox::Ptr	myNames;

string lvNames[]={"Easy  ","Medium","Hard  ","Expert"};



void refresh_list()
{
	printf("\t\t\trefresh list started......");
	printf("||\t Sign=%d,%d/%d %d,%d\n",myuserid,clientid,IamLoggedIn,client_epoch,ser_epoch);
	
	myNames->removeAllItems();
	
	vector<string>	*vs=new vector<string>;
	char *err;
	sqlite3_exec(hHunt_db,
		sqlite3_mprintf("SELECT fullnm,level FROM 'hHuntList' \
					  WHERE userid = %d\
					  ORDER by level,fullnm;",
						myuserid),
		RowReitriveCB,vs,&err);
	
	printf(">>>> Refresh List _Err=%d\n",err);
	printf("\t\t\trefresh list Ended......\n");
	if(err	||	vs->empty())
	{
		delete vs;
		return;
	}
	int n=vs->size(),i;
	
	string str;
	
	for(i=0;i<n;i+=2)
	{
		myNames->addItem(lvNames[(*vs)[i+1][0]-'0']+" :: "+(*vs)[i]);
	}
	
	delete vs;
}



Scene *buildNameMgmt(sf::RenderWindow &window)
{
	Gui *gp=new Gui(window);
	gp->setGlobalFont(font_names[0]);
	Scene *sc=new Scene(&window,gp);
	
	
	auto vl1=VerticalLayout::create();
	auto vl2=VerticalLayout::create();
		
	vl1->setPosition(bindWidth(*gp)*0.1,bindHeight(*gp)*0.1);
	vl1->setSize(bindWidth(*gp)*0.8,bindHeight(*gp)*0.8);
	
	myNames=ListBox::create(widget_names[0]);
	
	HorizontalLayout::Ptr	hl1=HorizontalLayout::create();
	
//	hl1->add(makeButton("Delete", [&](){ setScene(menu); }));
	hl1->add(makeButton("Refresh", [&]()
		{	refresh_list();	}));
	hl1->add(makeButton("Back", [&](){ setScene(menu); }));
	
	HorizontalLayout::Ptr	hl2=HorizontalLayout::create();
	static EditBox::Ptr	mvName=EditBox::create(widget_names[0]);
	mvName->setDefaultText("Movie Name");
		
	static ComboBox::Ptr	levelB=ComboBox::create(widget_names[0]);
	
	levelB->addItem("Easy","0");
	levelB->addItem("Medium","1");
	levelB->addItem("Hard","2");
	levelB->addItem("Expert","3");
	levelB->setSelectedItem("Easy");
	
	
	hl2->add(levelB);
	hl2->addSpace(.1);
	hl2->add(makeButton("Add/Update",[=]()
		{
			char	*fnm=mvName->getText().toAnsiString().c_str();
			if(mvName->getText()=="")
				return;
			
		//	printf("sending %s to add_name\n",fnm);
			vector<string>	*nm=get_name(mvName->getText().toAnsiString().c_str());
		//	printf("sending %s to add_name\n",fnm);
			if(nm	&& !nm->empty() &&	(*nm)[4]!=to_string(myuserid))
			{g
				delete nm;
				refresh_list();
				return;
			}
	//		printf("sending %s to add_name\n",fnm);
			del_name(mvName->getText().toAnsiString().c_str());
		//	printf("sending %s to add_name\n",fnm);
			add_name(mvName->getText().toAnsiString().c_str(),levelB->getSelectedItemId().toAnsiString()[0] - '0');
			
			delete nm;
			//onthread(refresh_list);
			refresh_list();
		}));
	

	vl1->add(mvName);
	vl1->add(hl2);
	vl1->add(myNames);
	vl1->add(hl1);
	
	vl1->setRatio(hl2,.7);
	vl1->setRatio(myNames,7);
	
	for(int i=hl1->getWidgets().size(); --i>0;)
		hl1->insertSpace(i,.15);
	for(int i=vl1->getWidgets().size(); --i>0;)
		vl1->insertSpace(i,.15);
	
	vl1->setTransparency(100);
	
	auto picPortrait = tgui::Picture::create("db/pda10.jpg");
	picPortrait->setSize(bindSize(*gp));
	gp->add(picPortrait, "BKGDPortrait");
	picPortrait->moveToBack();
	
	
	gp->add(vl1);
	
	
	auto gtip=Tooltip::create(widget_names[0]);
	gtip->setText("Please choose difficulty level carefully...");
	gtip->setPosition(bindPosition(hl2	/*,{1.5,1.5}*/	));
	gtip->setTimeToDisplay(milliseconds(10000));
//	gtip->setPosition(100,100);
		
//	gp->add(gtip);
	
	return sc;
}
