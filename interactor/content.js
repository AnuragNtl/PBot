var sender;
function pbot(){}
pbot.prototype.events={};
	pbot.prototype.onLoad=function(k)
	{
		this.onLoaded=k;
	}
	pbot.prototype.onOutputReceived=function(f)
	{
		this.onOutput=f;
	}
	pbot.prototype.on=function(event,onEvent)
	{
		this.events[event]=onEvent;
	}
pbot.prototype.exec=function(path,onExec)
{
	browser.runtime.sendMessage({sender:sender,msgType:1,data:path});
	this.onExec=onExec;
}
pbot.prototype.sendEvent=function(to,receiver,event,data)
{
	browser.runtime.sendMessage({sender:"general",msgType:3,receiver:to,extras:{receiver:receiver,event:event,data:data},data:""});
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
	browser.runtime.sendMessage({sender:sender,msgType:4,data:"",extras:{file:file}});
	this.onRequireFile=onFetch;
}
pbot.prototype.onError=function(onErr)
{
	this.onErr=onErr;
}
var pBot=new pbot();

browser.runtime.onMessage.addListener(function(response)
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
	else if(response.event)
	{
	pBot.trigger(response.event,{sender:response.sender,data:response.data});
	}
	else if(response.file)
	{
		pBot.onRequireFile(response.file);
	}
	else if(response.error)
	{
		pbot.onErr(response);
	}
});

//document.onload=function()
{
			sender=window.top.location.href.match(/:\/\/.+?\//)[0].replace(/\/|:/g,"");
		browser.runtime.sendMessage({sender:sender,msgType:0,data:""});
}
