
#include "all.h"






Scene *buildLogin(sf::RenderWindow &window)
{
	Gui *gp=new Gui(window);
	Gui *gl=new Gui(window);
	gl->setGlobalFont(font_names[0]);
	gp->setGlobalFont(font_names[0]);

	Scene *sc=new Scene(&window,gp,gl);
	sc->evF=BasicEvp;

	auto button = tgui::Button::create(widget_names[0]);
	button->setText("Quit Now");
	button->connect("clicked", [=](){ setScene(menu); menu->guip->get("about")->show(); });
	
	auto button2=button->copy(button);
	button2->setText("Quit Landscape");	
	
	
	auto emailBox = tgui::EditBox::create(widget_names[0]);
	emailBox->setDefaultText("Enter E-Mail...");
	
	auto pwdBox = tgui::EditBox::copy(emailBox);
	pwdBox->setDefaultText("Password");
	pwdBox->setPasswordCharacter('~');
	
	auto pwdcnfrmBox = tgui::EditBox::copy(pwdBox);
	pwdcnfrmBox->setDefaultText("Enter Password Again");
	pwdcnfrmBox->hide();
	
	auto nmBox = tgui::EditBox::copy(emailBox);
	nmBox->setDefaultText("Your Full Name");
	nmBox->hide();
	
	
	auto mobBox = tgui::EditBox::copy(emailBox);
	mobBox->setDefaultText("Mobile No.");
	mobBox->setNumbersOnly(1);
	mobBox->hide();
	
	auto sex=tgui::ComboBox::create(widget_names[0]);
	sex->addItem("Male","M");
	sex->addItem("Female","F");
	sex->setSelectedItem("Male");
	sex->hide();

	auto nbt = tgui::Button::create(widget_names[0]);	
	Tab::Ptr	t1=Tab::create(widget_names[0]);

	t1->add("Login");
	t1->add("Sign Up",0);
	t1->add("Guest Login",0);
	t1->connect("TabSelected" , [=]()
		{
			printf("Tab selected %d,\n",t1->getSelectedIndex());
			if(t1->getSelectedIndex() == 1)
			{
				pwdcnfrmBox->show();
				nmBox->show();
				mobBox->show();
				sex->show();
				
				nbt->setText("Sign Up");
			}
			else if(t1->getSelectedIndex() == 0)
			{
				pwdcnfrmBox->hide();
				nmBox->hide();
				mobBox->hide();
				sex->hide();
				nbt->setText("Login");
			}
			else
			{
				t1->select(0);
				setScene(menu);
				IamLoggedIn=2;
				myuserid=100;
				clientid=100;
				_setvar(myuserid);
				_setvar(clientid);
				_setvar(IamLoggedIn);
				
				hscore_tag="HighScores"+to_string(myuserid);
			}
		});
	

	nbt->setText("Login");
	nbt->connect("clicked",[=]()
		{
			static Thread *th;
			th=new Thread( [=]()
			{
				int status_flag=0;
				if(t1->getSelectedIndex())
				{
					showErr("\nSigning Up...\n This may take a while","Cancel",
							[=](){	setScene(curr_scene[1]);g	th->terminate();g delete th;	});
							
					if(	status_flag=sign_up(emailBox->getText().toAnsiString(),
							pwdBox->getText().toAnsiString(),
							pwdcnfrmBox->getText().toAnsiString(),
							nmBox->getText().toAnsiString(),
							mobBox->getText().toAnsiString(),
							sex->getSelectedItem().toAnsiString()
						  )	)
					{
						if(cnnctn)
							showInfo("Connection Failed!\n Check network settings...\n","Hmmm...",sf::Color(200,0,0));
						else if(status_flag==-5)
							showInfo("Email or Password can't be blank \n","Oops!@@!",sf::Color(0,0,255));
						else if(status_flag==-3)
							showInfo("Password's doesnt Match \n","Oops!@@!",sf::Color(0,0,255));
						else
							showErr("\nSignup Failed\n\t Check email and password\n");
					}
					else
						setScene(menu);
				}	  
				else
				{
					showErr("\nLogging In...\n This may take a while","Cancel",
							[=](){	setScene(curr_scene[1]);g	th->terminate();g	});
					
					if(status_flag=login(emailBox->getText().toAnsiString(),pwdBox->getText().toAnsiString()))
					{
						if(cnnctn)
							showInfo("Connection Failed!\n Check network settings...\n","Hmmm...",sf::Color(200,0,0));
						else if(status_flag==-5)
							showInfo("Email or Password can't be blank \n","Oops!@@!",sf::Color(0,0,255));
						else
							showErr("\nLogin Failed\n\t Check email and password\n");g
					}
					else
					{
						setScene(menu);
					}
				}
			
				th->terminate();
				delete th;
			} );
			
			th->launch();
		});	
	
	
	auto vl1=VerticalLayout::create();
	
	vl1->setPosition(bindWidth(*gp)*0.1,bindHeight(*gp)*0.05);
	vl1->setSize(bindWidth(*gp)*0.8,bindHeight(*gp)*0.8);
	
	
	vl1->add(t1);	
	vl1->add(emailBox);	
	vl1->add(pwdBox);
	vl1->add(pwdcnfrmBox);
	vl1->add(nmBox);
	vl1->add(mobBox);
	vl1->add(sex);
	vl1->add(nbt);
	vl1->add(button);
	
/**/	
	
	gp->add(vl1);
	
	for(int i=vl1->getWidgets().size(); --i>0;)
		vl1->insertSpace(i,0.1);
	

	
	auto picPortrait =std::make_shared<anim>();
	picPortrait->loadFromDir("db/anim3/",milliseconds(150));
	
	picPortrait->setLooping(true);
	picPortrait->play();
	picPortrait->setSmooth();
	
	picPortrait->setSize(bindSize(*gp));
	gp->add(picPortrait, "BKGDPortrait");
	picPortrait->moveToBack();
	
	
	sc->guil=NULL;
	return sc;
	
	
	
	auto vl2=	/*vl1->copy(vl1);*/	VerticalLayout::create();
	
	vl2->setPosition(bindWidth(*gl)*0.1,bindHeight(*gl)*0.05);
	vl2->setSize(bindWidth(*gl)*0.8,bindHeight(*gl)*0.5);
	
	gl->add(vl2);
	
	for(auto &w:vl1->getWidgets())
		vl2->add(w->clone());	
	
	vl2->remove(button);
	vl2->add(button2);
	
/*	
	for(int i=vl2->getWidgets().size(); --i>0;)
		vl2->insertSpace(i,.3);
*/	
	auto picLandscape = tgui::Picture::create("db/Background-Landscape.png");
	picLandscape->setSize(bindSize(*gl	, Vector2f(1,1))	);

	gl->add(picLandscape, "BKGDLandscape");
	picLandscape->moveToBack();
	
	
	printf("p: %u\t l: %u\n",gp,gl);	
	

	return sc;
}


