#include <pgmspace.h>

const char WIFI_SSID[] = ""; //fill with your wifi ssid
const char WIFI_PASSWORD[] = ""; //fill with your wifi passowrd

#define THINGNAME "" //fill with thing name

int8_t TIME_ZONE = 0;

const char* MQTT_HOST = ""; // fill with your MQTT HOST URL (Connect > Domain Configurations > Domain Name)

// root_ca.pem here VVV
static const char root_ca[] PROGMEM = R"EOF(
  -----BEGIN CERTIFICATE-----

  -----END CERTIFICATE-----
)EOF";

// certificate.pem here VVV
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
 
-----END CERTIFICATE-----
 
)KEY";
 
 
// private_key.pem here VVV
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
 
-----END RSA PRIVATE KEY-----
 
)KEY";
