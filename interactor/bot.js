/*console.log("started");
var port=browser.runtime.connectNative("pbot");
console.log(port);
port.onMessage.addListener((response)=>
{
console.log(response);
});
port.postMessage({sender:"sample",msgType:0,data:"ls"});
console.log("Sent");*/
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
