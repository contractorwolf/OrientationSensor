#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Set the delay between fresh samples EXPECT HIGH REFRESH*/
#define BNO055_SAMPLERATE_DELAY_MS (5)

float horizontal_angle = 0;
float horizontal_offset = 0;

float verticle_angle = 0;
float verticle_offset = 0;

float currentHorizontalAngle = 0;
float currentVerticleAngle = 0;

float lastHorizontalAngle = 0;
float lastVerticleAngle = 0;

const int buttonGndPin = 6;
const int buttonPin = 7;

int buttonState = 0;
int prevButtonState = 0;

int incoming = 0;

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");  
  Serial.println("------------------------------------");

  Serial.println("");

  Serial.println("waiting 2 seconds");
  
  delay(2000);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void) 
{
  Serial.begin(9600);
  Serial.println("\r\nORIENTATION SENSOR STARTED");
  delay(1000);
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);
    
  /* Display some basic information on this sensor */
  displaySensorDetails();

  bno.setExtCrystalUse(true);

  pinMode(buttonGndPin, OUTPUT); 
  digitalWrite(buttonGndPin, LOW); 
      
  pinMode(buttonPin, INPUT);  
  
  delay(3000);
  
  Mouse.begin();
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);

  //CONVERT OUTPUT FROM Adafruit_BNO055
  //**************************************************
  if(event.orientation.x > 180){
    horizontal_angle = (event.orientation.x -360) + horizontal_offset;
  }else{
    horizontal_angle = event.orientation.x + horizontal_offset;
  }
 
  verticle_angle = event.orientation.y + verticle_offset;
  //CONVERT OUTPUT FROM Adafruit_BNO055

  //convert to mouse moves, by subtracting where you were last
  currentHorizontalAngle = (horizontal_angle - lastHorizontalAngle)*50;
  currentVerticleAngle = (verticle_angle - lastVerticleAngle)*40;

  //get current button state
  buttonState = digitalRead(buttonPin);

  //DISPLAY DATA FROM Adafruit_BNO055
  //*************************************************************
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);     
  
  Serial.print("  horizontal offest: ");
  Serial.print(horizontal_offset, 4);   
  
  Serial.print("  HOR ANGLE: ");
  Serial.print(horizontal_angle, 4);   

  Serial.print("  Y: ");
  Serial.print(event.orientation.y, 4);
  
  Serial.print("  verticle offset: ");
  Serial.print(verticle_offset, 4);   

  Serial.print("  VERT ANGLE: ");
  Serial.print(verticle_angle, 4);   
  //Serial.print("  Z: ");
  //Serial.print(event.orientation.z, 4);

  Serial.print("  SW: ");
  Serial.print(buttonState);

  Serial.print("  T: ");
  Serial.print(millis()); 
  
  Serial.print("  moveH: ");
  Serial.print((int)currentHorizontalAngle); 
  
  Serial.print("  moveV: ");
  Serial.print((int)currentVerticleAngle); 

  Serial.println("");

  //ACT ON DATA
  //************************************************************
  //if button is on then move mouse
  if(buttonState==1){
      Mouse.move((int)currentHorizontalAngle, (int)currentVerticleAngle, 0);
   
      lastHorizontalAngle = horizontal_angle;
      lastVerticleAngle = verticle_angle;
  }
 
  //check that the button was off but is now on (change of state)
  if(buttonState==1&&prevButtonState==0){
    Serial.println("RESET CENTER");
    if(event.orientation.x > 180){
      horizontal_offset = -(event.orientation.x -360);
    }else{
      horizontal_offset = -event.orientation.x;
    }
  
    verticle_offset = -(event.orientation.y);
  }

  //save the current button state for the next loop
  prevButtonState = buttonState;
  
  //************************************************************
  
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
