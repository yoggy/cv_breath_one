#include "stdafx.h"

void usage()
{
	printf("usage : cv_breath_one [start_position_frame] [input_moviefile.m4v] [output_imagefile.png]\n");
	printf("      \n");
	exit(0);
}

int main(int argc, char* argv[])
{
	if (argc != 4) usage();

	int start_frame = atoi(argv[1]);
	int frame_num = 60 * 30;        // 60sec * 30fps;
	std::string input_moviefile = argv[2];
	std::string output_imagefile = argv[3];

	cv::VideoCapture capture(input_moviefile);
	capture.set(CV_CAP_PROP_POS_FRAMES, start_frame);

	int count = 0;
	int h = frame_num / 5;
	cv::Mat breath_img(cv::Size(frame_num, h), CV_8UC3);
	breath_img = 0;

	while (true) {
		cv::Mat capture_img;
		capture >> capture_img;

		if (!capture_img.empty()) {
			cv::imshow("capture_img", capture_img);

			cv::Rect slit_roi(capture_img.cols / 2, 0, 1, capture_img.rows);
			cv::Mat slit_img = capture_img(slit_roi);

			cv::Rect dst_roi(count, 0, 1, breath_img.rows);
			cv::resize(slit_img, breath_img(dst_roi), cv::Size(1, breath_img.rows));
		}
		cv::Mat resized_img;
		cv::resize(breath_img, resized_img, cv::Size(breath_img.cols / 2, breath_img.rows / 2));
		cv::imshow("breath_one", resized_img);

		count++;
		if (count >= frame_num) {
			break;
		}

		int c = cv::waitKey(1);
		if (c == 27) break;
	}

	cv::imwrite(output_imagefile, breath_img);

	capture.release();
	cv::destroyAllWindows();

	return 0;
}

