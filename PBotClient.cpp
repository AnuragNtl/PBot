#include<boost/asio.hpp>
#include<iostream>
#include<stdint.h>
#include<fstream>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;
using namespace std;
using boost::asio::ip::tcp;
const string bndry="\\\\\\\\\\\\\"\"\"\"\"\"//////";
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
void sendMessage(string);
int main(int argc,char *argv[])
{
	string host="localhost";
	boost::asio::io_service ioService;
	tcp::resolver resolver(ioService);
tcp::resolver::query q(string(host),"8822");
tcp::resolver::iterator it1=resolver.resolve(q);
tcp::socket socket(ioService);
boost::asio::connect(socket,it1);
while(true)
{
	string data1=getNextMessage();
write(socket,boost::asio::buffer(data1+bndry));
boost::asio::streambuf buf;
boost::asio::read_until(socket,buf,bndry);
istream in(&buf);
    string data((std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>());
        data.resize(data.find(bndry));
        sendMessage(data);
    }
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
void sendEvent(string receiver,string event)
{
	ostringstream buf;
	ptree tree;
	ptree extras;
	tree.put("sender","general");
	tree.put("msgType","general");

}
