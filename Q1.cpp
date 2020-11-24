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
	// [rint students ID
	cout << "810896045___810196510" << endl;

	struct timespec start, end;
	long double time1, time2;
	
	// Read images
	cv::Mat in_img1 = cv::imread("CA03__Q1__Image__01.png");
	cv::Mat in_img2 = cv::imread("CA03__Q1__Image__02.png");
	cv::Mat out_img(in_img1.rows, in_img1.cols, CV_8U);
	
	// Pointer to the images data(Matrix of Pixels)
	unsigned char *in_image1;
	in_image1 = (unsigned char*) in_img1.data;

	unsigned char *in_image2;
	in_image2 = (unsigned char*) in_img2.data;

	unsigned char *out_image;
	out_image = (unsigned char*) out_img.data;

	// Serial implementation
	unsigned int NROWS = in_img1.rows;
	unsigned int NCOLS = in_img1.cols;

	// start = ippGetCpuClocks();
	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int row = 0; row < NROWS; row++)
		for(int col = 0; col < NCOLS; col++)
			*(out_image + row * NCOLS + col) = abs(*(in_image1 + row * NCOLS + col) - *(in_image2 + row * NCOLS + col));
	// end = ippGetCpuClocks();
	clock_gettime(CLOCK_MONOTONIC, &end);

	// Duration
	time1 = end.tv_sec - start.tv_sec;
	time1 = (time1 * 1000000000) + end.tv_nsec - start.tv_nsec;

	
	__m128i *pSrc1;
	__m128i *pSrc2;
	__m128i *pRes;
	__m128i m1, m2, m3, m4, m5;

	pSrc1 = (__m128i *) in_img1.data;
	pSrc2 = (__m128i *) in_img2.data;
	pRes = (__m128i *) out_img.data;
	
	// Parallel implementation
	// start = ippGetCpuClocks();
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (int i = 0; i < NROWS; i++)
		for (int j = 0; j < NCOLS / 16; j++)
		{
			m1 = _mm_loadu_si128(pSrc1 + i * NCOLS/16 + j);
			m2 = _mm_loadu_si128(pSrc2 + i * NCOLS/16 + j);
			m3 = _mm_subs_epu8(m1, m2);
			m4 = _mm_subs_epu8(m2, m1);
			m5 = _mm_or_si128(m3, m4);
			_mm_storeu_si128 (pRes + i * NCOLS/16 + j, m5);
		}
	// end   = ippGetCpuClocks();
	clock_gettime(CLOCK_MONOTONIC, &end);
	time2 = end.tv_sec - start.tv_sec;
	time2 = (time2 * 1000000000) + end.tv_nsec - start.tv_nsec;

	// Display Run times and SpeedUp
	printf ("Serial Run time (ns) = %Lf \n", time1);
	printf ("Parallel Run time (ns) = %Lf \n", time2);
	printf ("Speedup = %4.2f\n", (float) (time1)/(float) time2);

	return 0;
}