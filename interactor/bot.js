console.log("started");
var port=browser.runtime.connectNative("pbot");
console.log(port);
port.onMessage.addListener((response)=>
{
console.log(response);
});
