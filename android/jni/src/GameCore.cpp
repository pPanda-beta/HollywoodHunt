

#include "all.h"

int	gmDirty[4]={1,1,1,1};
vector<string>	gmlist[4];



string	base,current,answer;
int 	nor,now,tor,tow,status;

Clock	gmclk;
int		gmtm;

string str1,str2,str3;

void showStatus()
{

	gmtm=gmclk.getElapsedTime().asMilliseconds();
	
//	printf("Time = %d\n",tm);
	
	static string  str;
	
	str3= "Time : "
		+	to_string(gmtm/3600000)+":"
		+	to_string((gmtm%3600000)/60000)+":"
		+	to_string((gmtm%60000)/1000)+"."
		+	to_string((gmtm%1000))+"\n"
		;
	hud1->setText(str1+str2+str3);	
}




void GameEnd(int dece)
{
	string	finish="";
	
	if(dece)
		finish+="You WON!!! :)\n";
	else
		finish+="You LOOSE!!! :(\n";
	
	finish+=str1+str2+str3;
	finish+="Name was : "+base+"\n";
	if(!dece)
		finish+="You found : "+current+"\n";
	
	int score=(100000000*nor)/(tor*(now+1)*gmtm);
	
//	score/=(gmtm/100);
	
	finish+="Score : "+to_string(score);
	addScore(score);
	
	FinalMsg->setText(finish);	
	setScene(gm_end_sc);
}



int tryAnswer(char ch)
{
//	printf("Tried %c\n",ch);
	
	int incorrect=1;
	
	for(int i=0;	base[i];	i++)
		if(toupper(base[i])==toupper(ch))
		{
			current[i]=base[i];	
			incorrect=0;
		}
	now+=incorrect;
	nor+=!incorrect;
	
	if(incorrect)
		Hollywood->get(now-1)->hide();
				
	gmLb->setText(current);
	
	str1="Right/Wrong : "+ to_string(nor) +"/"+to_string(now) +"\n";
	str2="Consonants Remaining :"+to_string(tor-nor) +"\n";
//	cout<<current<<"||\n";

	if(tor==nor)
		GameEnd(1);
	else if(now>=9)
		GameEnd(0);
	
}


int memUpdateCB(void *level,int argc,char **argv, char **colnm)
{//	printf("Found %s at level %d\n",argv[0],level);
	gmlist[(int) level].push_back(argv[0]);
	return 0;
}

void startNewGame(int level=1)
{
	nor=now=tor=tow=0;

	if(gmlist[level].empty()	||	gmDirty[level])
	{
		sqlite3_exec(hHunt_db,
			sqlite3_mprintf("SELECT fullnm FROM hHuntList WHERE level=%d ;",level),
			memUpdateCB,(void *)level,0);
		gmDirty[level]=0;
	}
	
	if(!gmlist[level].empty())
		base=gmlist[level][rand()%gmlist[level].size()];
	else
	{
		current=base="NO GAME FOUND";
//		return;
	}
	
	
	bool	alpha[26]={0};
	
	current.clear();
	for(int i=0;	base[i];	i++)
		if(!isalpha(base[i]) || strchr("AEIOU",toupper(base[i])))
			current+=base[i];
		else
			current+='_',alpha[toupper(base[i])-'A']=1;
	

	for(auto x:alpha)
		tor+=x;
	tow=26-tor;


	
	resetGameGui();
	str1="Right/Wrong : "+ to_string(nor) +"/"+to_string(now) +"\n";
	str2="Consonants to find :"+to_string(tor-nor) +"\n";
	gmLb->setText(current);
	gmclk.restart();
	setScene(gm_play_sc);
	
}




string& solidify(string& s)
{
	int i,j;

	for(i=j=0;	s[i];	i++)
		if(isalpha(s[i]))
			s[j++]=toupper(s[i]);
	
	s[j]='\0';
	
//	printf("\t\tstring_erase	i=%d,j=%d\n",i,j);
	if(j<i)
		s.erase(s.begin()+j+1,s.end());
	
	return s;
}

char *strdup(char *s)
{
	char *s2=new char[strlen(s)];
	strcpy(s2,s);
	return s2;
}

char *solid(string s)
{
	return strdup(solidify(s).c_str());		//
}
