#pragma once

#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SDLogger.h>

class WebServer {
   private:
	String serverName;
	String path;
	AsyncWebServer *server;
	SDLogger *sdLog;

	void setupMdns();
	void setupWifi(String ssid, String password);
	void setupHomepage();
	void setup404Page();
	void enumerateHTMLFiles();
	void registerWebFile(String pageName);

   public:
	WebServer();
	~WebServer();

	/// @brief
	/// @param serverName  including path, root is /
	bool begin(String serverName, String rootPath, String ssid,
			   String password, SDLogger *logger);
	bool start();
	bool stop();
};