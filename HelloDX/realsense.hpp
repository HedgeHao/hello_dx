#pragma once

#include <librealsense2/rs.hpp>

class Rs {
public:
	Rs() {};
	~Rs() {};

	bool running = false;
	rs2::points rsPoints;
	std::optional<rs2::video_frame> colorFrame;

	void startPipeline() {
	  //rs2::config cfg;
	  //cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGB8, 30);
	  //cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480);
		rsPipe.start();
	}

	void stopPipeline() {
		rsPipe.stop();
	}

	void update() {
		rs2::frameset frames = rsPipe.wait_for_frames();
		colorFrame = std::make_optional<rs2::video_frame>(frames.get_color_frame());
		rs2::depth_frame depthFrame = frames.get_depth_frame();

		rsPointCloud.map_to(colorFrame.value());
		rsPoints = rsPointCloud.calculate(depthFrame);
	}
private:
	rs2::pipeline rsPipe;
	rs2::pointcloud rsPointCloud;
};