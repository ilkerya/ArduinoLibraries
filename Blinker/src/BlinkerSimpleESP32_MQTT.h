#ifndef BlinkerSimplerESP32_MQTT_H
#define BlinkerSimplerESP32_MQTT_H

#include "Adapters/BlinkerMQTT.h"

#if defined(BLINKER_APCONFIG)
static WiFiServer *_server;
static WiFiClient _client;
static IPAddress apIP(192, 168, 4, 1);
#endif

class BlinkerSimpleESP32_MQTT 
    : public BlinkerProtocol<BlinkerMQTT>
{
    typedef BlinkerProtocol<BlinkerMQTT> Base;

    public : 
        BlinkerSimpleESP32_MQTT(BlinkerMQTT &transp)
            : Base(transp)
        {}

#if defined(BLINKER_ESP_SMARTCONFIG)
        void begin(const char* _auth) {
            Base::begin(_auth);
            smartconfig();
            this->conn.begin(_auth);
            strcpy(Base::_deviceName, this->conn.deviceName().c_str());
            Base::loadTimer();
            BLINKER_LOG1("ESP32_MQTT initialized...");
        }
// #endif
#elif defined(BLINKER_APCONFIG)
        void begin() {
            Base::begin();
            softAPinit();
            while(WiFi.status() != WL_CONNECTED) {
                serverClient();

                ::delay(10);
            }
            Base::loadTimer();
            BLINKER_LOG1("ESP8266_WiFi initialized...");
        }
#else
        void begin( const char* _auth,
                    const char* _ssid,
                    const char* _pswd)
        {
            Base::begin(_auth);
            connectWiFi(_ssid, _pswd);
            this->conn.begin(_auth);
            strcpy(Base::_deviceName, this->conn.deviceName().c_str());
            Base::loadTimer();
            BLINKER_LOG1("ESP32_MQTT initialized...");
        }
#endif

    private :
#if defined(BLINKER_APCONFIG)
        void softAPinit()
        {
            _server = new WiFiServer(80);
            
            WiFi.mode(WIFI_AP);
            String softAP_ssid = "DiyArduino_" + macDeviceName();
            WiFi.setHostname(softAP_ssid.c_str());
            WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
            WiFi.softAP(softAP_ssid.c_str(), "12345678");
            delay(100);

            _server->begin();
            BLINKER_LOG2(F("AP IP address: "), WiFi.softAPIP());
            BLINKER_LOG1("HTTP _server started");
            BLINKER_LOG1(String("URL: http://" + WiFi.softAPIP()));
        }

        void serverClient()
        {
            if (!_client)
            {
                _client = _server->available();
            }
            else
            {
                if (!_client.connected())
                {
                    _client.stop();
                    BLINKER_LOG1(F("Connection closed on _client"));
                }
                else
                {
                    if (_client.available())
                    {
                        String data = _client.readStringUntil('\r');

                        // data = data.substring(4, data.length() - 9);
                        _client.flush();

                        // BLINKER_LOG2("clientData: ", data);

                        if (STRING_contains_string(data, "ssid") && STRING_contains_string(data, "pswd")) {

                            String msg = "{\"hello\":\"world\"}";
                            
                            String s= "HTTP/1.1 200 OK\r\nContent-Type: application/json;charset=utf-8\r\n";
                            s += String("Content-Length: " + String(msg.length()) + "\r\n" +  
                                "Connection: Keep Alive\r\n\r\n" +  
                                msg + "\r\n");

                            _client.print(s);
                            
                            _client.stop();

                            parseUrl(data);
                        }
                    }
                }
            }
        }

        bool parseUrl(String data)
        {
            BLINKER_LOG2("APCONFIG data: ", data);
            DynamicJsonBuffer jsonBuffer;
            JsonObject& wifi_data = jsonBuffer.parseObject(data);

            if (!wifi_data.success()) {
                return false;
            }
                            
            String _ssid = wifi_data["ssid"];
            String _pswd = wifi_data["pswd"];

            BLINKER_LOG2("ssid: ", _ssid);
            BLINKER_LOG2("pswd: ", _pswd);

            free(_server);
            connectWiFi(_ssid, _pswd);
            return true;
        }
#endif

#if defined(BLINKER_ESP_SMARTCONFIG)
        void smartconfig() {
            WiFi.mode(WIFI_AP_STA);
            String _hostname = "DiyArduinoMQTT_" + macDeviceName();
            WiFi.setHostname(_hostname.c_str());
            WiFi.beginSmartConfig();
            
            BLINKER_LOG1("Waiting for SmartConfig.");
            while (!WiFi.smartConfigDone()) {
                ::delay(500);
            }

            BLINKER_LOG1("SmartConfig received.");
            
            BLINKER_LOG1("Waiting for WiFi");
            while (WiFi.status() != WL_CONNECTED) {
                ::delay(500);
            }

            BLINKER_LOG1("WiFi Connected.");

            BLINKER_LOG1("IP Address: ");
            BLINKER_LOG1(WiFi.localIP());

            // mDNSInit();
        }
#endif

        void connectWiFi(String _ssid, String _pswd)
        {
            connectWiFi(_ssid.c_str(), _pswd.c_str());
        }

        void connectWiFi(const char* _ssid, const char* _pswd)
        {
            uint32_t connectTime = millis();

            BLINKER_LOG2(("Connecting to "), _ssid);
            
            WiFi.mode(WIFI_STA);
            String _hostname = "DiyArduinoMQTT_" + macDeviceName();
            WiFi.setHostname(_hostname.c_str());

            if (_pswd && strlen(_pswd)) {
                WiFi.begin(_ssid, _pswd);
            }
            else {
                WiFi.begin(_ssid);
            }

            while (WiFi.status() != WL_CONNECTED) {
                ::delay(50);

                if (millis() - connectTime > BLINKER_CONNECT_TIMEOUT_MS && WiFi.status() != WL_CONNECTED) {
                    connectTime = millis();
                    BLINKER_LOG1(("WiFi connect timeout, please check ssid and pswd!"));
                    BLINKER_LOG1(("Retring WiFi connect again!"));
                }
            }
            BLINKER_LOG1(("Connected"));

            IPAddress myip = WiFi.localIP();
            BLINKER_LOG2(("Your IP is: "), myip);

            // mDNSInit();
        }
};

static BlinkerMQTT  _blinkerTransport;
BlinkerSimpleESP32_MQTT Blinker(_blinkerTransport);

#include <BlinkerWidgets.h>

#endif