function UpdateState(mdid)
{

}

function LoadTabPage(tabid)
{
var request = new XMLHttpRequest();

request.open('POST','/ctrlload',true);
request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
if (tabid == 1){
  request.addEventListener('readystatechange', function() {
  if ((request.readyState==4) && (request.status==200)) {
    var txt = "<table border=\"1\" cellspacing=\"0\" style=\"text-align: center;margin-left: 0%;width:100%\"><tr height=\"30\"><td width=\"25%\">Device Type</td><td width=\"10%\">Module ID</td><td width=\"10%\">Температура</td><td width=\"20%\">Влажность</td>";
    txt += "<td width=\"10%\">Состояние</td><td width=\"30%\">Last Key</td><td width=\"5%\"></td></tr></tr>";
    //console.log(this.responseText);
    myObj = JSON.parse(this.responseText);
    for (x in myObj) {
      txt += "<tr height=\"30\">"; 
      switch(myObj[x].DevCode) // Device Type
      {
					case 0: txt += "<td>Protocol</td>";break; // Protocol
					case 1: txt += "<td>RF 433Mhz key</td>";break; // RF433
					case 2: txt += "<td>Relay</td>";break; // Relay
					case 3: txt += "<td>DHT sensor</td>";break; // DHT sensor
					case 4: txt += "<td>PIR sensor</td>";break; // PIR sensor
					default: "<td>" +myObj[x].DevCode+"</td>"
      }
					txt += "<td>" +myObj[x].ModuleID+"</td>";
					switch(myObj[x].DevCode) //Температура
					{
					case 0: txt += "<td></td>";break; // Protocol
					case 1: txt += "<td></td>";break; // RF433
					case 2: txt += "<td></td>";break; // Relay
					case 3: txt += "<td>" + myObj[x].Temp + "</td>";break; // DHT sensor
					case 4: txt += "<td></td>";break; // PIR sensor
					}
					switch(myObj[x].DevCode) //Влажность
					{
					case 0: txt += "<td></td>";break; // Protocol
					case 1: txt += "<td></td>";break; // RF433
					case 2: txt += "<td></td>";break; // Relay
					case 3: txt += "<td>" + myObj[x].Hum + "</td>";break; // DHT sensor
					case 4: txt += "<td></td>";break; // PIR sensor
					}
					switch(myObj[x].DevCode) //Состояние
					{
					case 0: txt += "<td></td>";break; // Protocol
					case 1: txt += "<td></td>";break; // RF433
					case 2: txt += "<td>" + myObj[x].State + "</td>";break; // Relay
					case 3: txt += "<td></td>";break; // DHT sensor
					case 4: txt += "<td>" + myObj[x].State + "</td>";break; // PIR senso
					}
					switch(myObj[x].DevCode) //Key
					{
					case 0: txt += "<td></td>";break; // Protocol
					case 1: txt += "<td>" + myObj[x].LastKey + "</td>";break; // RF433
					case 2: txt += "<td></td>";break; // Relay
					case 3: txt += "<td></td>";break; // DHT sensor
					case 4: txt += "<td></td>";break; // PIR senso
					}
					txt += "<td><input type=\"button\" name=\"operate\" value=\"Х\" style=\"width:28px;\" onclick=\"UpdateState("+myObj[x].ModuleID+");\"/></td>"; // button
					txt += "</tr>";
				}
				txt += "</table>";
				
				document.getElementById("sensValue").innerHTML = txt;
			}
		});
	} else {
		request.addEventListener('readystatechange', function() {
			if ((request.readyState==4) && (request.status==200)) {
				var txt = "<table border=\"1\" cellspacing=\"0\" style=\"text-align: center;margin-left: 0%;width:100%\"><tr height=\"30\"><td width=\"25%\">Device Type</td><td width=\"10%\">Module ID</td><td width=\"10%\">Pin</td>";
					txt += "<td width=\"30%\">Key</td><td width=\"20%\">State</td><td width=\"5%\"></td></tr></tr>";
				//console.log(this.responseText);
				myObj = JSON.parse(this.responseText);
				for (x in myObj) {
					txt += "<tr height=\"30\">"; 
					switch(myObj[x].DevCode) // Device Type
					{
					case 0: txt += "<td>Protocol</td>";break; // Protocol
					case 1: txt += "<td>RF 433Mhz key</td>";break; // RF433
					case 2: txt += "<td>Relay</td>";break; // Relay
					default: "<td>" +myObj[x].DevCode+"</td>"
					}
					txt += "<td>" +myObj[x].ModuleID+"</td>";
					switch(myObj[x].DevCode) //Pin
					{
					case 0: txt += "<td></td>";break; // Protocol
					case 1: txt += "<td>" + myObj[x].Pin + "</td>";break; // RF433
					case 2: txt += "<td>" + myObj[x].Pin + "</td>";break; // Relay
					}
					switch(myObj[x].DevCode) //Key
					{
					case 0: txt += "<td></td>";break; // Protocol
					case 1: txt += "<td></td>";break; // RF433
					case 2: txt += "<td>" + myObj[x].Key + "</td>";break; // Relay
					}
					switch(myObj[x].DevCode) //State
					{
					case 0: txt += "<td></td>";break; // Protocol
					case 1: txt += "<td></td>";break; // RF433
					case 2: txt += "<td>" + myObj[x].State + "</td>";break; // Relay
					}
					txt += "<td><input type=\"button\" name=\"operate\" value=\"Х\" style=\"width:28px;\" onclick=\"UpdateState("+myObj[x].ModuleID+");\"/></td>"; // button
					txt += "</tr>";
				}
				txt += "</table>";
				document.getElementById("execValue").innerHTML = txt;
			}
		});
	}
	var req = "{\ntabpage:"+tabid+"\n}";
	request.send(req);
}

function OnClick(tabhid){
 var tabh = document.getElementById(tabhid);
console.log(tabhid);
 if (tabh==null) return;
 
 var tabheader=tabh.parentNode;

 if (tabheader==null) return;
 
 var tabhs;
 tabhs = SelectChildren(tabheader,'div');

 if (tabhs.length == 0) return;
 
for(var i = 0; i < tabhs.length; i++)
     if (tabh.style.backgroundColor = '#eee')
        tabhs[i].style.backgroundColor = '#fff';
 
 tabh.style.backgroundColor = '#eee';

 var tabcontrol;
 
 try {
	 tabcontrol = tabheader.parentNode;
 } catch (ex)
 {
	 return;
 }

 var tabbody;
 
 try {
	 tabbody = tabcontrol.getElementsByClassName('tab-body')[0];
 } catch (ex)
 {
	 return;
 }
 
 var tabs;
 tabs = SelectChildren(tabbody,'div');

 if (tabs.length == 0)
     return;

 var selectedtab= document.getElementById('b'+tabh.id);

 if (selectedtab==null)
     return; 

 for(var i = 0; i < tabs.length; i++)
     if (tabs[i].style.display == 'block')
        tabs[i].style.display = 'none';

 selectedtab.style.display = 'block';
 LoadTabPage(tabhid.substring(tabhid.length - 1));
}

function SelectChildren(node, childname)
{
 var result = [];

 for(var i = 0; i < node.childNodes.length; i++)
    {
     var child = node.childNodes[i];
     if (child.nodeType==1) //Элемент
        if (child.tagName.toLowerCase()==childname.toLowerCase())
           result.push(child); 
    }

 return result; 
}
