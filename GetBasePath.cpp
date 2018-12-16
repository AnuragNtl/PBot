#include<iostream>
#include<fstream>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
#include<boost/optional.hpp>
using namespace std;
using namespace boost::property_tree;

string getData(string file)
{
    ifstream in(file.c_str());
    int ct=0;
    string r="";
    char *buf=new char[1024];
    do
    {
        in.getline(buf,1024);
        ct=in.gcount();
        if(ct>0)
        r=r+buf+"\n";
    }
    while(ct>0);
    if(r.size()>0)
    r.resize(r.size()-1);
    in.close();
    return r;
}
int main()
{
	string rawConfig=getData("PBotConfig.json");
	istringstream ss(rawConfig);
	ptree root;
	try
	{
		read_json(ss,root);
		cout <<root.get_child("base_path").get_value<string>();
	}
	catch(boost::exception &e)
	{
		return 1;
	}
	return 0;
}
