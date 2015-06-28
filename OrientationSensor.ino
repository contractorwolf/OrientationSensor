#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.
   
   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).
   
   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3-5V DC
   Connect GROUND to common ground
    
   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples EXPECT HIGH REFRESH*/
#define BNO055_SAMPLERATE_DELAY_MS (10)


float horizontal_angle = 0;
float horizontal_offset = 0;


float verticle_angle = 0;
float verticle_offset = 0;



int incoming = 0;

   
Adafruit_BNO055 bno = Adafruit_BNO055(55);

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/

/**************************************************************************/
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


  sensors_event_t event; 
  bno.getEvent(&event);





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

  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    incoming = Serial.parseInt();

  }

  //CONVERT OUTPUT FROM Adafruit_BNO055
  //**************************************************
  if(event.orientation.x > 180){
    horizontal_angle = (event.orientation.x -360) + horizontal_offset;
  }else{
    horizontal_angle = event.orientation.x + horizontal_offset;
  }
  
  verticle_angle = event.orientation.y + verticle_offset;
  //


  //DISPLAY DATA FROM Adafruit_BNO055
  //*************************************************************
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);     
  
  Serial.print("\thorizontal angle: ");
  Serial.print(horizontal_angle, 4);   

    Serial.print("\thorizontal offest: ");
  Serial.print(horizontal_offset, 4);   
  
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);

  Serial.print("\tverticle angle: ");
  Serial.print(verticle_angle, 4);   

  Serial.print("\tverticle offest: ");
  Serial.print(verticle_offset, 4);   
  

  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);

  Serial.print("\tmillis:: ");
  Serial.print(millis()); 
  
  Serial.println("");

  //ACT ON DATA
  //************************************************************

  if(incoming==1){
    Mouse.move((int)horizontal_angle, (int)verticle_angle, 0);
  }

  if(incoming==2){
    horizontal_offset = -horizontal_angle;
    verticle_offset = -verticle_angle;
    incoming=0;
  }


  
  //************************************************************
  
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
