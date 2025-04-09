#include <pgmspace.h>

#define SECRET

const char WIFI_SSID[] = "";
const char WIFI_PASSWORD[] = "";

#define THINGNAME ""

int8_t TIME_ZONE = 1 //UK: +1 UTC

const char MQTT_HOST = "";

// root_ca.pem here VVV
static const char root_ca[] PROGMEM = R"EOF(
  -----BEGIN CERTIFICATE-----

  -----END CERTIFICATE-----
)EOF";

// certificate.pem.crt here VVV
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
 
-----END CERTIFICATE-----
 
)KEY";
 
 
// private_key.pem here VVV
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
 
-----END RSA PRIVATE KEY-----
 
)KEY";