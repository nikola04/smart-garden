#include "network.h"
#include "HTTPClient.h"
#include "config.h"
#include "storage.h"
#include "json.h"

int sendData(SensorsData sdata, power_data_t pdata){
    if (WiFi.status() != WL_CONNECTED) return -2;

    String api_key = getAPIKey();

    HTTPClient http;
    http.setTimeout(5000);
    String payload = stringifyAPIData(sdata, pdata);

    try{
        http.begin(DEFAULT_API_URL);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-api-key", api_key);
        int responseCode = http.POST(payload);
        if (responseCode < 200 || responseCode >= 300)
            return -1;
    }catch(...){
        return -2;
    }

    http.end();
    return 0;
}
