<h1>PBot<h1>
<h3>Extension and Command Line tools to make webpages in firefox browser interact with underlying platform and shell.<h3>
<p>Using a simple JavaScript API, give WebPages power to be controlled by external scripts:</p>
<h2>How to Use</h2>
Run :
<pre>
$ ./PBotConfig
	PBot Configuration:
Enter Script Base Path
/home/administrator/PBotScripts

</pre>
<br>
Then to add Script for a webpage:
<a href="#howToCreate">How to Write Scripts<a>
<pre>
$ bash AddScript.sh
Add Script
Enter Script Path: /tmp/T1.js
Enter WebSite: www.google.com
Installed Script
</pre>
<br>
Then Run:
<pre>$ ./PBotManager</pre> to run the server.
Load webpages in firefox and the scripts for those pages will be loaded.

<div id="howToCreate">
<h2>How to write scripts</h2>
<p>All script needs to be Stored in <pre>pBot.onLoad(function()
{
/*
JavaScript to Run on WebPages
For Example: alert("Welcome to PBot");
*/
});</pre>
</p>
<p>
Use <pre>pBot.exec(executable_path:string,callback)</pre> to execute shell commands.<br>
executable_path: The command to execute.
callback : A function that receives the object with the following keys:<br>
<ul><li>output:the output of command</li>
<li>err_code: the error code of the command</li>

</p>
<p>
Make use of pBot.onError() that receives a function with error details as parameter to catch the errors.
</p>
<p>
Use : pBot.requireFile(javascript_file_path,callback) to import external data.
</p>
<p>
pBot.on(event,callback) is triggered when PBotClient program is executed with the following arguments:
<pre>
$ ./PBotClient --help
PBotClient:
Usage: PBotClient RECEIVER EVENT_NAME JSON_DATA

</pre>
RECEIVER is the website for which script is executing
EVENT_NAME and JSON_DATA are passed to callback of pBot.on() function
</p>

</div>
