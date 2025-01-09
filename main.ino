#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Arduino.h>
#include <ArduinoJson.h>


#define LEDON 0x0
#define LEDOFF 0x1

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Setup</title>
        <style>
            * {
                --text: #ecf1f5;
                --background: #09131b;
                --primary: #90b9dd;
                --secondary: #125d9f;
                --accent: #3e29a4;

                --font: Arial, Helvetica, sans-serif;
                --border-radius: 7px;

                margin: 0;
                padding: 0;
                border: 0 solid transparent;
            }
            body {

                min-height: 100vh;
                line-height: 1;
                /*text-rendering: optimizeSpeed;*/

                background-color: var(--background);
                font-family: var(--font);
                color: var(--text);
                
                display: flex;
                justify-content: center;
            }

            h1 {
                font-weight: bold;
                font-size: min(15vw, 36px);
                
            }
            
            button {
                border-radius: var(--border-radius);
            }

            input {
                border-radius: var(--border-radius);
                margin: 0.5vh;
            }

            input:focus {
                transition: 0.2s;
                outline: 3px solid var(--secondary);
            }

            #main {
                margin-top: 5vh;
                width: 90vw;
                text-align: center;
                transition: 0.2s;
            }

            #submit {
                color: var(--background);
                background-color: var(--primary);
                
                cursor: pointer;
                
            }

            .textfield {
                min-height: 32px;
                height: 32px;
                padding-left: 3px;
                padding-right: 3px;
                outline: 0px solid var(--secondary);
                transition: 0.2s;
            }

            
            /* mobile */
            @media (max-aspect-ratio: 5/4) {
                .textfield {
                    height: 50px;
                    width: 85vw;
                    font-size: 22px;
                    padding: 0;
                    text-indent: 2vw;
                }

                #submit {
                    width: 85vw;
                    font-size: 18px;
                    height: 50px; 
                    padding: 0;
                }
            }
            /* pc */
            @media (min-aspect-ratio: 5/4) {
                h1 {
                    font-size: 6vw;
                }

                .textfield {
                    width: 18vw;
                    height: 4.5vw;
                    font-size: 2vw;
                }

                #submit {
                    width: 15vw;
                    height: 5vw;
                    font-size: 2vw;
                }
            }
            
        </style>
    </head> 
    <body>
        <div id="main">
            <h1>Velkommen til setup!</h1>

            <form id="wifi-info" action="/connect" method="post">
                <br>
                <input class="input textfield" id="ssid" type="text" name="ssid" placeholder="Wifi navn" tabindex="1"><br>
                <input class="input textfield" id="pass" type="text" name="pass" placeholder="Wifi kode"><br>
                <br>
                <input class="input btn" id="submit" type="submit" name="submit" value="Indtast">
            </form>
        </div>
        
    </body>
</html>
)rawliteral";

const char connect_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Tilslutter...</title>
      <style>
          * {
              --text: #ecf1f5;
              --background: #09131b;
              --primary: #90b9dd;
              --secondary: #125d9f;
              --accent: #3e29a4;

              --font: Arial, Helvetica, sans-serif;
              --border-radius: 7px;

              margin: 0;
              padding: 0;
              border: 0 solid transparent;
          }
          body {

              min-height: 100vh;
              line-height: 1;
              /*text-rendering: optimizeSpeed;*/

              background-color: var(--background);
              font-family: var(--font);
              color: var(--text);
              
              display: flex;
              justify-content: center;
          }

          h1 {
              font-weight: bold;
              font-size: min(15vw, 36px);
              
          }
          
          button {
              border-radius: var(--border-radius);
          }

          input {
              border-radius: var(--border-radius);
              margin: 0.5vh;
          }

          input:focus {
              transition: 0.2s;
              outline: 3px solid var(--secondary);
          }

          #main {
              margin-top: 5vh;
              width: 90vw;
              text-align: center;
              transition: 0.2s;
          }

          #submit {
              color: var(--background);
              background-color: var(--primary);
              
              cursor: pointer;
              
          }

          .textfield {
              min-height: 32px;
              height: 32px;
              padding-left: 3px;
              padding-right: 3px;
              outline: 0px solid var(--secondary);
              transition: 0.2s;
          }

          
          /* mobile */
          @media (max-aspect-ratio: 5/4) {
              .textfield {
                  height: 50px;
                  width: 85vw;
                  font-size: 22px;
                  padding: 0;
                  text-indent: 2vw;
              }

              #submit {
                  width: 85vw;
                  font-size: 18px;
                  height: 50px; 
                  padding: 0;
              }
          }
          /* pc */
          @media (min-aspect-ratio: 5/4) {
              h1 {
                  font-size: 6vw;
              }

              .textfield {
                  width: 18vw;
                  height: 4.5vw;
                  font-size: 2vw;
              }

              #submit {
                  width: 15vw;
                  height: 5vw;
                  font-size: 2vw;
              }
          }
          
      </style>
  </head>
  <body>
      <div id="main">
          <h1>
              Wifi navn og kode er registreret.
          </h1>
          <br>
          <h2 id="connecting">Tilslutter om  sekunder</h2>
      </div>

      <script>
          let countdown = 10;
          const heading = document.getElementById("connecting");

          heading.textContent = "Prøver at tilslutte om " + countdown + " sekunder!";

          const timer = setInterval(() => {
              countdown--;
              if (countdown > 0)
          {
          
          heading.textContent = "Prøver at tilslutte om " + countdown + " sekunder!";
          } else {
              heading.textContent = "Setup slukker og prøver at tilslutte";
              clearInterval(timer);
          }
      }, 1000);
      
      </script>
  </body>
</html>
)rawliteral";

const char connect_fejl_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Fejl</title>
      <style>
          * {
              --text: #ecf1f5;
              --background: #09131b;
              --primary: #90b9dd;
              --secondary: #125d9f;
              --accent: #3e29a4;

              --font: Arial, Helvetica, sans-serif;
              --border-radius: 7px;

              margin: 0;
              padding: 0;
              border: 0 solid transparent;
          }
          body {

              min-height: 100vh;
              line-height: 1;
              /*text-rendering: optimizeSpeed;*/

              background-color: var(--background);
              font-family: var(--font);
              color: var(--text);
              
              display: flex;
              justify-content: center;
          }

          h1 {
              font-weight: bold;
              font-size: min(15vw, 36px);
              
          }

          #fejl {
              color: red;
          }
          
          button {
              border-radius: var(--border-radius);
          }

          input {
              border-radius: var(--border-radius);
              margin: 0.5vh;
          }

          input:focus {
              transition: 0.2s;
              outline: 3px solid var(--secondary);
          }

          #main {
              margin-top: 5vh;
              width: 90vw;
              text-align: center;
              transition: 0.2s;
          }

          #submit {
              color: var(--background);
              background-color: var(--primary);
              
              cursor: pointer;
              
          }

          .textfield {
              min-height: 32px;
              height: 32px;
              padding-left: 3px;
              padding-right: 3px;
              outline: 0px solid var(--secondary);
              transition: 0.2s;
          }

          a {
              font-weight: bold;
              color: var(--primary);
              text-decoration: underline overline;
              font-size: 3vw;
          }

          
          /* mobile */
          @media (max-aspect-ratio: 5/4) {
              .textfield {
                  height: 50px;
                  width: 85vw;
                  font-size: 22px;
                  padding: 0;
                  text-indent: 2vw;
              }

              #submit {
                  width: 85vw;
                  font-size: 18px;
                  height: 50px; 
                  padding: 0;
              }
          }
          /* pc */
          @media (min-aspect-ratio: 5/4) {
              h1 {
                  font-size: 6vw;
              }

              .textfield {
                  width: 18vw;
                  height: 4.5vw;
                  font-size: 2vw;
              }

              #submit {
                  width: 15vw;
                  height: 5vw;
                  font-size: 2vw;
              }
          }
          
      </style>
  </head>
  <body>
      <div id="main">
          <h1>
              Der er fejl i dataen indtastet. 
          </h1>
          <br>
          <h2 id="fejl1">Måske mangler du at skrive adgangskoden eller navnet</h2>
          <h2 id="fejl2">gå venligst tilbage til setup siden</h2>
          <br>
          <a href="/">her</a>
      </div>

      
  </body>
</html>
)rawliteral";

const char client_index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Tilsluttet!</title>
      <style>
          * {
              --text: #ecf1f5;
              --background: #09131b;
              --primary: #90b9dd;
              --secondary: #125d9f;
              --accent: #3e29a4;

              --font: Arial, Helvetica, sans-serif;
              --border-radius: 7px;

              margin: 0;
              padding: 0;
              border: 0 solid transparent;
          }
          body {

              min-height: 100vh;
              line-height: 1;
              /*text-rendering: optimizeSpeed;*/

              background-color: var(--background);
              font-family: var(--font);
              color: var(--text);
              
              display: flex;
              justify-content: center;
          }

          h1 {
              font-weight: bold;
              font-size: min(15vw, 36px);
              
          }

          p {
              font-size: 7vw;
          }
          
          button {
              border-radius: var(--border-radius);
          }

          input {
              border-radius: var(--border-radius);
              margin: 0.5vh;
          }

          input:focus {
              transition: 0.2s;
              outline: 3px solid var(--secondary);
          }

          #main {
              margin-top: 5vh;
              width: 90vw;
              text-align: center;
              transition: 0.2s;
          }

          #submit {
              color: var(--background);
              background-color: var(--primary);
              
              cursor: pointer;
              
          }

          .textfield {
              min-height: 32px;
              height: 32px;
              padding-left: 3px;
              padding-right: 3px;
              outline: 0px solid var(--secondary);
              transition: 0.2s;
          }

          
          /* mobile */
          @media (max-aspect-ratio: 5/4) {
              .textfield {
                  height: 50px;
                  width: 85vw;
                  font-size: 22px;
                  padding: 0;
                  text-indent: 2vw;
              }

              #submit {
                  width: 85vw;
                  font-size: 18px;
                  height: 50px; 
                  padding: 0;
              }
          }
          /* pc */
          @media (min-aspect-ratio: 5/4) {
              h1 {
                  font-size: 6vw;
              }

              p {
                  font-size: 2vw;
              }

              .textfield {
                  width: 18vw;
                  height: 4.5vw;
                  font-size: 2vw;
              }

              #submit {
                  width: 15vw;
                  height: 5vw;
                  font-size: 2vw;
              }
          }
          
      </style>
  </head> 
  <body>
      <div id="main">
          <h1>Tilsluttet til netværket!</h1>

          <p>Nu skal du downloade klient programmet <a href="https://github.com/penewold/crazy/releases/download/test/crazyhello.exe" download="dørklokke-setup">her</a></p>
          
      </div>
  </body>
</html>
)rawliteral";


const char *ApSsid = "setup";
String ssid;
String pass;
bool debounce = false;
unsigned long prevTime;

unsigned long lastClickedTime = 0;
int stage = 1;

void initStage1Server();
void initMDNS(String hostName);
void checkTimeElapsed();
void setStage2();
String checkReturnCode(int returnCode);
void handleStage1Root();
void handleConnectPOST();
void handleConnectGET();
void handleStage2Root();
void handleStage2Klokke();
String constructJson(unsigned long currentTime, unsigned long previousTime);

ESP8266WebServer server(80);
ESP8266WebServer stage2Server(80);


void setup() {
  //pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);

  Serial.println("\n\n[stage1][WifiAP] configuring ap");
  WiFi.softAP(ApSsid);

  lastClickedTime = millis();

  initMDNS("setup");

  initStage1Server();
}

// the loop function runs over and over again forever
void loop() {
  if (stage == 1) {
    server.handleClient();
    MDNS.update();
      
    

    if (ssid != "" && pass != "") {
      checkTimeElapsed();
    }

  } else if (stage == 2) {
    stage2Server.handleClient();
    MDNS.update();
  }
}

void initStage1Server() {
  
  server.on("/", handleStage1Root);
  Serial.println("[stage1][webserver] handleStage1Root set up on /");
  server.on("/connect", HTTP_POST, handleConnectPOST);
  Serial.println("[stage1][webserver] handleConnectPOST set up on /connect");
  server.on("/connect", HTTP_GET, handleConnectGET);
  Serial.println("[stage1][webserver] handleConnectGET set up on /connect");
  server.begin();
}

void initMDNS(String hostName) {
  
  MDNS.addService("http", "tcp", 80);  // Registers _http._tcp.local

  if (!MDNS.begin(hostName)) {
    Serial.println("Error starting mDNS on stage " + stage);
  }
  Serial.print("[stage");
  Serial.print(stage);
  Serial.print("][mDNS] mDNS started on ");
  Serial.print(hostName);
  Serial.println(".local");
}

// GOAL: Check when 10 seconds has passed using millis()
void checkTimeElapsed() {
  unsigned long curTime = millis();
  if (!debounce) {
    prevTime = millis();
    debounce = true;
  } else if (curTime > (prevTime + 10 * 1000)) {
    Serial.println("[stage1/2][wifiClient] Trying to connect to the network");
    setStage2();
    /*
    * TODO: change stage to 2.
    * Switch wifi to STA mode.
    * Try connecting to selected network. 
    * Incase it fails switch back to stage 1.
    * Incase it's successful start the webserver with user config and client api
    */
    Serial.println("triggered");
    debounce = false;
  }
}

void setStage2() {
  Serial.println("[stage2][init] setting stage 2");
  stage = 2;
  Serial.println("[stage2][init] closing previous server");
  server.close();
  
  Serial.println("[stage2][init] disconnecting ap and mDNS");
  WiFi.softAPdisconnect(true);
  MDNS.close();

  
  Serial.println("[stage2][init] connecting to wifi");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("[stage2][connecting] code: " + checkReturnCode(WiFi.status()));
    delay(500);
  }

  IPAddress staticIP = WiFi.localIP();          // Use current IP
  staticIP[2] = 50;                             // Adjust next to last octet to a known ip
  staticIP[3] = 200;                            // Adjust last octet for uniqueness
  IPAddress gateway = WiFi.gatewayIP();         // Use the current gateway
  IPAddress subnet = WiFi.subnetMask();         // Use the current subnet mask
  IPAddress dns = WiFi.gatewayIP();             // Use gateway as DNS, or set a custom one like 8.8.8.8

  WiFi.disconnect();
  delay(1000);
  if (WiFi.config(staticIP, gateway, subnet, dns)) {
    Serial.println("[stage2][static ip config] Static IP configured successfully");
  } else {
    Serial.println("[stage2][static ip config] Failed to configure static IP");
  }
  
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("[stage2][connecting] code: " + checkReturnCode(WiFi.status()));
    delay(500);
  }

  Serial.println("[stage2][wifi] connected to: " + WiFi.SSID());
  Serial.println("[stage2][wifi] connected with ip: " + WiFi.localIP().toString());
  

  initMDNS("klokke");

  stage2Server.on("/", handleStage2Root);
  Serial.println("[stage2][webserver] handleStage2Root set up on /");
  stage2Server.on("/klokke", handleStage2Klokke);
  Serial.println("[stage2][webserver] handleStage2Klokke set up on /klokke");
  stage2Server.begin();
}

String checkReturnCode(int returnCode) {
  switch (returnCode) {
    case 0:
      return "WL_IDLE_STATUS";
    case 1:
      return "WL_NO_SSID_AVAIL";
    case 3:
      return "WL_CONNECTED";
    case 4:
      return "WL_CONNECT_FAILED";
    case 6:
      return "WL_CONNECT_WRONG_PASSWORD";
    case 7:
      return "WL_DISCONNECTED";
    default:
      return "unrecognized";
  }
}

// functions for handling stage 1

void handleStage1Root() {
  Serial.println("[stage1][webserver] Received request on /");
  server.send(200, "text/html", index_html);
}

void handleConnectPOST() {
  Serial.println("[stage1][webserver] Received POST request on /connect");
  if (server.arg("ssid") != "" && server.arg("pass") != "") {
    server.send(200, "text/html", connect_html);

    ssid = server.arg("ssid");
    pass = server.arg("pass");

    Serial.println("[stage1][webserver] ssid: " + ssid);
    Serial.println("[stage1][webserver] pass: " + pass);

    Serial.println("[stage1][webserver] waiting 10 sec then trying to connect to network");
  } else {
    server.send(400, "text/html", connect_fejl_html);
  }
}

void handleConnectGET() {
  Serial.println("[stage1][webserver] Received GET request on /connect");
  server.send(200, "text/html", connect_html);
}

// Functions for handling stage 2

void handleStage2Root() {
  Serial.println("[stage2][webserver] Received request on /");
  stage2Server.send(200, "text/html", client_index_html);
}

void handleStage2Klokke() {
  
  Serial.println("[stage2][webserver] Received request on /klokke");
  String data = constructJson(millis(), lastClickedTime);
  stage2Server.send(200, "application/json", data);
}

String constructJson(unsigned long currentTime, unsigned long previousTime) {
  // Create a JSON document
  StaticJsonDocument<200> jsonData;

  unsigned long diff = currentTime - previousTime;

  // Populate the JSON object
  jsonData["current_time"] = currentTime;
  jsonData["clicked_time"] = previousTime;
  jsonData["diff"] = diff;
  jsonData["format"] = "millis";

  String jsonString;
  serializeJson(jsonData, jsonString);
  Serial.println(jsonString);
  return jsonString;
}