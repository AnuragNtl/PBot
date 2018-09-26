#include<iostream>
#include<string>
#include<map>
#include<boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;
struct Message
{
	static enum MESSAGE_TYPES{LOAD,EXEC};
	int msgType;
	string data;
	Message(int,string);
};
Message :: Message(int mType,string d) : msgType(mType),data(d)
{
	
}
class Connection;
class BotServer
{
	public:
	BotServer(int);
	Connection* getNextConnection();
	void stop();
	void start();
};
BotServer :: BotServer(int port)
{
	boost::asio::io_service ioSvc;
	tcp::acceptor acceptor(ioSvc,tcp::endpoint(tcp::v4(),port));
	while(1)
	{
		tcp::socket socket(ioSvc);
		acceptor.accept(socket);
	}
}
class Connection
{
	private:
	tcp::socket &socket;
	public:
	Connection(tcp::socket &);
	Message getNextMessage();
};
Connection :: Connection(tcp::socket &socket)
{
	this->socket=socket;
}
Message getNext