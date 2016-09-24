

#include <cstdio>
#include <cstdlib>
#include <ctype.h>

#include <iostream>
#include <string>
#include <sstream>


#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Main.hpp>



using namespace std;
using namespace sf;

#include <TGUI/Tgui.hpp>
extern tgui::TextBox::Ptr err_text;


#define g  fprintf(stderr,"Debug %s/\t::%d\t%d\n",__FILE__,__LINE__,rand()%1000);

std::string&	url_filter(std::string &s)
{
	int i,k;
	char	buf[5];
	for(i=0;s[i];i++)
		if(!isalnum(s[i]))
		{
			k=sprintf(buf,"%%%x",s[i]);
			s.replace(i,1,buf);
			i+=k-1;
		}
	
	return s;
}




FILE *loginit()
{
#if defined (SFML_SYSTEM_ANDROID)
	FILE *fp=fopen("/data/data/pPanda.hHunt/files/serlog.txt","a+");  
#else
	FILE *fp=fopen("serlog.txt","a+");
#endif
	
	fprintf(fp,"\n\n\n\n\n\n\nLog Started\n\n");
	setvbuf(fp, NULL, _IONBF, 0);
	return fp;
}

//FILE *tfp=loginit();
FILE *tfp=NULL;




extern int cnnctn;

sf::Mutex	mut;

std::string geturl(sf::Http &http,sf::String url)
{								//		sf::Lock lc(mut);
	char err_buf[10000];
	
	sf::Http::Request request(url);
	sf::Http::Response response = http.sendRequest(request);

	sf::Http::Response::Status status = response.getStatus();
	if (status == sf::Http::Response::Ok)
	{
		sprintf(err_buf,"Succesful http\n\
Request = %s\n\
Res Status=%d\n\
Response ... \n%s\n\
	    ",	url.toAnsiString().c_str(),status,response.getBody().c_str());
	
		if(tfp)
			fprintf(tfp,"%s\n",err_buf);
//		printf("%s\n",err_buf);

		cnnctn=0;
		return response.getBody();
	}
	else
	{
	    sprintf(err_buf,"\n\nError Occured for http\n\
Request = %s\n\
Res Status=%d\n\
Response = %s\n\
	    ",	url.toAnsiString().c_str(),status,response.getBody().c_str());
		
		if(tfp)
			fprintf(tfp,"%s\n",err_buf);
//		printf("%s\n",err_buf);
		

		cnnctn=-1;
		return "";
	}	
}


std::string geturl(sf::String host,sf::String url="")
{
	sf::Http http(host);
	std::string base=geturl(http,url);
	return base;
}



std::stringstream	&fetchurl(sf::Http &http,sf::String url="")
{	
	return *(new std::stringstream(geturl(http,url)));
}



std::stringstream	&fetchurl(sf::String host,sf::String url="")
{
	return *(new std::stringstream(geturl(host,url)));
}



FILE *fetchurl(sf::String host_with_url)
{
	FILE *tfp=tmpfile();
	std::string	base=geturl(host_with_url);
	fwrite(base.c_str(),base.size(),1,tfp);
	return tfp;
}


FILE *fetchurl_fp(sf::Http &http,sf::String url="")
{
	FILE *tfp=tmpfile();
	std::string	base=geturl(http,url);
	fwrite(base.c_str(),base.size(),1,tfp);
	return tfp;
}


