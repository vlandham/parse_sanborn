
//clang++ -c line_find.cpp -I/usr/local/Cellar/opencv/2.2/include/opencv2/  -g -L/usr/local/Cellar/opencv/2.2/lib/libopencv_core.dylib -arch x86_64
//
//clang++ line_find.o -o line_finder -L/usr/local/Cellar/opencv/2.2/lib/ -arch x86_64 -lopencv_core  -lopencv_highgui -lopencv_ml -lopencv_features2d -lopencv_contrib -lopencv_video -lopencv_imgproc

#include <stdio.h>
#include "opencv.hpp"
#include "highgui/highgui.hpp"

using namespace cv;

int main( int argc, char** argv )
{
  IplImage	*src;
  IplImage	*res;
  CvPoint		minloc, maxloc;
  double		minval, maxval;
  int			img_width, img_height;
  int			tpl_width, tpl_height;
  int			res_width, res_height;

  /* check for arguments */
  if( argc < 2 ) {
    fprintf( stderr, "Usage: line_find <reference>\n" );
    return 1;
  }

  /* load reference image */
  src = cvLoadImage( argv[1], 0 );

  /* always check */
  if( src == 0 ) {
    fprintf( stderr, "Cannot load file %s!\n", argv[1] );
    return 1; 
  }
    IplImage* dst = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* color_dst = cvCreateImage( cvGetSize(src), 8, 3 );
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
    int i;
    cvCanny( src, dst, 10, 300, 3 );
    cvCvtColor( dst, color_dst, CV_GRAY2BGR );
#if 0
    lines = cvHoughLines2( dst,
        storage,
        CV_HOUGH_STANDARD,
        1,
        CV_PI/180,
        100,
        0,
        0 );

    for( i = 0; i < MIN(lines->total,100); i++ )
    {
      float* line = (float*)cvGetSeqElem(lines,i);
      float rho = line[0];
      float theta = line[1];
      CvPoint pt1, pt2;
      double a = cos(theta), b = sin(theta);
      double x0 = a*rho, y0 = b*rho;
      pt1.x = cvRound(x0 + 1000*(-b));
      pt1.y = cvRound(y0 + 1000*(a));
      pt2.x = cvRound(x0 - 1000*(-b));
      pt2.y = cvRound(y0 - 1000*(a));
      cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, 8 );
    }
#else
    lines = cvHoughLines2( dst,
        storage,
        CV_HOUGH_PROBABILISTIC,
        1,
        CV_PI/180,
        80,
        50,
        30 );
    for( i = 0; i < lines->total; i++ )
    {
      CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
      cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 1, 8 );
    }
#endif
    cvNamedWindow( "Source", 1 );
    cvShowImage( "Source", src );

    cvNamedWindow( "Hough", 1 );
    cvShowImage( "Hough", color_dst );

    cvWaitKey(0);
  return 0;
}

