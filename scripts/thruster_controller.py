#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Example Python node to publish on a specific topic."""

# Import required Python code.
import rospy
from std_msgs.msg import Bool
from thruster_controller.srv import ControlThruster


class ThrusterController(object):
    """Thruster Controller class."""

    def __init__(self):
        self.state = False
        # self.rate = 10
        self.pub = rospy.Publisher('thruster_state', Bool, queue_size=10)
        rospy.Service('control_thruster', ControlThruster, self.handle_control)
        print "Server is ready."

        rate = rospy.Rate(10) # 10hz
        while not rospy.is_shutdown():
            self.pub.publish(self.state)
            rate.sleep()
        

    def handle_control(self, req):
        self.state = req.state
        return True


# Main function.
if __name__ == "__main__":
    # Initialize the node and name it.
    rospy.init_node('thruster_cmd')
    # Go to class functions that do all the heavy lifting.

    try:
        ThrusterController()
    except rospy.ROSInterruptException:
        pass
    # Allow ROS to go to all callbacks.
    rospy.spin()

