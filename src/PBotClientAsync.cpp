#include<boost/asio.hpp>
#include<iostream>
#include<stdint.h>
#include<fstream>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
#include<queue>
#include<boost/thread.hpp>

using namespace boost::property_tree;
using namespace std;
using boost::asio::ip::tcp;
const string bndry="\\\\\\\\\\\\\"\"\"\"\"\"//////";
void log(string message)
{
	ofstream out("/tmp/T1.txt",ios::out | ios::app);
	out <<message <<endl;
	out.close();
}
bool sendEvent(string,string,string,tcp::socket&);
string getNextMessage()
{
		char rawLen[4];
	fread(rawLen,4,sizeof(char),stdin);
	uint32_t msgLen=*((uint32_t *)(rawLen));
	if(!msgLen)
		return 0;
	char *msg=new char[msgLen+1];
	fread(msg,msgLen,sizeof(char),stdin);
	msg[msgLen]='\0';
	string msg1=msg;
	delete[] msg;
	return msg1;
}
void addToMessages(tcp::socket *socket)
{
	while(true)
	{
		string msg1=getNextMessage();
	write(*socket,boost::asio::buffer(msg1+bndry));
	}
}
void sendMessage(string);
void handleMessage(tcp::socket *socket)
{
while(true)
{
boost::asio::streambuf buf;
boost::asio::read_until(*socket,buf,bndry);
istream in(&buf);
    string data((std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>());
        data.resize(data.find(bndry));
        sendMessage(data);
    }
}
int main(int argc,char *argv[])
{
	if(argc>1 && string(argv[1])=="--help")
	{
		cout <<"PBotClient:\n";
		cout <<"Usage: PBotClient RECEIVER EVENT_NAME JSON_DATA\n";
		return 0;
	}
	string host="localhost";
	boost::asio::io_service ioService;
	tcp::resolver resolver(ioService);
tcp::resolver::query q(string(host),"8822");
tcp::resolver::iterator it1=resolver.resolve(q);
tcp::socket socket(ioService);
boost::asio::connect(socket,it1);
if(argc==4)
{
sendEvent(argv[1],argv[2],argv[3],socket);
boost::asio::streambuf buf;
boost::asio::read_until(socket,buf,bndry);
istream in(&buf);
    string data((std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>());
data.resize(data.find(bndry));
sendMessage(data);
return 0;
}
boost::thread thrd1(addToMessages,&socket);
boost::thread thrd2(handleMessage,&socket);
while(true){}
        socket.close();
return 0;
}
void sendMessage(string msg)
{
	uint32_t msgLen=msg.size();
	char *rLen=reinterpret_cast<char *>(&msgLen);
	fwrite(rLen,4,sizeof(char),stdout);
	fwrite(msg.c_str(),msgLen,sizeof(char),stdout);
	fflush(stdout);
}
bool sendEvent(string receiver,string event,string data,tcp::socket &s)
{
	istringstream ss(data);
	ptree eventDetails;
	try
	{
		read_json(ss,eventDetails);
	}
	catch(boost::exception &e)
	{
		return false;
	}
	ptree tree;
	ptree extras;
	tree.put("sender","general");
	tree.put("msgType",3);
	tree.put("data","");
	extras.put("receiver",receiver);
	extras.put("event",event);
	extras.add_child("data",eventDetails);
	tree.add_child("extras",extras);
	ostringstream ss1;
	write_json(ss1,tree,false);
	write(s,boost::asio::buffer(ss1.str()+bndry));
	return true;
}
