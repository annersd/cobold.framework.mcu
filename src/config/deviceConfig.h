#pragma once
#include "Arduino.h"

char deviceConfig[] PROGMEM = R"(
  {
    "cobold":
    {
      "node":
      {
        "name": "nid-fw-13501"
      },
      "mqtt":
      {
        "host": "192.168.0.69",
        "port": 1883,
        "clientid": "nid-fw-13501"
      },
      "network": 
      { 
        "wifi":
        { 
            "ssid": "enter your ssid here", 
            "password": "enter your password here" 
        } 
      } 
    }
  })";
