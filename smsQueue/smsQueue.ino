#include <cppQueue.h>
#define	IMPLEMENTATION	LIFO


typedef struct smsData {
		uint16_t	templateId;
} SMS;

SMS tab[3] = {
	{ 0 },
	{ 0},
	{ 2 }
};

String  templates[3] = {
	{ "turned on!"},
	{ "turnd off!"},
	{ "overheated!"}
};

cppQueue	q(sizeof(SMS), 10, IMPLEMENTATION);	// Instantiate queue

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
	unsigned int i;
	
	for (i = 0 ; i < 3 ; i++)
	{
		SMS rec = tab[i];
		q.push(&rec);
		// q.push(&rec);
		// q.push(&rec);
		// q.push(&rec);
		// q.push(&rec);
	}
	Serial.println("start");
	int count =q.getCount();
  Serial.println(count);
	for (i = 0 ; i < count ; i++)
	{
		SMS rec;
		q.peek(&rec);
	  Serial.print("cnt: ");
		Serial.println(templates[rec.templateId]);
    q.drop();
	}

  int countt =q.getCount();
  Serial.println(countt);

	Serial.println("tamam");
	while(1);
}
