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
#include<boost/optional.hpp>
using namespace std;
using boost::asio::ip::tcp;
using namespace boost::property_tree;
using namespace boost::process;
using boost::optional;
class Message;
class Connection;
string getData(string file);
string basePath="";
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
    ptree extras;
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
    static map<string,Connection*> connections;
    static boost::mutex *connectionMutex;
    static void startListen(tcp::socket*,BotServer *);
    MessageReceiver *msgReceiver;
    public:
    BotServer(int,MessageReceiver *);
    void stop();
    void start();
    static void putConnection(string,Connection*);
    static Connection* getConnection(string sender)
    {
        Connection *r=NULL;
        connectionMutex->lock();
        if(connections.find(sender)!=connections.end())
        r=connections[sender];
    connectionMutex->unlock();
    return r;
    }
};
boost::mutex* BotServer :: connectionMutex=new boost::mutex;
map<string,Connection*> BotServer :: connections=map<string,Connection*>();
class Connection
{
    private:
    string nextBuf;
    tcp::socket *socket;
    string boundary;
    Connection(tcp::socket *,string);
    Message getNextMessage();
    friend class BotServer;
public:
    void sendResponse(string);
    void sendResponse(ptree &);
    bool isConnected();
};
BotServer :: BotServer(int port,MessageReceiver *msgReceiver) : msgReceiver(msgReceiver)
{
    this->port=port;
}
void BotServer :: putConnection(string sender,Connection *con)
{
    connectionMutex->lock();
    connections[sender]=con;
    connectionMutex->unlock();
}
void BotServer :: startListen(tcp::socket *socket,BotServer *bServer)
{
    try
    {
    Connection *con=new Connection(socket,bndry);
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
Connection :: Connection(tcp::socket *socket,string boundary) : nextBuf(""),boundary(boundary)
{
    this->socket=socket;
}
void Connection :: sendResponse(string response)
{
    boost::asio::write(*socket,boost::asio::buffer(response+boundary));
}
void Connection :: sendResponse(ptree &tree)
{
    ostringstream buf;
    write_json(buf,tree,false);
    sendResponse(buf.str());
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
    string d1="";
    optional<ptree &> o1=root.get_child_optional("data");
    if(o1)
        d1=o1->get_value<string>();
    cout <<d1 <<endl;
    optional<ptree &>extras1=root.get_child_optional("extras");
    ptree extras;
    if(extras1)
        extras=*extras1;
    string sender=root.get_child("sender").get_value<string>();
    Message r(msgType,d1);
    r.sender=sender;
    r.extras=extras;
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
    con->sendResponse(resp);
    }
    break;
    case (Message::LOAD):
    {
    	BotServer::putConnection(msg.sender,con);
    //receiveMutex.lock();
        string k1="";
        if(boost::filesystem::exists(msg.sender+".load.js"))
        k1=getData(basePath+msg.sender+".load.js");
        ptree resp;
        resp.put("load",k1);
    con->sendResponse(resp);
    //receiveMutex.unlock();
    }
    break;
    case (Message::SEND_EVENT):
    {
    	ptree resp;
    	optional<ptree &> receiverOpt=msg.extras.get_child_optional("receiver"),eventOpt=msg.extras.get_child_optional("event"),dataOpt=msg.extras.get_child_optional("data");
    	string receiver,event;
        ptree data;
    	if(!receiverOpt || !dataOpt || !eventOpt)
    	{
    		resp.put("error","invalid request");
    	}
    	else
    	{
    	ptree payload;
        receiver=receiverOpt->get_value<string>();
    		event=eventOpt->get_value<string>();
    		data=*dataOpt;
    		payload.put("event",event);
    		payload.add_child("data",data);
    		payload.put("receiver",receiver);
    		Connection *recvCon=BotServer::getConnection(receiver);
            if(recvCon)
            {
    		recvCon->sendResponse(payload);
    		resp.put("success","sent event");
    	}
        else
            resp.put("error","receiver not active");
        }
    	con->sendResponse(resp);
    }
    break;
    case (Message::REQUIRE_FILE):
    {
        optional<ptree &> file1=msg.extras.get_child_optional("file");
        ptree resp;
        if(!file1)
        {
            resp.put("error","File Not Given");
        }
        else
        {
            string file=file1->get_value<string>();
            if(!boost::filesystem::exists("dependencies/"+file))
            {
                resp.put("error","File Not Found");
            }
            else
            {
                string data=getData(basePath+file);
                resp.put("load",data);
            }

        }
        con->sendResponse(resp);
    }
    break;
    default:exit(0);
    break;
}
}
int main()
{
 if(boost::filesystem::exists("PBotConfig.json"))
 {   
string rawPathData=getData("PBotConfig.json");
try
{
    ptree root;
 istringstream ss(rawPathData);
 read_json(ss,root);
 basePath=root.get_child("base_path").get_value<string>();
 basePath=(basePath[basePath.size()-1]=='/'?basePath:basePath+"/");
}
catch(boost::exception &e){}
}
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