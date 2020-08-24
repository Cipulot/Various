#include <EepromAT24C32.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

//Display handle
#define OLED_RESET D5
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//Clock stuff
char *serial;
int on_time = 0;
char CURRENTTIME[9];
char *days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int startDay = 0; // Sunday's value is 0, Saturday is 6
String week1 = "";
String week2 = "";
String week3 = "";
String week4 = "";
String week5 = "";
int newWeekStart = 0; // used to show start of next week of the month
char monthString2[37] = {"JanFebMarAprMayJunJulAugSepOctNovDec"};
int monthIndex2[122] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33};
char monthName2[3] = "";
int monthLength = 0;

//Wifi handle
#ifndef STASSID
#define STASSID "Hawking-Network"
#define STAPSK "SheldonOps691"
#endif
const char *ssid = STASSID;
const char *password = STAPSK;
IPAddress wifiip;
bool connected = false;
int strength = 0;

//Various stuff
int i;

void setup()
{
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	pinMode(D6, INPUT);
	pinMode(D7, INPUT);
	pinMode(D8, INPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200);
	Serial.println("Booting");
	serial = ("Booting");
	writeOLED(0);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	//try to connect to wifi for 10 seconds
	i = 10;
	while (WiFi.status() != WL_CONNECTED && i >= 0)
	{
		if (i < 5)
			writeOLED(6);
		delay(1000);
		i--;
	}
	if (WiFi.status() == WL_CONNECTED)
	{
		connected = true;
		Serial.println("Ready");
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
		writeOLED(2);
	}
	else
	{
		connected = false;
		writeOLED(6);
	}
	configTime(1 * 3600, 3600, "pool.ntp.org", "time.nist.gov");
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
	digitalWrite(LED_BUILTIN, HIGH);
	display.ssd1306_command(SSD1306_DISPLAYON);
}

void loop()
{
	//WIFI control
	if (digitalRead(D6) == HIGH)
	{
		display.ssd1306_command(SSD1306_DISPLAYON);
		if (WiFi.status() == WL_CONNECTED)
		{ //connected...so we need to disconnect wifi
			WiFi.disconnect();
			connected = false;
			writeOLED(4);
			delay(1500);
		}
		else
		{ //not connected...so we need to connect wifi
			WiFi.begin(ssid, password);
			writeOLED(8);
			//try to connect for 5 sec
			i = 5;
			while (WiFi.status() != WL_CONNECTED && i >= 0)
			{
				if (i < 3)
					writeOLED(7);
				delay(1000);
				i--;
			}
			if (WiFi.status() == WL_CONNECTED)
			{
				connected = true;
				writeOLED(5);
			}
			else
			{
				connected = false;
				writeOLED(6);
			}
			delay(1500);
		}
	}
	//Display control button
	if (digitalRead(D8) == HIGH)
	{
		on_time = 0;
		display.ssd1306_command(SSD1306_DISPLAYON);
	}
	if (on_time < 5)
	{
		on_time++;
		writeOLED(3);
	}
	else
	{
		display.ssd1306_command(SSD1306_DISPLAYOFF);
	}
	//Show calendar
	if (digitalRead(D7) == HIGH)
	{
		display.ssd1306_command(SSD1306_DISPLAYON);
		//writeOLED(2);
		drawCalendar();
		delay(1500);
	}
	delay(1000);
}

void writeOLED(int var)
{
	display.clearDisplay(); /* Clear display */
	if (var == 0)
	{
		drawStr(40, 30, serial);
	}
	else if (var == 1)
	{
		serial = ("Connection Failed!");
		drawStr(5, 30, serial);
		serial = ("Rebooting...");
		drawStr(5, 40, serial);
	}
	else if (var == 2)
	{
		drawStr(50, 10, "Ready");
		drawStr(30, 30, "IP address:");
		wifiip = WiFi.localIP();
		String ipStr = String(wifiip[0]) + "." + String(wifiip[1]) + "." + String(wifiip[2]) + "." + String(wifiip[3]);
		ipStr.toCharArray(serial, 15);
		drawStr(25, 40, serial);
	}
	else if (var == 3)
	{
		time_t now = time(nullptr);
		struct tm *timeinfo;
		timeinfo = localtime(&now);
		display.setTextSize(2);
		snprintf(CURRENTTIME, sizeof(CURRENTTIME), "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		drawStr(15, 25, CURRENTTIME);
		display.setTextSize(1.5);
		snprintf(CURRENTTIME, sizeof(CURRENTTIME), "%02d/%02d/%02d", timeinfo->tm_mday, (timeinfo->tm_mon) + 1, (timeinfo->tm_year) - 100);
		drawStr(42, 55, CURRENTTIME);
		snprintf(CURRENTTIME, sizeof(CURRENTTIME), "%s", days[(timeinfo->tm_wday)]);
		drawStr(0, 0, CURRENTTIME);
		snprintf(CURRENTTIME, sizeof(CURRENTTIME), "%d%c", (85400 / analogRead(A0)), 37);
		drawStr(110, 0, CURRENTTIME);
		if (connected == true)
		{ //here we have to draw the cell infos
			for (int b = 0; b <= getBarsSignal(WiFi.RSSI()); b++)
			{
				display.fillRect(b * 5, 64 - (b * (5 - 3)), 3, b * 5, WHITE);
			}
		}
	}
	else if (var == 4)
	{
		drawStr(10, 30, "WiFi disconnected!");
	}
	else if (var == 5)
	{
		drawStr(15, 30, "WiFi connected!");
	}
	else if (var == 6)
	{
		drawStr(15, 30, "WiFi not found!");
	}
	else if (var == 7)
	{
		drawStr(15, 30, "still waiting...");
	}
	else if (var == 8)
	{
		drawStr(15, 30, "Connecting...");
	}
	display.display();
}

void drawStr(uint8_t x, uint8_t y, char *str)
{
	display.setCursor(x, y); /* Set x,y coordinates */
	display.println(str);
}

// calculate first day of month
int startDayOfWeek(int y, int m, int d)
{
	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	y -= m < 3;
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

void drawCalendar()
{
	display.clearDisplay();
	display.setTextSize(1);
	// display a full month on a calendar
	drawStr(2, 5, "Su Mo Tu We Th Fr Sa");
	// display this month
	time_t nowcalendar = time(nullptr);
	struct tm *timeinfocalendar;
	timeinfocalendar = localtime(&nowcalendar);
	//
	// get number of days in month
	if (timeinfocalendar->tm_mon == 0 || timeinfocalendar->tm_mon == 2 || timeinfocalendar->tm_mon == 4 || timeinfocalendar->tm_mon == 6 || timeinfocalendar->tm_mon == 7 || timeinfocalendar->tm_mon == 9 || timeinfocalendar->tm_mon == 11)
	{
		monthLength = 31;
	}
	else
	{
		monthLength = 30;
	}
	if (timeinfocalendar->tm_mon == 1)
	{
		monthLength = 28;
	}
	startDay = startDayOfWeek((timeinfocalendar->tm_year) - 100, (timeinfocalendar->tm_mon) + 1, 1); // Sunday's value is 0
	// now build first week string
	switch (startDay)
	{
	case 0:
		// Sunday
		week1 = " 1  2  3  4  5  6  7";
		break;
	case 1:
		// Monday
		week1 = "    1  2  3  4  5  6";
		break;
	case 2:
		// Tuesday
		week1 = "       1  2  3  4  5";
		break;
	case 3:
		// Wednesday
		week1 = "          1  2  3  4";
		break;
	case 4:
		// Thursday
		week1 = "             1  2  3";
		break;
	case 5:
		// Friday
		if (monthLength == 28 || monthLength == 30)
		{
			week1 = "                1  2";
		}
		if (monthLength == 31)
		{
			week1 = "31              1  2";
		}
		break;
	case 6:
		// Saturday
		if (monthLength == 28)
		{
			week1 = "                   1";
		}
		if (monthLength == 30)
		{
			week1 = "30                 1";
		}
		if (monthLength == 31)
		{
			week1 = "30 31              1";
		}

		break;
	} // end first week
	newWeekStart = (7 - startDay) + 1;
	char *newWeek1 = (char *)week1.c_str();
	drawStr(2, 15, newWeek1);
	// display week 2
	week2 = "";
	for (int f = newWeekStart; f < newWeekStart + 7; f++)
	{
		if (f < 10)
		{
			week2 = week2 + " " + String(f) + " ";
		}
		else
		{
			week2 = week2 + String(f) + " ";
		}
	}
	char *newWeek2 = (char *)week2.c_str();
	drawStr(2, 25, newWeek2);
	// display week 3
	newWeekStart = (14 - startDay) + 1;
	week3 = "";
	for (int f = newWeekStart; f < newWeekStart + 7; f++)
	{
		if (f < 10)
		{
			week3 = week3 + " " + String(f) + " ";
		}
		else
		{
			week3 = week3 + String(f) + " ";
		}
	}
	char *newWeek3 = (char *)week3.c_str();
	drawStr(2, 35, newWeek3);
	// display week 4
	newWeekStart = (21 - startDay) + 1;
	week4 = "";
	for (int f = newWeekStart; f < newWeekStart + 7; f++)
	{
		if (f < 10)
		{
			week4 = week4 + " " + String(f) + " ";
		}
		else
		{
			week4 = week4 + String(f) + " ";
		}
	}
	char *newWeek4 = (char *)week4.c_str();
	drawStr(2, 45, newWeek4);
	// do we need a fifth week
	week5 = "";
	newWeekStart = (28 - startDay) + 1;
	// is is February?
	if (newWeekStart > 28 && ((timeinfocalendar->tm_mon) + 1) == 2)
	{
		// do nothing unless its a leap year
		if (((timeinfocalendar->tm_year) - 100) == (((timeinfocalendar->tm_year) - 100) / 4) * 4)
		{ // its a leap year
			week5 = "29";
		}
	}
	else
	{ // print up to 30 anyway
		if (((timeinfocalendar->tm_mon) + 1) == 2)
		{ // its February
			for (int f = newWeekStart; f < 29; f++)
			{
				week5 = week5 + String(f) + " ";
			}
			// is it a leap year
			if (((timeinfocalendar->tm_year) - 100) == (((timeinfocalendar->tm_year) - 100) / 4) * 4)
			{ // its a leap year
				week5 = week5 + "29";
			}
		}
		else
		{
			for (int f = newWeekStart; f < 31; f++)
			{
				week5 = week5 + String(f) + " ";
			}
			// are there 31 days
			if (monthLength == 31 && week5.length() < 7)
			{
				week5 = week5 + "31";
			}
		}
	}
	char *newWeek5 = (char *)week5.c_str();
	drawStr(2, 55, newWeek5);
	display.display();
}

int getBarsSignal(long rssi)
{
	// 5. High quality: 90% ~= -55db
	// 4. Good quality: 75% ~= -65db
	// 3. Medium quality: 50% ~= -75db
	// 2. Low quality: 30% ~= -85db
	// 1. Unusable quality: 8% ~= -96db
	// 0. No signal
	int bars;

	if (rssi > -55)
	{
		bars = 5;
	}
	else if (rssi<-55 & rssi> - 65)
	{
		bars = 4;
	}
	else if (rssi<-65 & rssi> - 75)
	{
		bars = 3;
	}
	else if (rssi<-75 & rssi> - 85)
	{
		bars = 2;
	}
	else if (rssi<-85 & rssi> - 96)
	{
		bars = 1;
	}
	else
	{
		bars = 0;
	}
	return bars;
}
