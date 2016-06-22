#include <Process.h>
#include <ChainableLED.h>
#include <DHT.h>


Process p;

#define LIGHT_PIN A1
#define SOUND_PIN A2

#define NUM_LEDS  1
ChainableLED leds(4, 5, NUM_LEDS);

#define DHT_PIN A0 
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);


char cmd_buf[256] = {0, };


int temperature = 0;
int humidity = 0;
int light = 0;
int sound = 0;
long rgbled = 0;
int i;


void setup()
{
    Serial.begin(115200);
    Serial.println("Seeeduino Cloud Azure IoT Remote Monitoring.");
    
    leds.init();
    dht.begin();
    delay(1000);
    
    Bridge.begin();
    delay(1000);
}

void loop()
{
    int i;
    char *ptr;
    long rgbtemp;
    char message[128] = {0, };
    
    light = analogRead(LIGHT_PIN);
    sound = analogRead(SOUND_PIN);
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    
    snprintf(message, sizeof(message), "\"Temperature\":%d,\"Humidity\":%d,\"Light\":%d,\"Sound\":%d", temperature, humidity, light, sound);
    Serial.println(message);
    azureMessageSend(message);
    
    memset(message, 0, sizeof(message));
    azureMessageReceive(message);
    Serial.println(message);
    
    ptr = strstr(message, "\"SetRGBLed\":");
    if(ptr)
    {
        rgbtemp = atof((const char*)&message[12]);
        
        if(rgbtemp != rgbled)
        {
            rgbled = rgbtemp;
            leds.setColorRGB(0, (rgbled >> 16) & 0xff, (rgbled >> 8) & 0xff, rgbled & 0xff);  
        }
    }
    
    delay(1000);
}

void azureMessageSend(char *buffer)
{
    memset(cmd_buf, 0, sizeof(cmd_buf));
    snprintf(cmd_buf, sizeof(cmd_buf), "echo -n '%s' > /root/AzureMessageSend", buffer);
    
    p.runShellCommand(cmd_buf);
    while(p.running());

    p.runShellCommand("sed -i '$a >CLIENT_SEND' /root/AzureMessageSend");
    while(p.running());
}

void azureMessageReceive(char *buffer)
{
    int i = 0;
    
    p.runShellCommand("cat /root/AzureMessageReceive");
    while(p.running());
    while(p.available())
    {
        buffer[i ++] = (char)p.read();
    }
}
