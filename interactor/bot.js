console.log("started");
var port=browser.runtime.connectNative("pbot");
console.log(port);
port.onMessage.addListener((response)=>
{
console.log(response);
});
port.postMessage({sender:"sample",msgType:0,data:"ls"});
console.log("Sent");
