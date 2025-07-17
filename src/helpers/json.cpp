#include "json.h"

String stringify_wifi(wifi_status_t wifi){
    String wifi_status = "disconnected";;
    if (wifi == WIFI_CONNECTED)
        wifi_status = "connected";
    else if (wifi == WIFI_CONNECTING)
        wifi_status = "connecting";
        
    String json = "{";
    json += "\"wifi\":\"" + wifi_status + "\"";
    json += "}";

    return json.c_str();
}
