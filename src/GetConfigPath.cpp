
#include <unistd.h>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include "GetConfigPath.h"
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
#include<boost/optional.hpp>
#include<fstream>

using namespace boost::property_tree;

string getConfigPath() {
    string basePath;
    string workingDirectory = getenv("HOME");
    workingDirectory.append(string("/") + CONFIG_DIR);
    if(!boost::filesystem::exists(workingDirectory)) {
        boost::filesystem::create_directory(workingDirectory);
    }
    string configFilePath = workingDirectory + (string("/") + CONFIG_FILE);
    if(!boost::filesystem::exists(configFilePath)) {
        generateConfigFile(configFilePath, workingDirectory);
    }
    return configFilePath;
}

void generateConfigFile(string path, string basePath) {

	ofstream out(path);
	ptree tree;
	tree.put(BASE_PATH_KEY, basePath);
	ostringstream ss;
	write_json(ss,tree,false);
	out <<ss.str();
	out.close();
}

string getData(string file) {
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

string getBasePath() {
	istringstream ss(getData(getConfigPath()));
	ptree root;
	read_json(ss,root);
    return root.get_child(BASE_PATH_KEY).get_value<string>();
}

