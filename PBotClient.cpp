#include<boost/asio.hpp>
#include<iostream>
using namespace std;
using boost::asio::ip::tcp;
const string bndry="\\\\\\\\\\\\\"\"\"\"\"\"//////";
int main(int argc,char *argv[])
{

	if(argc>1)
	{
	boost::asio::io_service ioService;
		tcp::resolver resolver(ioService);
tcp::resolver::query q(string(argv[1]),"8822");
tcp::resolver::iterator it1=resolver.resolve(q);
tcp::socket socket(ioService);
boost::asio::connect(socket,it1);
write(socket,boost::asio::buffer(string(argv[2])+bndry));
boost::asio::streambuf buf;
boost::asio::read_until(socket,buf,bndry);
istream in(&buf);
    string data( (std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>() );
        data.resize(data.find(bndry));
        cout <<data;
        socket.close();
return 0;
}
else
{
	string rd="";
while(rd!="quit")
{
	cin >>rd;
	cout <<
}
}
return 0;
}
