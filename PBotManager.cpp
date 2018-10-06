#include<iostream>
#include<string>
#include<map>
#include<boost/asio.hpp>
#include<boost/thread.hpp>
#include<vector>
using namespace std;
using boost::asio::ip::tcp;
class Message;
class MessageReceiver
{
public:
	virtual void receive(Message *con);
};
struct Message
{
	string sender;
	enum MESSAGE_TYPES{LOAD,EXEC};
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
private:
	int port;
	static void startListen(tcp::socket*);
	public:
	BotServer(int);
	void stop();
	void start();
};
class Connection
{
	private:
	tcp::socket *socket;
	public:
	Connection(tcp::socket *);
	Message getNextMessage();
};
BotServer :: BotServer(int port)
{
	this->port=port;
}
void BotServer :: startListen(tcp::socket *socket)
{
	Connection *con=new Connection(socket);
	do
	{
		Message msg=con->getNextMessage();
	}
	while(1);
}
void BotServer :: start()
{
		boost::asio::io_service ioSvc;
	tcp::acceptor acceptor(ioSvc,tcp::endpoint(tcp::v4(),port));
	while(1)
	{
		tcp::socket *socket=new tcp::socket(ioSvc);
		acceptor.accept(*socket);
		boost::thread *thrd1=new boost::thread(startListen,socket);
	}
}
Connection :: Connection(tcp::socket *socket)
{
	this->socket=socket;
}
Message Connection :: getNextMessage()
{
		vector<char> buf(4);
	boost::system::error_code error;
	write(*socket,boost::asio::buffer("abcd"));
	socket->read_some(boost::asio::buffer(buf),error);
	for(int i=0;i<buf.size();i++)
		cout <<buf[i];
	cout <<endl;
	return Message(Message::LOAD,"");
}
int main()
{
BotServer server(8822);
server.start();
	return 0;
}
