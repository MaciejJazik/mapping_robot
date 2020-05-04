#ifndef _HOLONOMOUS_ROBOT_PLUGIN_HH_
#define _HOLONOMOUS_ROBOT_PLUGIN_HH_

#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <thread>
#include <stdlib.h>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/Float32.h"
#include "geometry_msgs/Twist.h"
#include "gazebo/util/system.hh"
#include "gazebo/transport/TransportTypes.hh"
#include "gazebo/common/Plugin.hh"

#define TWIST_VELOCITY_FACTOR 0.35095316
#define LINEAR_VELOCITY_FACTOR 0.06993007
#define PI 3.14159265359

enum {LEFT, RIGHT};

namespace gazebo
{
	
  /// \brief A plugin to control a HolonomousRobot sensor.
  class HolonomousRobotPlugin : public ModelPlugin
  {
	  
		/// \brief Constructor
		public: HolonomousRobotPlugin() {}

		/// \brief The load function is called by Gazebo when the plugin is
		/// inserted into simulation
		/// \param[in] _model A pointer to the model that this plugin is
		/// attached to.
		/// \param[in] _sdf A pointer to the plugin's SDF element.
		public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
		{
		  // Just output a message for now
		  std::cerr << "\nThe HolonomousRobot plugin is attach to model[" <<
			_model->GetName() << "]\n";
			
			// Safety check
		  if (_model->GetJointCount() == 0)
		  {
			std::cerr << "Invalid joint count, Velodyne plugin not loaded\n";
			return;
		  }

		  // Store the model pointer for convenience.
		  this->model = _model;

		  // Get the first joint. We are making an assumption about the model
		  // having one joint that is the rotational joint.
		  this->jointLeft = _model->GetJoints()[0];
		  this->jointRight = _model->GetJoints()[1];
		  std::cerr<<"\njoints names is :"<<_model->GetJoints()[0]->GetName()<<", "<<_model->GetJoints()[1]->GetName()<<"\n";

		  // Setup a PID-controller
		  this->pid = common::PID(0.6, 1.0, 0.3);

		  // Apply the PID-controller to the joint.
		  this->model->GetJointController()->SetVelocityPID(
			  this->jointLeft->GetScopedName(), this->pid);
			  
			this->model->GetJointController()->SetVelocityPID(
			  this->jointRight->GetScopedName(), this->pid);

		// Default to zero velocity
		LeftWheelVelocity = 0;
		RightWheelVelocity = 0;
		loopsWithoutPublishingVelocityData=0;
		avg_linear_vel=0;
		avg_twist_vel=0;

		this->SetLeftWheelVelocity(LeftWheelVelocity);
		this->SetRightWheelVelocity(RightWheelVelocity);
		
		// Initialize ros, if it has not already bee initialized.
		if (!ros::isInitialized())
		{
		  int argc = 0;
		  char **argv = NULL;
		  ros::init(argc, argv, "gazebo_client",
			  ros::init_options::NoSigintHandler);
		}

		// Create our ROS node. This acts in a similar manner to
		// the Gazebo node
		this->rosNode1.reset(new ros::NodeHandle("gazebo_client"));

		// Create a named topic, and subscribe to it.
		ros::SubscribeOptions so =
		  ros::SubscribeOptions::create<geometry_msgs::Twist>(
			  "/" + this->model->GetName() + "/cmd_vel",
			  2,
			  boost::bind(&HolonomousRobotPlugin::velCallback, this, _1),
			  ros::VoidPtr(), &this->rosQueue);
			  
		this->rosSub1 = this->rosNode1->subscribe(so);
		
		this->velocityPublisher = this->rosNode1->advertise<geometry_msgs::Twist>("/my_hol_robot/geometry_msg", 1000);
		
		// Spin up the queue helper thread.
		this->rosQueueThread =
		  std::thread(std::bind(&HolonomousRobotPlugin::QueueThread, this));
    }
		
		void velCallback(const geometry_msgs::TwistConstPtr &_msg)
		{
		   this->SetRightWheelVelocity(_msg->linear.x-_msg->angular.z);
		   this->SetLeftWheelVelocity(_msg->linear.x+_msg->angular.z);
		   double twistVel =(avg_twist_vel/(double)loopsWithoutPublishingVelocityData) * TWIST_VELOCITY_FACTOR;
		   double linearVel = (avg_linear_vel/(double)loopsWithoutPublishingVelocityData) * LINEAR_VELOCITY_FACTOR;
		   geometry_msgs::Twist velocityData;
		   velocityData.linear.x = linearVel;
		   velocityData.linear.y = 0;
		   velocityData.linear.z = 0;
		   velocityData.angular.x = 0;
		   velocityData.angular.y = 0;
		   velocityData.angular.z = twistVel;
		   this->velocityPublisher.publish(velocityData);
		   loopsWithoutPublishingVelocityData=0;
		   avg_linear_vel=0;
		   avg_twist_vel=0;
		   
		}



		/// \brief Set the velocity of the Velodyne
		/// \param[in] _vel New target velocity
		public: void SetLeftWheelVelocity(const double &_vel)
		{
		  // Set the joint's target velocity.
		  LeftWheelVelocity=_vel;
			this->model->GetJointController()->SetVelocityTarget(
				this->jointLeft->GetScopedName(), _vel);
		}
		
		public: void SetRightWheelVelocity(const double &_vel)
		{
		  // Set the joint's target velocity.
		  RightWheelVelocity=_vel;
			this->model->GetJointController()->SetVelocityTarget(
				this->jointRight->GetScopedName(), _vel);
		}

		/// \brief ROS helper function that processes messages
		private: void QueueThread()
		{
			double current_angle_L;
			double current_angle_R;
			double current_vel_L;
			double current_vel_R;
		  static const double timeout = 0.01;
		  while (this->rosNode1->ok())
		  {
			this->rosQueue.callAvailable(ros::WallDuration(timeout));
			current_angle_L = this->jointLeft->Position(0);
			current_angle_R = this->jointRight->Position(0);
			current_vel_L = this->jointLeft->GetVelocity(0);
			current_vel_R = this->jointRight->GetVelocity(0);
			if(LeftWheelVelocity==0&&RightWheelVelocity==0)
			{
				this->jointRight->SetPosition(0,current_angle_R);
				this->jointLeft->SetPosition(0,current_angle_L);
			}
			else
			{
				if(current_vel_L>0&&current_vel_R<0)
					avg_twist_vel+=(current_vel_L-current_vel_R)/2;
				else if(current_vel_L<0&&current_vel_R>0)
					avg_twist_vel-=(abs(current_vel_R)+abs(current_vel_L))/2;
				else
					avg_linear_vel+=(current_vel_L+current_vel_R)/2;
			}
			
			loopsWithoutPublishingVelocityData++;
//			if(avg_twist_vel!=0||avg_linear_vel!=0)
//				std::cerr<<"avg twist velocity: "<<(avg_twist_vel/(double)loopsWithoutPublishingVelocityData)<<" avg lin velocity: "<< (avg_linear_vel/(double)loopsWithoutPublishingVelocityData)<<"\n";
		  }
		}
		
		
		private: double avg_twist_vel;
		private: double avg_linear_vel;
		private: int loopsWithoutPublishingVelocityData;
		private: double LeftWheelVelocity;
		private: double RightWheelVelocity;
		
		private: ros::Publisher velocityPublisher;
		
		/// \brief A node use for ROS transport
		private: std::unique_ptr<ros::NodeHandle> rosNode1;

		/// \brief A ROS subscriber
		private: ros::Subscriber rosSub1;

		/// \brief A ROS callbackqueue that helps process messages
		private: ros::CallbackQueue rosQueue;

		/// \brief A thread the keeps running the rosQueue
		private: std::thread rosQueueThread;
		
		/// \brief A node used for transport
		private: transport::NodePtr node;

		/// \brief A subscriber to a named topic.
		private: transport::SubscriberPtr sub;
	
	/// \brief Pointer to the model.
	private: physics::ModelPtr model;

	/// \brief Pointer to the joint.
	private: physics::JointPtr jointLeft;
	private: physics::JointPtr jointRight;

	/// \brief A PID controller for the joint.
	private: common::PID pid;
  };
  // Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
  GZ_REGISTER_MODEL_PLUGIN(HolonomousRobotPlugin)
}
#endif