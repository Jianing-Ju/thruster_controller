FROM ros:melodic-robot

RUN apt-get update && apt-get install -y \
    build-essential \
    && apt-get install arduino-core avr-libc avrdude binutils-avr \
    gcc-avr libftdi1 libusb-0.1-4 python-serial ros-melodic-rosserial-arduino \
    ros-melodic-rosserial-client ros-melodic-rosserial-msgs ros-melodic-rosserial-python\
    ros-melodic-rosserial-arduino ros-melodic-rosserial


RUN echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc

RUN mkdir -p /home/catkin_ws/src
WORKDIR /home/catkin_ws/src
RUN /bin/bash -c '. /opt/ros/melodic/setup.bash; catkin_init_workspace /home/catkin_ws/src' \
    && git clone https://github.com/Jianing-Ju/thruster_controller.git

WORKDIR /home/catkin_ws
RUN /bin/bash -c '. /opt/ros/melodic/setup.bash; cd /home/catkin_ws; catkin_make'
RUN echo "source /home/catkin_ws/devel/setup.bash" >> ~/.bashrc




