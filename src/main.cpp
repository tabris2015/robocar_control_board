
#include <mbed.h>
#include <Servo.h>
#include <Motor.h>
#include <ros.h>

// ros msgs

#include <geometry_msgs/Twist.h> // sub to Twist message for control
#include <sensor_msgs/Range.h>   // pub to Range message for sensor

DigitalOut led(D13);

Servo steering(D9);            // steering servo
Motor motor(D12, D10, D11); // motor driver

// ROS part
// software utils
Timer t;
Ticker pub_ticker;
// ros part
ros::NodeHandle nh;

// messages
geometry_msgs::Twist control_cmd; // control command is of type twist

// callback prototypes
void ctrlCommandCb(const geometry_msgs::Twist &command);

// subscribers
// ros subscriber for control command
ros::Subscriber<geometry_msgs::Twist> ctrlSub("cmd_vel", ctrlCommandCb);

// publishers
// ros pub for laser range sensor
// helper prototypes

// prototypes 
// callbacks
void ctrlCommandCb(const geometry_msgs::Twist &command);

int main() {
    motor.period(0.0005);     // frecuencia 2 Khz
    // put your setup code here, to run once:
    steering = 0;
    // wait(1);
    // steering = 0.5;
    // wait(1);
    // steering = 1.0;
    // wait(1);

    for(float s = 0; s <= 1; s += 0.2)
    {
        motor.speed(s);
        wait(0.1);
    }
    for(float s = 1; s >= 0; s -= 0.2)
    {
        motor.speed(s);
        wait(0.1);
    }

    for(float s = 0; s >= -1; s -= 0.2)
    {
        motor.speed(s);
        wait(0.1);
    }
    for(float s = -1; s <= 0; s += 0.2)
    {
        motor.speed(s);
        wait(0.1);
    }

    // init sensors
    led = 1;
    // ros initialization
    nh.initNode();
    nh.subscribe(ctrlSub);
    //
    led = 0;

    while(1) {
        nh.spinOnce();
        wait_ms(1);
    }
}

// callbacks

// control command callback
void ctrlCommandCb(const geometry_msgs::Twist &command)
{
    led = !led;

    float linear = command.linear.x;
    float angular = command.angular.z / 2 + 0.5; // (command.angular.z * 0.35) + 0.32;

    steering = angular;

    // testigo = abs(linear * 8);
    motor.speed(linear);
}

