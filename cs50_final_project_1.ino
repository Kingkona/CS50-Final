#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

//preprocessor constants
#define SENSOR1_PIN 4 // define GPIO pin Digital 1 (D1) for moisture sensor on Node MCU 
#define SENSOR2_PIN 5 // define GPIO pin D2 for moisture sensor
#define RELAY_PIN 16 // define GPIO pin D0 for relay

#define WIFI_SSID     "Plant Monitor" // SSID for WiFi network
#define WIFI_PASSWORD "ballsfish" // Passphrase for WiFi network

// Reference Material: 
// https://randomnerdtutorials.com/esp8266-nodemcu-async-web-server-espasyncwebserver-library/

AsyncWebServer server(80); // Create web server object

// boolean state variables for sensors
bool sensor1state = false; 
bool sensor2state = false;

// HTML for web interface; ensures data is stored in program memory
// .green and .red CSS classes to change change colors of moist/dry and on/off
const char html[] PROGMEM =
R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title> Plant Monitor </title>
    <meta http-equiv="refresh" content="1">
  </head>
  <style>
    body {
      font-family: Arial;
      text-align: center;
    }

    p {
      font-size: 2em;
    }

    h1 {
      font-size: 8em;
    }

    .green {
      color: green;
    }

    .red {
      color: red;
    }
  </style>
  <body>
    <h1> Plant Monitor </h1>
    %STATE_INFO%
  </body>
</html>
)rawliteral";

// Display data regarding sensor states
// String is a class 
String processState(const String& var)
{
  // Check to see if STATE_INFO template parameter was found (see %STATE_INFO% above)
  // This is similar to Jinja in Flask
  if(var == "STATE_INFO")
  {
    String state = "";
    // Check state of sensor 1 and add appropriate HTML based on the state
    if(sensor1state)
    {
      state += "<p> Pot Sensor #1 reports <b class=\"green\"><i>moist</i></b> </p>";
    }
    else
    {
      state += "<p> Pot Sensor #1 reports <b class=\"red\"><i>dry</i></b> </p>";
    }
    
    // Check state of sensor 2 and add appropriate HTML based on the state
    if(sensor2state)
    {
      state += "<p> Pot Sensor #2 reports <b class=\"green\"><i>moist</i></b> </p>";
    }
    else
    {
      state += "<p> Pot Sensor #2 reports <b class=\"red\"><i>dry</i></b> </p>";
    }

    // Compute the valve state in the same way as in void loop(), below,
    // ...and add appropriate HTML based on the state
    if(!(sensor2state || sensor1state))
    {
      state += "<p> The valve is <b class=\"green\"><i>on</i></b> </p>";
    }
    else
    {
      state += "<p> The valve is <b class=\"red\"><i>off</i></b> </p>";
    }

    return state; // Return HTML data to be filled into the template
  }

  return String(); // Return nothing if template is not state info
}

void setup()
{
  // Runs once at beginning to initialize code
  // Set sensor GPIO pins to input
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  // Set relay GPIO pin to output
  pinMode(RELAY_PIN, OUTPUT);
  // Start serial interface to 115200 baud rate
  Serial.begin(115200);

  // Print SSID over the serial port
  Serial.print("Hosting SSID ");
  Serial.print(WIFI_SSID);
  
  // Start broadcasting SSID with Passphrase
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

  // Print Node MCU IP address over the serial port
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Define '/' route for HTTP Get Requests.
  // Send HTML string processed with the processState() preprocessor function
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", html, processState);
  });
  
  // Enable web server
  server.begin();

}

void loop()
{
  // Poll data from sensors and assign it to the variables
  sensor1state = !digitalRead(SENSOR1_PIN);
  sensor2state = !digitalRead(SENSOR2_PIN);

  // Determine valve state and set relay accordingly
  if(!(sensor1state || sensor2state))
  {
    digitalWrite(RELAY_PIN, HIGH);
  } 
  else
  {
    digitalWrite(RELAY_PIN, LOW);
  }
}
