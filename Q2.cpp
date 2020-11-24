#include<iostream>
#include <cstdlib> 
#include<math.h>
#include <sys/time.h>
#include "x86intrin.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

int main()
{
	// Print students ID
	cout << "810896045___810196510" << endl;

	struct timeval start, end;
	long double time1, time2;

	// Read images
	cv::Mat in_img1 = cv::imread("CA03__Q2__Image__01.png", cv::IMREAD_GRAYSCALE);
	cv::Mat in_img2 = cv::imread("CA03__Q2__Image__02.png", cv::IMREAD_GRAYSCALE);

	cv::Mat out_img(in_img1.rows, in_img1.cols, CV_8U);

	//Pointer to the images data(Matrix of Pixels)
	unsigned char *in_image1;
	in_image1 = (unsigned char*) in_img1.data;

	unsigned char *in_image2;
	in_image2 = (unsigned char*) in_img2.data;

	unsigned char *out_image;
	out_image = (unsigned char*) out_img.data;

	unsigned int NROWS = in_img1.rows;
	unsigned int NCOLS = in_img1.cols;

	// Serial implementation
	gettimeofday(&start, NULL);

	for(int row = 0; row < NROWS; row++)
	{
		for(int col = 0; col < NCOLS; col++)
		{
			*(out_image + row * NCOLS + col) = *(in_image1 + row * NCOLS + col) + *(in_image2 + row * in_img2.cols + col) * 0.5 * (row < in_img2.rows && col < in_img2.cols);
			// *(out_image + row * NCOLS + col) = *(out_image + row * NCOLS + col) > 255 ? 255 : *(out_image + row * NCOLS + col);
		}
	}

	gettimeofday(&end, NULL);

	// Duration
	time1 = end.tv_sec - start.tv_sec;
	time1 = (time1 * 1000000) + end.tv_usec - start.tv_usec;
	
	__m128i *pSrc1;
	__m128i *pSrc2;
	__m128i *pRes;
	__m128i m1, m2, m3;

	pSrc1 = (__m128i *) in_img1.data;
	pSrc2 = (__m128i *) in_img2.data;
	pRes = (__m128i *) out_img.data;
	
	// Parallel implementation
	float Alpha = 0.5;
	// start = ippGetCpuClocks();
	gettimeofday(&start, NULL);

	// for (int i = 0; i < NROWS; i++)
	// {
	// 	for (int j = 0; j < NCOLS / 16; j++)
	// 	{
	// 		m1 = _mm_loadu_si128 (pSrc1 + i * NCOLS/16 + j);
	// 		m2 = _mm_loadu_si128 (pSrc2 + i * NCOLS/16 + j);
	// 		m2 >> 1;
	// 		__m128i num = _mm_set1_epi8(127);
	// 		m2 = _mm_and_si128 (m2, num);
	// 		m3 = _mm_add_epi8 (m1, m2);
	// 		_mm_storeu_si128 (pRes + i * NCOLS/16 + j, m3);
	// 	}
	// }

	gettimeofday(&end, NULL);
	// end = ippGetCpuClocks();
	time2 = end.tv_sec - start.tv_sec;
	time2 = (time2 * 1000000) + end.tv_usec - start.tv_usec;

	// Display output image
	cv::namedWindow("output_image", cv::WINDOW_AUTOSIZE);		// Creat a window to display output image
	cv::imshow("output_image", out_img);		// Show image

	// Display Run times and SpeedUp
	printf ("Serial Run time (us) = %Lf \n", time1);
	printf ("Parallel Run time (us) = %Lf \n", time2);
	printf ("Speedup = %4.2f\n", (float) (time1)/(float) time2);
	cv::waitKey(0);
			
	return 0;
}