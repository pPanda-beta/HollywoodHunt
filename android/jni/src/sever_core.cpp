
#include "all.h"



//String	ser_loc="http://programmerpanda.byethost22.com";
String	ser_loc="http://ppanda.bugs3.com";
//String	ser_loc="localhost";

Http		&ser=*(new Http(ser_loc));
int guestuserid=100;
int myuserid=100,clientid=100,ser_epoch,client_epoch,IamLoggedIn,ref_int_s2m=25000,ref_int_m2s=25000;

int cnnctn;


int logout()
{
	if(IamLoggedIn==2)
	{
		setScene(lin);
		IamLoggedIn=0;
		
		_setvar(IamLoggedIn);
		return 0;
	}
	
/*	static */Thread *th=new Thread( [=]()
	{
		static int ret_code=0;
		int retry=7;
		while(!(ret_code==1024 || ret_code==1020)	&& retry--)
		{
			stringstream	&out=fetchurl(ser,
			"hhunt/logout.php?userid="+to_string(myuserid)
			+"&clientid="+to_string(clientid));
			
			out>>ret_code;
			delete	&out;
		}
		
		IamLoggedIn=0;
		myuserid=100;
		clientid=100;
		_setvar(myuserid);
		_setvar(clientid);
		_setvar(IamLoggedIn);
		setScene(lin);g
	//	delete th;
	}
	);
			
	th->launch();
}



int	login(string	email,	string pwd)
{
//	cout<<"Login GOt : "<<email<<" , "<<pwd<<" \n";
//	printf("\t\t\t %d,%d\n\n",email=="" , pwd=="");
	cnnctn=0;
	if(email=="" || pwd=="")
		return -5;
	
	int ret_code=1020;
	stringstream	&out=fetchurl(ser,
		"hhunt/login.php?email="+url_filter(email)
		+"&pwd="+url_filter(pwd));

	out>>ret_code;
	if(ret_code==1024)
	{
		out>>myuserid>>clientid>>ser_epoch;
		IamLoggedIn=1;
		_setvar(myuserid);
		_setvar(clientid);
		_setvar(ser_epoch);
		_setvar(IamLoggedIn);
		hscore_tag="HighScores"+to_string(myuserid);
	//	Beep(450,500);
		g
//		out.~stringstream();g
		delete	&out;	g
		return 0;
	}
	else
	{
//		cout<<myuserid<<"\t"<<clientid<<"\a\a\n";
		delete	&out;	
		return -1;
	}
	
}

int	sign_up(string	email, string pwd, string  rpwd, string name,string mob, string sex)
{
	cnnctn=0;
	if(email=="" || pwd=="")
		return -5;
	if(pwd!=rpwd)
		return -3;
	
	int ret_code=1020;
	
	stringstream	&out=fetchurl(ser,
		"hhunt/signup.php?email="+url_filter(email)  +"&pwd="+url_filter(pwd)
		+"&name="+url_filter(name)  +"&sex="+url_filter(sex)   
		+"&mob="+(mob==""?"0":url_filter(mob)));
	
	out>>ret_code;
	
	if(ret_code==1024)
	{
		out>>myuserid;
		cout<<out.str();
		g
	//	out.~stringstream();g
		delete	&out;	g
		err_text->addLine("\n Succesfully signed up.\n Logging in...\n");
		return login(email,pwd);
	}
	else
	{
		
	//	out.~stringstream();g
		delete	&out;	
		return -1;
	}
}


int _id,_uid;

int upload_name(string fullnm,string level)
{
	int ret_code=1020;
	char	fail_msg[100];
	
	stringstream	&out=fetchurl(ser,
		"hhunt/hhuntadd.php?userid="+to_string(myuserid)
		+"&clientid="+to_string(clientid)
		+"&gamename="+url_filter(fullnm)
		+"&level="+level
		);
	
	out>>ret_code;
	
	if(ret_code==1024)
	{
		
		out>>ser_epoch>>_id;
//		out.~stringstream();g
		delete	&out;	
		return 0;
	}
	else	if(ret_code==1021)
	{
		out>>_uid>>_id;
		out.get();
		out.get(fail_msg,sizeof(fail_msg));		
//		out.~stringstream();g
		delete	&out;	
		return -1;
	}
	else
	{
		delete	&out;	
		return -3;
	}
	
}



int upload_name_CB(void *x,int argc,char **argv, char **colnm)
{
//	printf("%s,\t%s,\t%s  %d\n",argv[0],argv[1],argv[2],string(argv[2])=="0");
	
	int id;
	switch(id=upload_name(argv[1],argv[2]))
	{
		case  -1:
			sqlite3_exec(hHunt_db,
				sqlite3_mprintf("UPDATE  'hHuntList' set id=%d , userid=%d , status = 'perm' \
							WHERE solidnm = %Q ; ",_id,_uid,argv[0]),
			0,0,0);
			break;
		
		
		case 0:
			sqlite3_exec(hHunt_db,
				sqlite3_mprintf("UPDATE  'hHuntList' set id=%d , status = 'perm' \
							WHERE solidnm = %Q ; ",_id,argv[0]),
			0,0,0);
			break;
	}
	
	if(id!=-3)
	{
		client_epoch++;
		_setvar(client_epoch);
		printf("||\t Sign=%d,%d/%d %d,%d\n",myuserid,clientid,IamLoggedIn,client_epoch,ser_epoch);
		
	}
	return 0;
}



int 	auto_crrct;


void me2ser_names()
{
	
	char *err;
/*	sqlite3_exec(hHunt_db,
		"SELECT solidnm,fullnm,level FROM 'hHuntList' WHERE status = 'temp';",
		upload_name_CB,0,&err);
*/	
	vector<string>	*vs=new vector<string>;	
	sqlite3_exec(hHunt_db,
		"SELECT solidnm,fullnm,level FROM 'hHuntList' WHERE status = 'temp';",
		RowReitriveCB,(void *)vs,&err);
	char *argv[3];
	
	
	auto_crrct=auto_crrct?3:0;
	
	if(!err	&&	!vs->empty())
	{
		for(int i=0; i < vs->size(); i+=3)
		{
			argv[0]=(*vs)[i].c_str();
			argv[1]=(*vs)[i+1].c_str();
			argv[2]=(*vs)[i+2].c_str();
			
			upload_name_CB(0,3,argv,0);
		}
	}
	
	if(auto_crrct==3)
		auto_crrct=0;
	
	delete vs;
	
	printf("me2ser N_Err=%s\n",err);
}




void ser2me_names()
{
	int ret_code=1020;
	int	n,sid,slevel,suserid;
	char	sname[256];
	g	
	stringstream	&out=fetchurl(ser,
		"hhunt/hhuntsync.php?userid="+to_string(myuserid)
		+"&clientid="+to_string(clientid)
		+"&clientepoch="+to_string(client_epoch)
		);
	
	out>>ret_code;
	g
	sleep(milliseconds(1950));g
	printf("ser2me Got ret_code=%d\n\n",ret_code);
	g
	if(ret_code==1024)
	{
		
		out>>ser_epoch>>n;
		_setvar(ser_epoch);
		while(n--)
		{
			out>>sid>>slevel>>suserid;
			out.get();
			out.getline(sname,sizeof(sname));
			
			del_name(sname);
			add_name(sid,sname,slevel,suserid,"perm");
			
			client_epoch++;
			_setvar(client_epoch);	
			printf("\a\a\t%c%c\n",3,3);
			printf("||\t Sign=%d,%d/%d %d,%d\n",myuserid,clientid,IamLoggedIn,client_epoch,ser_epoch);
			
		}
		
//		out.~stringstream();g
		delete	&out;	
		return 0;
	}
	else
	{
		g
		
//		out.~stringstream();g
		delete	&out;	
		return -1;
	}
}



void me2ser_th()
{
	while(1)
	{
		
		sleep(milliseconds(ref_int_m2s));
		if(IamLoggedIn)
			me2ser_names();
	}
}

void ser2me_th()
{
	while(1)
	{g
		printf("||\t Sign=%d,%d/%d %d,%d\n",myuserid,clientid,IamLoggedIn,client_epoch,ser_epoch);
		printf("XXXXXXXXXX %d, au=%d\n",cnnctn,auto_crrct);
		if(client_epoch==ser_epoch	|| cnnctn==-1)
			sleep(milliseconds(ref_int_s2m));
		
		while(!IamLoggedIn	||	auto_crrct)
			sleep(milliseconds(ref_int_s2m));

		
		
		if(IamLoggedIn	&&	!auto_crrct)
			ser2me_names();
		
		if(client_epoch>ser_epoch)
		{
			auto_crrct=1;
			client_epoch=0;
			_setvar(client_epoch);
			
			sqlite3_exec(hHunt_db,"UPDATE  'hHuntList' set status = 'temp' ;",0,0,0);
		}
		
		
		
	}
}

