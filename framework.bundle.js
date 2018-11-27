var port=browser.runtime.connectNative("pbot");
function pbot()={}
pbot.prototype.events={};
	pbot.prototype.onLoad=function(k)
	{
		this.onLoaded=f;
	}
	pbot.prototype.onOutputReceived(f)
	{
		this.onOutput=f;
	}
	pbot.prototype.on=function(event,onEvent)
	{
		this.events[event]=f;
	}
pbot.prototype.exec=function(path,onExec)
{
	this.onExec=onExec;
}
pbot.prototype.sendEvent=function(to,data)
{
	port.postMessage({sender:"general",msgType:"event",receiver:to,data:data});
}
pbot.prototype.trigger=function(event,data)
{
	if(this.events[event])
	this.events[event](data);
}
pbot.prototype.saveFile=function(file,data,mode)
{
	
}
pbot.prototype.requireFile=function(file,onFetch)
{
	this.onRequireFile=onFetch;
}
pbot.prototype.onError=function(onErr)
{
	this.onErr=onErr;
}
var pBot=new pbot();
port.onMessage.addListener(function(response)
{
	if(response.load)
	{
		eval(response.load);
		pBot.onLoaded();
	}
	else if(response.output)
	{
		pBot.onExec(response);
	}
	else if(response.event,response.data);
	{
	pBot.trigger(response.event,{sender:response.sender,data:response.data});
	}
	else if(response.file)
	{
		pbot.onRequireFile(file);
	}
	else if(response.error)
	{
		pbot.onErr(response);
	}
});
};