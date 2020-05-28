/*
    Task 2. Subscriber and server client.
*/

#include <ros.h>
#include <std_msgs/Bool.h>
#include <thruster_controller/ControlThruster.h>
std_msgs::Bool prev_msg;
using thruster_controller::ControlThruster;

ros::NodeHandle nh;
/**
    Subscribe to thruster_state and call service to change the state
*/
class ThrusterSwitch
{
  public:
    ThrusterSwitch()
      : sub_("thruster_state", &ThrusterSwitch::messageCb, this),
        client_("control_thruster")
    {}

    void init(ros::NodeHandle& nh)
    {
      nh.subscribe(sub_);
      nh.serviceClient(client_);
      first_time_con = true;
    }
    /**
      Since setup finished, turn ON the thruster at 0s, turn OFF at 3s and turn ON at 6s.
      @param nh node handler
    */
    void run(ros::NodeHandle& nh)
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
          client_.call(req, res);
          if (res.success) nh.loginfo("Requested to turn the thruster ON.");
        }
        else if (current_time == 3000 )
        {
          req.state = false;
          client_.call(req, res);
          if (res.success) nh.loginfo("Requested to turn the thruster OFF.");
        }
        else if (current_time == 6000)
        {
          req.state = true;
          client_.call(req, res);
          if (res.success) nh.loginfo("Requested to turn the thruster ON.");
        }

      }
    }
    /**
       Callback function of the subscriber to thruster_state.
       @param msg  message reference
    */
    void messageCb( const std_msgs::Bool& msg)
    {
      if (msg.data != prev_msg.data)
      {
        if (msg.data) nh.loginfo("Thruster is ON");
        else nh.loginfo("Thruster is OFF");
        prev_msg = msg;
      }

    }
  private:
    /**
      Subscriber to thruster_state.
    */
    ros::Subscriber<std_msgs::Bool, ThrusterSwitch> sub_;
    /**
      Server client of the service ControlThruster
    */
    ros::ServiceClient<ControlThruster::Request, ControlThruster::Response> client_;
    /**
      Start time when setup of the node is done
    */
    unsigned long start_time;
    /**
      Time since setup of the node is done
    */
    unsigned long current_time;
    /**
      Whether the the node is set up for the first time
    */
    bool first_time_con;

};

ThrusterSwitch thruster_switch;

void setup()
{
  nh.initNode();
  thruster_switch.init(nh);
}

void loop()
{
  thruster_switch.run(nh);
  nh.spinOnce();
  delay(1);


}
