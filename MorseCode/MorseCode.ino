
#define LED 2
#define PAUSE_ON 1000
#define PAUSE_OFF 1000
#define LONG 400
#define SHORT 150

#define DEBUG(x) Serial.print(x)

const char letters[] = "abcdefghijklmnopqrstuvwxyz";
const char morse[] = ".- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --..";
String code[27] = { "" };

void FillTheCode()
{
	String tmp(morse);
	for (size_t i = 0; i < sizeof(letters)/sizeof(char); i++)
	{
		auto p = tmp.indexOf(' ');
		if (p == -1) break;
		code[i] = tmp.substring(0, p);
		tmp = tmp.substring(p + 1);

		Serial.print(letters[i]);
		Serial.print(" => ");
		Serial.println(code[i]);
	}
}

String CodeForLetter(char c)
{
	String letter(c);
	letter.toLowerCase();
	int p = letter[0] - 97;

	if (p < 0 || p > 27)
		return "";
	
	return code[p];
}

void Blink(String letter)
{
	for (size_t i = 0; i < letter.length(); i++)
	{
		digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
		delay(letter[i] == '.' ? SHORT : LONG);
		digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
		delay(LONG);              // wait for a second
	}
}

void MorseBlink(String message)
{
	for (size_t i = 0; i < message.length(); i++)
	{
		Serial.print(message[i]);
		auto letter = message[i];
		auto code = CodeForLetter(letter);
		Blink(code);
		delay(SHORT*2);
	}
	Serial.println();
}

// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin 13 as an output.
	Serial.begin(115200);
	pinMode(LED, OUTPUT);

	FillTheCode();

}

void Reset()
{
	for (size_t i = 0; i < 1; i++)
	{
		digitalWrite(LED, HIGH);  
		delay(PAUSE_ON);          
		digitalWrite(LED, LOW);   
		delay(PAUSE_OFF);
	}      
	Serial.println("==> RESET <==");
}

// the loop function runs over and over again forever
void loop() {
	auto message = "SOS";
	MorseBlink("SOS");

	Reset();
}
