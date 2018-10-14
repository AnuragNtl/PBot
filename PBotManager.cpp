#include<iostream>
#include<string>
#include<map>
#include<boost/asio.hpp>
#include<boost/thread.hpp>
#include<vector>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace std;
using boost::asio::ip::tcp;
using namespace boost::property_tree;
class Message;
class MessageReceiver
{
public:
    virtual void receive(Message *con);
};
struct Message
{
    string sender;
    enum MESSAGE_TYPES{LOAD,EXEC,INCOMPLETE_MESSAGE,INVALID_MESSAGE};
    int msgType;
    string data;
    Message(int,string);
    operator string();
};
Message :: Message(int mType,string d) : msgType(mType),data(d)
{

}
Message :: operator string()
{
char buf[20];
sprintf(buf,"%d",msgType);
return string(buf)+" "+data;
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
    string nextBuf;
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
        cout <<"Message Arrived " <<(string)msg <<endl;
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
Connection :: Connection(tcp::socket *socket) : nextBuf("")
{
    this->socket=socket;
}
Message Connection :: getNextMessage()
{
    boost::system::error_code error;
    string bndry="\\\\\\\\\\\\\"\"\"\"\"\"//////";
    boost::asio::streambuf buf;
    boost::asio::read_until(*socket,buf,bndry);
    istream in(&buf);
    string data( (std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>() );
    data.resize(data.size()-bndry.size());
    cout <<data <<endl;
    ptree root;
    istringstream ss(data);
    read_json(ss,root);
    int msgType=root.get_child("msgType").get_value<int>();
    cout <<msgType <<endl;
    string d1=root.get_child("data").get_value<string>();
    return Message(msgType,d1);
}
int main()
{
BotServer server(8822);
server.start();
    return 0;
}