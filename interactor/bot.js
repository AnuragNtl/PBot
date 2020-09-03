var resp;
var ports={};
browser.runtime.onMessage.addListener(function(message,sender)
{
var tabId=sender.tab.id;
var port=ports[tabId];
if(!port)
{
	port=browser.runtime.connectNative("pbot");
	console.log("sending");
port.onMessage.addListener(function(response)
{
	console.log(response);
	browser.tabs.sendMessage(tabId,response);
});
port.onDisconnect=(()=>{ports[tabId]=null;console.log(tabId+" disconnected");});
ports[tabId]=port;
}
port.postMessage(message);
});
