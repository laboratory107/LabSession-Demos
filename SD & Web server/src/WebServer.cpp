#include "WebServer.h"

WebServer::WebServer() { server = new AsyncWebServer(80); }

WebServer::~WebServer() {}

void WebServer::setupWifi(String WiFiSSID, String WiFiPassword) {
	WiFi.mode(WIFI_STA);
	WiFi.begin(WiFiSSID.c_str(), WiFiPassword.c_str());
	sdLog->writeLine("Connecting to WiFi...");
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		sdLog->writeLine(".", false);
		delay(600);
	}

	sdLog->writeLine("Wifi connected, IP Address: " +
					 WiFi.localIP().toString());
}

void WebServer::setupMdns() {
	WiFi.setHostname("smartlock");
	if (!MDNS.begin("smartlock")) {
		sdLog->writeLine("MDNS error");
		while (1) {
			delay(1000);
		}
		// TODO: http:// mora biti
	}

	sdLog->writeLine("MDNS name: " + serverName);
}

void WebServer::setupHomepage() {
	server->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
		sdLog->writeLine("new request ");
		request->send(200, "text/html",
					  SD.open("/WebManager/index.html").readString());
	});
}

void WebServer::setup404Page() {
	server->onNotFound([&](AsyncWebServerRequest *request) {
		request->send(404, "text/plain", "Not found");
		sdLog->writeLine("404 request ");
	});
}

void WebServer::registerWebFile(String pageName) {
	server->on(pageName.c_str(), HTTP_GET, [&](AsyncWebServerRequest *request) {
		sdLog->writeLine("new request " + request->url());
		String content = SD.open("/WebManager" + request->url()).readString();

		if (request->url().equals("/log.html")) {
			if (request->hasArg("clear")) {
				sdLog->writeLine("log clear requested", false);
				sdLog->clear();
			}
			String logContent = "id=\"log\">";
			logContent.concat(sdLog->readAll());
			content.replace("id=\"log\">", logContent);
		}
        
		if (request->url().equals("/manager.html")) {
			File fileKeys = SD.open("/keys.txt", FILE_READ);
			String keyContent = "id=\"keys\">";
			keyContent.concat(fileKeys.readString());
			fileKeys.close();
			content.replace("id=\"keys\">", keyContent);
		}

		if (request->url().equals("/save.html")) {
			String saveMe = "";
			for (size_t i = 0; i < request->params(); i++) {
				saveMe.concat(request->getParam(i)->name());
				saveMe.concat("=");
				saveMe.concat(request->getParam(i)->value());
				saveMe.concat("\n");
			}
			File fileKeys = SD.open("/keys.txt", FILE_WRITE);
			fileKeys.print(saveMe);
			fileKeys.close();
		}
		request->send(200, "text/html", content);
	});
}

void WebServer::enumerateHTMLFiles() {
	File folder = SD.open(this->path, FILE_READ);
	folder.rewindDirectory();
	String on = "";
	while (true) {
		File entry = folder.openNextFile();
		if (!entry) {  // No more files
			break;
		}

		sdLog->writeLine("new file " + String(entry.name()));
		on = "/";
		on.concat(entry.name());
		on = on.substring(on.lastIndexOf("/"));

		sdLog->writeLine("file registration: " + on);

		registerWebFile(on);
		entry.close();
	}
}

bool WebServer::begin(String ServerName, String RootPath, String WiFiSSID,
					  String WiFiPassword, SDLogger *logger) {
	sdLog = logger;
	this->serverName = ServerName;
	this->path = RootPath;

	setupWifi(WiFiSSID, WiFiPassword);
	setupMdns();

	enumerateHTMLFiles();
	setup404Page();
	setupHomepage();

	sdLog->writeLine("Server setup done");
	return true;
}

bool WebServer::start() {
	server->begin();
	sdLog->writeLine("Server started");
	return false;
}

bool WebServer::stop() {
	server->end();
	sdLog->writeLine("Server stopped");
	return false;
}