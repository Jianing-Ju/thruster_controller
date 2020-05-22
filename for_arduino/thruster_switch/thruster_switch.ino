/*
   rosserial Subscriber Example
   Blinks an LED on callback
*/

#include <ros.h>
#include <std_msgs/Bool.h>
#include <thruster_controller/ControlThruster.h>
std_msgs::Bool prev_msg;

unsigned long start_time;
unsigned long current_time;
bool first_time_con;
ros::NodeHandle  nh;
using thruster_controller::ControlThruster;

void messageCb( const std_msgs::Bool& msg)
{
  if (msg.data != prev_msg.data)
  {
    if (msg.data) nh.loginfo("Thruster is ON");
    else nh.loginfo("Thruster is OFF");
    prev_msg = msg;
  }

}

ros::Subscriber<std_msgs::Bool> sub("thruster_state", &messageCb);
ros::ServiceClient<ControlThruster::Request, ControlThruster::Response> client("control_thruster");


void setup()
{
  nh.initNode();
  nh.subscribe(sub);
  nh.serviceClient(client);

  first_time_con = true;
}

void loop()
{

  if (nh.connected())
  {
    if (first_time_con)
    {
      start_time = millis();
      first_time_con = false;
    }

    ControlThruster::Request req;
    ControlThruster::Response res;
    current_time = millis() - start_time;
    if (current_time == 0)
    {
      req.state = true;
      client.call(req, res);
      if (res.success) nh.loginfo("Requested to turn the thruster ON.");
    }
    else if (current_time == 3000 )
    {
      req.state = false;
      client.call(req, res);
      if (res.success) nh.loginfo("Requested to turn the thruster OFF.");
    }
    else if (current_time == 6000)
    {
      req.state = true;
      client.call(req, res);
      if (res.success) nh.loginfo("Requested to turn the thruster ON.");
    }

  }


  nh.spinOnce();
  delay(1);


}
