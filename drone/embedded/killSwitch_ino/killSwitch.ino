#include <ros.h>
#include <ArduinoHardware.h>
#include <std_msgs/String.h>
#include <stdlib.h>

int alarm = 2;
int ks1  = 4;
int ks2  = 5;

ros::NodeHandle  nh;

void messageCb(const std_msgs::String& toggle_msg);

void messageCb(const std_msgs::String& toggle_msg)
{

  // Motor move forward
  if(toggle_msg.data == "1")
  {
    digitalWrite(ks1,HIGH);
    digitalWrite(ks2,HIGH);
  }
  else if (toggle_msg.data == "2")
  {
    digitalWrite(alarm,HIGH);
  }

}

ros::Subscriber<std_msgs::String> sub_drone_killswitch("/pkm/drone/killswitch", &messageCb );

void setup()
{
  pinMode(alarm, OUTPUT);
  pinMode(ks1, OUTPUT);
  pinMode(ks2, OUTPUT);

  digitalWrite(alarm, LOW);
  digitalWrite(ks1,LOW);
  digitalWrite(ks2,LOW);
  
  nh.initNode();
  nh.subscribe(sub_drone_killswitch);
}

void loop()
{
  nh.spinOnce();
  delay(500);
}
