pBot.onLoad(function()
{
alert("loaded");
pBot.exec("ls",function(data)
{
	alert(JSON.stringify(data.output));
});

});
