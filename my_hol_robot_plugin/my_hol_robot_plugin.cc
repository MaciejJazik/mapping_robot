#ifndef _HOLONOMOUS_ROBOT_PLUGIN_HH_
#define _HOLONOMOUS_ROBOT_PLUGIN_HH_

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/Float32.h"
#include "geometry_msgs/Twist.h"

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

		  // Setup a P-controller, with a gain of 0.1.
		  this->pid = common::PID(0.1, 0.1, 0.1);

		  // Apply the PID-controller to the joint.
		  this->model->GetJointController()->SetVelocityPID(
			  this->jointLeft->GetScopedName(), this->pid);
			  
			this->model->GetJointController()->SetVelocityPID(
			  this->jointRight->GetScopedName(), this->pid);

		// Default to zero velocity
		double LeftWheelVelocity = 0;
		double RightWheelVelocity = 0;

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
		  ros::SubscribeOptions::create<std_msgs::Float32>(
			  "/" + this->model->GetName() + "/cmd_vel",
			  2,
			  boost::bind(&HolonomousRobotPlugin::velCallback, this, _1),
			  ros::VoidPtr(), &this->rosQueue);
			  
		this->rosSub1 = this->rosNode1->subscribe(so);

		/*this->rosNode2.reset(new ros::NodeHandle("gazebo_client"));

		// Create a named topic, and subscribe to it.
		so =
		  ros::SubscribeOptions::create<std_msgs::Float32>(
			  "/" + this->model->GetName() + "/vel_cmd_right",
			  2,
			  boost::bind(&HolonomousRobotPlugin::OnRosMsgR, this, _1),
			  ros::VoidPtr(), &this->rosQueue);
			  
		this->rosSub2 = this->rosNode2->subscribe(so);*/
		
		// Spin up the queue helper thread.
		this->rosQueueThread =
		  std::thread(std::bind(&HolonomousRobotPlugin::QueueThread, this));
    }
	
	/// \brief Set the velocity of the Velodyne
		/// \param[in] _vel New target velocity
		public: void SetLeftWheelVelocity(const double &_vel)
		{
		  // Set the joint's target velocity.
		  this->model->GetJointController()->SetVelocityTarget(
			  this->jointLeft->GetScopedName(), _vel);
		}
		
		public: void SetRightWheelVelocity(const double &_vel)
		{
		  // Set the joint's target velocity.
		  this->model->GetJointController()->SetVelocityTarget(
			  this->jointRight->GetScopedName(), _vel);
		}
		
		void velCallback(const std_msgs::Float32ConstPtr &_msg)
		{
		   this->SetRightWheelVelocity(_msg->data);
		   this->SetLeftWheelVelocity(_msg->data);
		   
		}
		
		/// \brief Handle an incoming message from ROS
		/// \param[in] _msg A float value that is used to set the velocity
		/// of the Velodyne.
		public: void OnRosMsgL(const std_msgs::Float32ConstPtr &_msg)
		{
		  this->SetLeftWheelVelocity(_msg->data);
		}
		
		public: void OnRosMsgR(const std_msgs::Float32ConstPtr &_msg)
		{
		  this->SetRightWheelVelocity(_msg->data);
		}

		/// \brief ROS helper function that processes messages
		private: void QueueThread()
		{
		  static const double timeout = 0.01;
		  while (this->rosNode1->ok())//&&this->rosNode2->ok()
		  {
			this->rosQueue.callAvailable(ros::WallDuration(timeout));
		  }
		}
		
		/// \brief Handle incoming message
		/// \param[in] _msg Repurpose a vector3 message. This function will
		/// only use the x component.
		/*private: void OnMsg(ConstVector3dPtr &_msg)
		{
		  this->SetVelocity(_msg->x());
		}*/
		
		/// \brief A node use for ROS transport
		private: std::unique_ptr<ros::NodeHandle> rosNode1;
		//private: std::unique_ptr<ros::NodeHandle> rosNode2;

		/// \brief A ROS subscriber
		private: ros::Subscriber rosSub1;
		//private: ros::Subscriber rosSub2;

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