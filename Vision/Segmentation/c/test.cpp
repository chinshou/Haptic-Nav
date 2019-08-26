#include <opencv2/opencv.hpp>
#include "tiny_deeplab_api.hpp"
#include <iostream>
#include <algorithm>

int main() {
	using namespace std;
	using namespace cv;

	// Initialize Deeplab object
	Deeplab dl = Deeplab();
	cout << "Successfully constructed Deeplab object" << endl;

	// Read/resize input image
	Mat image = imread("/Users/Daniel/Desktop/cat.jpg"); 
	int orig_height = image.size().height;
	int orig_width = image.size().width;
	double resize_ratio = (double) 513 / max(orig_height, orig_width);
	Size new_size((int)(resize_ratio*orig_width), (int)(resize_ratio*orig_height));
	Mat resized_image;
	resize(image, resized_image, new_size);
	cout << "Image resized (h, w): (" << orig_height << "," << orig_width << ") --> (" << new_size.height << ", " << new_size.width << ")" << endl;
//	imshow("Image", resized_image);
//	waitKey(0);

	vector<uint8_t> v(new_size.width*new_size.height*3);
	for(int i=0; i<v.size(); i++) {
		if(i%3==1) 
			v[i] = 255;
		else 
			v[i] = 0;
	}
	Mat myImg(new_size.height, new_size.width, CV_8UC3);
	memcpy(myImg.data, v.data(), new_size.width*new_size.height*3);
	imshow("Custom Image", myImg);
	waitKey(0);


	// Allocate input image object
	const int64_t dims_in[3] = {new_size.width, new_size.height, 3};
	image_t* img_in = (image_t*)malloc(sizeof(image_t));
	img_in->dims = &dims_in[0];
	//img_in->data_ptr = (uint8_t*)malloc(new_size.width*new_size.height*3);
	img_in->data_ptr = myImg.data;
	//for(int i=0; i<100; i++) {
	//	cout << (int)img_in->data_ptr[i] << ", ";
	//}
	img_in->bytes = new_size.width*new_size.height*3*sizeof(uint8_t);

	// Allocate output segmentation map object
	const int64_t dims_out[2] = {new_size.width, new_size.height};
	segmap_t* seg_out = (segmap_t*)malloc(sizeof(segmap_t));
	seg_out->dims = &dims_out[0];
	seg_out->data_ptr = (uint8_t*)malloc(new_size.width*new_size.height);
	seg_out->bytes = new_size.width*new_size.height*sizeof(uint8_t);
	

	// Run Deeplab
	cout << "Running segmentation" << endl;
	int status = dl.run_segmentation(img_in, seg_out);
	if(status != 0) {
		cout << "ERROR RUNNING SEGMENTATION: " << status << endl;
		return 1;
	}

	cout << "Successfully ran segmentation" << endl;

	// Interpret results

	return 0;
}

