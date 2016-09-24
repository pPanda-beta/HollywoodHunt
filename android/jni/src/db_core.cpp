
#include "all.h"
#include "sqlite3.h"


sqlite3 *hHunt_db;










//typedef int (*sqlite3_callback)(void*,int,char**, char**);

/*
SQLITE_API int sqlite3_exec(
  sqlite3*,                                  // An open database 
  const char *sql,                           // SQL to be evaluated
  int (*callback)(void*,int,char**,char**),  // Callback function 
  void *,                                    // 1st argument to callback 
  char **errmsg                              // Error msg written here 
);
*/

int sqlCB(void *x,int argc,char **argv, char **colnm)
{
	for(int i=0;i<argc;i++)
	{
		printf("%d: %s=%s\n",i,colnm[i],argv[i]);
		
	}
	printf("\n_/\\_\n");
	return 0;
}




int table_exists(char *tablenm,sqlite3 *db)
{
	char *err;
	sqlite3_exec(db,(string("SELECT * FROM ")+tablenm+";").c_str(),0,0,&err);
	printf("E_Err=%d\n",err);
	return !err;
}

int create_table(char *tablenm,char *tablebody,sqlite3 *db)
{
	char *err;
	sqlite3_exec(db,(string("DROP TABLE ")+tablenm+" ;").c_str(),0,0,&err);	
	printf("D_Err=%s\n",err);
	sqlite3_exec(db,(string("CREATE TABLE ")+tablenm+tablebody+" ;").c_str(),0,0,&err);
	printf("C_Err=%s\n",err);
	return !!err;
}





int RowReitriveCB(void *x,int argc,char **argv, char **colnm)
{
	for(int i=0;i<argc;i++)
	{
		((vector<string> *)x)->push_back(argv[i]?argv[i]:"");
	}
	return 0;
}


vector<string>	*get_name(char *fullname)
{
	vector<string>	*vs=new vector<string>;
//	printf("ZZchecking for %s\n",fullname);
	char *err;
	sqlite3_exec(hHunt_db,
		sqlite3_mprintf("SELECT * FROM 'hHuntList' WHERE solidnm = %Q;",
					solid(fullname)),
		RowReitriveCB,(void *)vs,&err);
		
	printf("GET N_Err=%d\n",err);
	
//	printf("XXchecking for %s\n",fullname);
	if(!err	&&	!vs->empty())
		return vs;

	delete vs;
	return NULL;
}


sf::Mutex		sql_mut;

int add_name(char *fullname,int level=0)
{
//	Lock	lc(sql_mut);
//cout<<level<<"|||\n";
	char *err;
	
	g
	solid(fullname);
	g
	
	sqlite3_mprintf("INSERT INTO 'hHuntList' ('solidnm','fullnm','level','userid')\
						VALUES(%Q,%Q,%d,%d);",
					solid(fullname),fullname,level,myuserid);
	
	g
	sqlite3_exec(hHunt_db,
		sqlite3_mprintf("INSERT INTO 'hHuntList' ('solidnm','fullnm','level','userid')\
						VALUES(%Q,%Q,%d,%d);",
					solid(fullname),fullname,level,myuserid),
		0,0,&err);
	printf("adding %s... ",fullname);
	printf("A N_Err=%s\n",err);
	return !err;
}


int add_name(int id,char *fullname,int level,int userid,char *status)
{
	char *err;
	sqlite3_exec(hHunt_db,
		sqlite3_mprintf("INSERT INTO 'hHuntList' \
						VALUES(%d,%Q,%Q,%d,%d,%Q);",
				id,solid(fullname),fullname,level,userid,status),
		0,0,&err);
		
	printf("A_FULL N_Err=%s\n",err);
	return !err;
}



int del_name(char *fullname)
{
	char *err;
	sqlite3_exec(hHunt_db,
		sqlite3_mprintf("DELETE FROM 'hHuntList' WHERE solidnm = %Q;",
					solid(fullname)),
		0,0,&err);
		
	printf("DEL N_Err=%s\n",err);
	return !err;
}






vector<string>	*getParams(string pNm)
{
	vector<string>	*vs=new vector<string>;
	
	char *err;
	sqlite3_exec(hHunt_db,
		sqlite3_mprintf("SELECT value FROM 'params' WHERE field = %Q;",
					pNm.c_str()),
		RowReitriveCB,(void *)vs,&err);
		
	printf("GET Param_Err=%s	|| %d\n",err,vs->empty());

	if(!err	&&	!vs->empty())
		return vs;

	delete vs;
	return NULL;
}



string getParam(string pNm)
{
	string	result="";
	vector<string>	*vs=getParams(pNm);
	if(vs)
	{
		result=(*vs)[0];
		delete vs;
	}
	
	return result;
}






int delParams(string pNm)
{

	char *err;
	sqlite3_exec(hHunt_db,
		sqlite3_mprintf("DELETE FROM 'params' WHERE field = %Q;",
					pNm.c_str()),
		0,0,&err);
		
	printf("Del Param_Err=%s\n",err);

	return !err;
}





int addParam(string pNm,string pVl)
{
	char *err;
	sqlite3_exec(hHunt_db,
		sqlite3_mprintf("INSERT INTO 'params'  VALUES(%Q,%Q)	;",
					pNm.c_str(),pVl.c_str()),
		0,0,&err);
		
	printf("ADD Param_Err=%s\n",err);

	return !err;
}


int setParam(string pNm,string pVl)
{
	return delParams(pNm),addParam(pNm,pVl);
}





void sql_db_init()
{
	g
#if defined (SFML_SYSTEM_ANDROID)
	sqlite3_open("/data/data/pPanda.hHunt/files/hhunt_1.db",&hHunt_db);    
#else
	sqlite3_open("hhunt_1.db",&hHunt_db);
#endif	
	g
	if(!table_exists("params",hHunt_db))
	{g
		create_table("'params'",
		"(	\
			'field'	varchar(255) ,	\
			'value'	varchar(255) DEFAULT NULL	\
		)",hHunt_db);
		
		setParam("OneTimeInit","DoubleDone");
		
		IamLoggedIn=0;
		myuserid=100;
		clientid=100;
		_setvar(myuserid);
		_setvar(clientid);
		_setvar(IamLoggedIn);
		
		client_epoch=0;
		_setvar(client_epoch);
		
	}
	
	if(!table_exists("hHuntList",hHunt_db))
	{g
		create_table("'hHuntList'",
		"(	\
			'id' int(10)	DEFAULT NULL,	\
			'solidnm'	varchar(255) ,	\
			'fullnm'	varchar(255) DEFAULT NULL,	\
			'level' int(10)	DEFAULT 0,	\
			'userid' int(10)	DEFAULT 0,	\
			'status'	varchar(50) DEFAULT  'temp',	\
			PRIMARY KEY	(solidnm)				\
		)",hHunt_db);
		
/*		add_name("Hello World",0);
		add_name("Hi' There",0);
		add_name("Joker",0);
		add_name("Jack The Giant Killer 2012");*/
	}


	g
	
	
	
	_getvar(myuserid);g
	_getvar(clientid);g
	_getvar(IamLoggedIn);g
	_getvar(client_epoch);g
	_getvar(ser_epoch);g
	
	hscore_tag="HighScores"+to_string(myuserid);
}





















int main2()
{
	int count;
	FILE *fp;
	sqlite3 *db;
	string query; 
	vector<int> fid;
	vector<int> fver;
	vector<string> fname;
	
	
	sqlite3_open("abc.db",&db);
	hHunt_db=db;
/*	
	sqlite3_exec(db,
		sqlite3_mprintf("INSERT INTO file VALUES(%d,%d,%Q);",7,5,"xyhhz.dll"),
		sqlCB,0,0);

	sqlite3_exec(db,
		"INSERT INTO file VALUES(\"6\",\"5\",\"xyz.dll\");",
		sqlCB,0,0);

	sqlite3_exec(db,"SELECT * FROM file;",sqlCB,0,0);
	
	*/
	
//	sqlite3_exec
	printf("Start.......\n");
	table_exists("file",db);
	
	
//	sqlite3_exec(db,"DROP TABLE hHuntList;",0,0,0);
/*	
	create_table("'hHuntList'",
	"(	\
		'id' int(10)	DEFAULT NULL,	\
		'solidnm'	varchar(255) ,	\
		'fullnm'	varchar(255) DEFAULT NULL,	\
		'level' int(10)	DEFAULT 0,	\
		'userid' int(10)	DEFAULT 0,	\
		'status'	varchar(50) DEFAULT  'temp',	\
		PRIMARY KEY	(solidnm)				\
	)",	db);
	
	*/
	
	
	string s="ABCD - 2 2015 -team17:)Ha HA";
	cout<<s<<"\n";
		
	solidify(s);
	
	cout<<s<<"\n";
	
	
	/*
	add_name("Hello World",0);
	add_name("Hi' There",0);
	add_name("Joker",0);
	add_name("Jack The Giant Killer 2012");
	*/
	sqlite3_exec(db,"SELECT * FROM hHuntList;",sqlCB,0,0);
	
	srand(time(0));
	
	startNewGame(0);
	
	cout<<base<<"\n";
	cout<<current<<"\n";

	system("PAUSE");
	return 0;
}



