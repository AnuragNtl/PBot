#include<iostream>
#include<fstream>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
#include<boost/optional.hpp>
using namespace std;
using namespace boost::property_tree;
int main()
{
	string basePath;
	cout <<"\tPBot Configuration:\n";
	cout <<"Enter Script Base Path\n";
	std::getline(cin,basePath);
	ofstream out("PBotConfig.json");
	ptree tree;
	tree.put("base_path",basePath);
	ostringstream ss;
	write_json(ss,tree,false);
	out <<ss.str();
	out.close();
	return 0;
}
