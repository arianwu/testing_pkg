#include "testing_pkg/tf_filter.h"

class TFFilterNode{
public:
	TFFilterNode();
	~TFFilterNode();

	void commandCallback(const std_msgs::Float64MultiArray& msg);
private:
	ros::NodeHandle nh;
	ros::Publisher pub_result;
	ros::Subscriber sub_input;

	std_msgs::Float64MultiArray send_msg;

	std::vector<double> input;
	std::vector<double> output;
	filters::MultiChannelFilterBase<double>* filter;
};

TFFilterNode::TFFilterNode(){
	// ROS Infrastructure
	pub_result = nh.advertise<std_msgs::Float64MultiArray>("topic", 5);
	sub_input = nh.subscribe("command", 1000, &TFFilterNode::commandCallback, this);

	// Filter
	filter = new filters::MultiChannelTransferFunctionFilter<double>();
	filter->configure(6, "elfin_command_filter");
	output.resize(6);
}

TFFilterNode::~TFFilterNode(){}

void TFFilterNode::commandCallback(const std_msgs::Float64MultiArray& msg){
	input = msg.data;
	filter->update(input, output);
	send_msg.data = output;
	pub_result.publish(send_msg);
}


int main(int argc, char **argv){
	ros::init(argc, argv, "testing_node");

	TFFilterNode node;

	while (ros::ok()){
		ros::spin();
	}

	return 0;
}