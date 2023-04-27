#!/usr/bin/env python3

import rospy
import serial
from std_msgs.msg import String

def serial_to_ros():

    # Initialize the node
    rospy.init_node('serial_to_ros')

    # Get the USB port from ROS parameter server
    port = rospy.get_param('~port', '/dev/ttyUSB0')

    # Set up the serial connection
    ser = serial.Serial(port, 9600, timeout=1)

    # Set up the ROS publisher
    pub = rospy.Publisher('serial_data', String, queue_size=10)

    # Set the loop rate in Hz
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():

        # Read the serial data
        data = ser.readline().strip().decode('ascii')
        print(data)

        # Publish the data onto the ROS topic
        pub.publish(data)

        # Sleep for the remainder of the loop cycle
        rate.sleep()

if _name_ == '_main_':
    try:
        serial_to_ros()
    except rospy.ROSInterruptException:
        pass
