#include <WebServer.h>

WebServer server(80);

File fsUploadFile;

void setupWebServer() {
  server.on("/status", HTTP_GET, []() {
    if (hasSSIDStored()) {
      readFile("/disconnect.html");
    } else {
      readFile("/connect.html");
    }
  });
  server.on("/stats/restart-count", HTTP_GET, []() {
    server.send(200, "text/html", String(preferences.getUInt("counter", 0)));
  });
  server.on("/stats/node-id", HTTP_GET, []() {
    server.send(200, "text/html", nodeId());
  });
  server.on("/wifi", HTTP_DELETE, []() {
    unsetStorage();
    readFile("/connect.html");
  });
  server.on("/wifi", HTTP_POST, []() {
    if (!server.hasArg("ssid") || server.arg("ssid").isEmpty()) {
      server.sendHeader("HX-Retarget", "#error-message");
      server.sendHeader("HX-Reswap", "innerHTML");
      return server.send(200, "text/html", hError("Error: You are missing the Wifi name."));
    }
    if (!server.hasArg("password") || server.arg("password").isEmpty()) {
      server.sendHeader("HX-Retarget", "#error-message");
      server.sendHeader("HX-Reswap", "innerHTML");
      return server.send(200, "text/html", hError("Error: Your Password is empty."));
    }
    String ssid = server.arg("ssid");
    ssid.trim();
    String password = server.arg("password");
    password.trim();
    WiFi.begin(ssid, password);
    uint8_t status = WiFi.waitForConnectResult(3000);

    Serial.print("Wifi Status: ");
    Serial.println(status);

    if(status == WL_NO_SSID_AVAIL) {
      server.sendHeader("HX-Retarget", "#error-message");
      server.sendHeader("HX-Reswap", "innerHTML");
      return server.send(200, "text/html", hError("Error: We couldn't find that Wifi name."));
    }
    
    if(status == WL_CONNECT_FAILED || status == WL_DISCONNECTED) {
      server.sendHeader("HX-Retarget", "#error-message");
      server.sendHeader("HX-Reswap", "innerHTML");
      return server.send(200, "text/html", hError("Error: Password was not accepted."));
    }

    setWifiConnection(ssid, password);
    readFile("/disconnect.html");
  });
  server.on("/list", HTTP_GET, []() {
    if (!server.hasArg("dir")) {
      server.send(500, "text/plain", "BAD ARGS");
      return;
    }
    String path = server.arg("dir");
    File root = LittleFS.open(path);
    path = String();
    String output = "";
    if (root.isDirectory()) {
      File file = root.openNextFile();
      while (file) {
        output += "{\"type\":\"";
        output += (file.isDirectory()) ? "dir" : "file";
        output += "\",\"name\":\"";
        output += String(file.path()).substring(1);
        output += "\"}\n";
        file = root.openNextFile();
      }
    }
    server.send(200, "text/jsonl", output);
  });
  server.on(
    "/file", HTTP_POST, []() {
      okResponse();
    },
    []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;
        if (!filename.startsWith("/")) {
          filename = "/" + filename;
        }
        fsUploadFile = LittleFS.open(filename, "w");
        return;
      }
      if (!fsUploadFile) {
        return;
      }
      if (upload.status == UPLOAD_FILE_WRITE) {
        fsUploadFile.write(upload.buf, upload.currentSize);
        return;
      }
      if (upload.status == UPLOAD_FILE_END) {
        fsUploadFile.close();
        return;
      }
    });
  server.on("/file", HTTP_GET, []() {
    if (!server.hasArg("path")) {
      server.send(500, "text/plain", "BAD ARGS");
      return;
    }
    if (!readFile(server.arg("path"))) {
      fileNotFound();
    }
  });
  server.on("/file", HTTP_DELETE, []() {
    if (!server.hasArg("path")) {
      server.send(500, "text/plain", "BAD ARGS");
      return;
    }
    if (!LittleFS.exists(server.arg("path"))) {
      fileNotFound();
    }
    LittleFS.remove(server.arg("path"));
    okResponse();
  });
  server.onNotFound([]() {
    if (!readFile("/index.html")) {
      fileNotFound();
    }
  });
  server.begin();
}

void okResponse() {
  server.send(200, "text/plain", "OK");
}

void fileNotFound() {
  server.send(404, "text/plain", "File Not Found");
}

bool readFile(String path) {
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    server.streamFile(file, getContentType(path, server.hasArg("download")));
    file.close();
    return true;
  }
  return false;
}

void stepWebServer() {
  server.handleClient();
}