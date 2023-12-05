#include "SDLogger.h"

SDLogger::SDLogger() {}

SDLogger::~SDLogger() {
	fileWrite.close();
	fileRead.close();
}

bool SDLogger::begin(String FileName) {
	isReady = false;
	if (!SD.begin()) {
		Serial.println("SD begin failed");
		return isReady;
	}
	this->fileName = FileName;
	isReady = true;
	return isReady;
}

void SDLogger::writeLine(String logEntry, bool saveToCard) {
	Serial.println("Log: " + logEntry);
	if (saveToCard && isReady) {
		logEntry.concat("\n");
		logEntry.concat(readAll());

		fileWrite = SD.open(this->fileName, FILE_WRITE);
		fileWrite.print(logEntry);
		fileWrite.close();
	}
}

String SDLogger::readAll() {
	String data = "";
	if (!isReady) {
		return data;
	}

	fileRead = SD.open(this->fileName, FILE_READ);
	data.concat(fileRead.readString());
	fileRead.close();

	return data;
}

void SDLogger::clear() {
	if (!isReady) {
		return;
	}

	fileWrite = SD.open(this->fileName, FILE_WRITE);
	fileWrite.println("");
	fileWrite.close();
}