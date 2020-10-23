var aButtonScan = wificfg.scan, 
    aButtonReboot = wificfg.wifireboot,
	aButtonSave = wificfg.wifisave,
    aSelect = wificfg.selSSID,
	aKey    = wificfg.wifiPwd,
	aModule = wificfg.mName,
	aAP     = wificfg.APEnable;
	
function WiFiScan()
{
	var request = new XMLHttpRequest();
	request.open('POST','/wifiscan',true);
	request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	request.addEventListener('readystatechange', function() {
		if ((request.readyState==4) && (request.status==200)) {
			while (aSelect.options[aSelect.options.selectedIndex])
			{
				aSelect.remove(aSelect.options.selectedIndex);
			}
			var myObj = JSON.parse(this.responseText);
			for (x in myObj) {
				var newOption = new Option(myObj[x].key, myObj[x].key);
				aSelect.options[aSelect.options.length]=newOption;
			}
		}
	});
	request.send("SCAN WIFI");
}

function WiFiReboot()
{
	var request = new XMLHttpRequest();
	request.open('POST','/wifireboot',true);
	request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	request.addEventListener('readystatechange', function() {
		if ((request.readyState==4) && (request.status==200)) {
			alert("Устройство перезагружается.");
		}
	});
	request.send("REBOOT WIFI");
}

function WiFiSave()
{
	var request = new XMLHttpRequest();
	request.open('POST','/wifisave',true);
	request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	request.addEventListener('readystatechange', function() {
		if ((request.readyState==4) && (request.status==200)) {
			alert("Конфигурация сохранена.");
		}
	});
	var cfg = "{\nSSID:"+aSelect.options[aSelect.options.selectedIndex].value+"\n"+
			  "SSIDkey:"+aKey.value+"\n"+
			  "MOD:"+aModule.value+"\n";
	if (aAP.checked) //alert("Y"); else alert("N");
			  cfg = cfg + "AP:Y\n";
	else      cfg = cfg + "AP:N\n"
	cfg = cfg + "}\n";
	request.send(cfg);
}

aButtonScan.addEventListener("click", WiFiScan);
aButtonReboot.addEventListener("click", WiFiReboot);
aButtonSave.addEventListener("click", WiFiSave);