#include<iostream>
#include<string>
#include<map>
#include<boost/asio.hpp>
#include<boost/thread.hpp>
#include<vector>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
#include<boost/process.hpp>
#include<boost/filesystem.hpp>
using namespace std;
using boost::asio::ip::tcp;
using namespace boost::property_tree;
using namespace boost::process;
class Message;
class Connection;
string getData(string file);
const string bndry="\\\\\\\\\\\\\"\"\"\"\"\"//////";
class MessageReceiver
{
public:
    virtual void receive(Message con,Connection *)=0;
};
struct Message
{
    string sender;
    enum MESSAGE_TYPES{LOAD,EXEC,INVALID_MESSAGE,SEND_EVENT,REQUIRE_FILE,GENERAL};
    Message() : msgType(INVALID_MESSAGE){}
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
    static void startListen(tcp::socket*,BotServer *);
    MessageReceiver *msgReceiver;
    public:
    BotServer(int,MessageReceiver *);
    void stop();
    void start();
};
class Connection
{
    private:
    string nextBuf;
    tcp::socket *socket;
    Connection(tcp::socket *);
    Message getNextMessage();
    friend class BotServer;
public:
    void sendResponse(string);
};
BotServer :: BotServer(int port,MessageReceiver *msgReceiver) : msgReceiver(msgReceiver)
{
    this->port=port;
}
void BotServer :: startListen(tcp::socket *socket,BotServer *bServer)
{
    try
    {
    Connection *con=new Connection(socket);
    do
    {
        Message msg=con->getNextMessage();
     //   cout <<"Message Arrived " <<(string)msg <<endl;
        bServer->msgReceiver->receive(msg,con);
    }
    while(1);
}
catch(boost::exception &e){}
}
void BotServer :: start()
{
        boost::asio::io_service ioSvc;
    tcp::acceptor acceptor(ioSvc,tcp::endpoint(tcp::v4(),port));
    while(1)
    {
        tcp::socket *socket=new tcp::socket(ioSvc);
        acceptor.accept(*socket);
        boost::thread *thrd1=new boost::thread(startListen,socket,this);
    }
}
Connection :: Connection(tcp::socket *socket) : nextBuf("")
{
    this->socket=socket;
}
void Connection :: sendResponse(string response)
{
    boost::asio::write(*socket,boost::asio::buffer(response));
}
Message Connection :: getNextMessage()
{
    boost::system::error_code error;
    boost::asio::streambuf buf;
    boost::asio::read_until(*socket,buf,bndry);
    istream in(&buf);
    string data( (std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>() );
    data.resize(data.find(bndry));
    cout <<data <<endl;
    ptree root;
    istringstream ss(data);
    try
    {
    read_json(ss,root);
    }
    catch(boost::exception &e)
    {
        cout <<"Error in parsing\n";
        return Message(0,"");
        //e.get_error_info();
        //cout <<e.what() <<endl;
    }
    int msgType=root.get_child("msgType").get_value<int>();
    cout <<msgType <<endl;
    string d1=root.get_child("data").get_value<string>();
    string sender=root.get_child("sender").get_value<string>();
    Message r(msgType,d1);
    r.sender=sender;
    return r;
}
boost::mutex receiveMutex;
class PBotMessageReceiver : public MessageReceiver
{
public:
void receive(Message msg,Connection *);
};
void PBotMessageReceiver :: receive(Message msg,Connection *con)
{
    switch(msg.msgType)
    {
    case (Message::EXEC):
    {
        ipstream pipe;
        child process(msg.data,std_out > pipe);
        string rd;
        string output="";
        std::error_code k;
        //process.wait(k);
        while(pipe && getline(pipe,rd) && !rd.empty())
            output=output+rd+"\\n";
    cout <<"Message Received from " <<msg.sender 
    <<" with type " <<msg.msgType <<" with data "+msg.data <<endl;
    ptree resp;
    resp.put("output",output);
    resp.put("err_code",k.value());
    ostringstream buf;
    write_json(buf,resp,false);
    con->sendResponse(buf.str()+bndry);
    }
    break;
    case (Message::LOAD):
    {
    //receiveMutex.lock();
        string k1="";
        if(boost::filesystem::exists(msg.sender+".load.js"))
        getData(msg.sender+".load.js");
        ptree resp;
        ostringstream buf;
        resp.put("load",k1);
        write_json(buf,resp,false);
    con->sendResponse(buf+bndry);
    //receiveMutex.unlock();
    }
    break;
    default:exit(0);
    break;
}
}
int main()
{
BotServer server(8822,new PBotMessageReceiver);
server.start();
    return 0;
}
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
