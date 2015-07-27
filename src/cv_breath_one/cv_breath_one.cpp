#include "stdafx.h"

void usage()
{
	printf("usage : cv_breath_one [input_moviefile.m4v] [start_position_frame] [start_position_frame] [frame_count] [frame_step]\n");
	printf("      \n");
	exit(0);
}

int start_frame = 0;
int frame_count = 60 * 30;  // 60sec * 30fps;
int frame_step = 1;

int main(int argc, char* argv[])
{
	if (argc == 1) usage();

	std::string input_moviefile = argv[1];
	std::string output_imagefile = input_moviefile + ".jpg";

	cv::VideoCapture capture(input_moviefile);

	if (argc >= 3) {
		int start_frame = atoi(argv[2]);
	}
	capture.set(CV_CAP_PROP_POS_FRAMES, start_frame);

	if (argc >= 4) {
		frame_count = atoi(argv[3]);
		if (frame_count < 0) {
			frame_count = (int)capture.get(CV_CAP_PROP_FRAME_COUNT);
		}
	}

	if (argc >= 5) {
		frame_step = atoi(argv[4]);
	}

	int count = 0;
	int h = 200;
	cv::Mat breath_img(cv::Size(frame_count / frame_step, h), CV_8UC3);
	breath_img = 0;

	while (true) {
		printf("%d/%d\n", count, frame_count);
		cv::Mat capture_img;
		capture >> capture_img;

		if (count % frame_step == 0) {
			if (!capture_img.empty()) {
				cv::imshow("capture_img", capture_img);

				cv::Rect slit_roi(capture_img.cols / 2, 0, 1, capture_img.rows);
				cv::Mat slit_img = capture_img(slit_roi);

				int dst_x = count / frame_step;
				if (dst_x < breath_img.cols - 1) {
					cv::Rect dst_roi(count / frame_step, 0, 1, breath_img.rows);

					cv::resize(slit_img, breath_img(dst_roi), cv::Size(1, breath_img.rows));
				}

				cv::Mat resized_img;
				cv::resize(breath_img, resized_img, cv::Size(breath_img.cols / 2, breath_img.rows / 2));
				cv::imshow("breath_one", resized_img);
			}
		}

		count++;
		if (count >= frame_count) {
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

