/*
 * TemplateMatching 1.0
 *
 * @author 	  Nashruddin Amin <me@nashruddin.com>
 * @version   1.0
 * @website	  http://www.nashruddin.com
 */

//clang++ -c template_match.cpp -I/usr/local/Cellar/opencv/2.2/include/opencv2/  -g -L/usr/local/Cellar/opencv/2.2/lib/libopencv_core.dylib -arch x86_64
//
//clang++ template_match.o -o template_matcher -L/usr/local/Cellar/opencv/2.2/lib/ -arch x86_64 -lopencv_core  -lopencv_highgui -lopencv_ml -lopencv_features2d -lopencv_contrib -lopencv_video -lopencv_imgproc



#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "opencv.hpp"
#include "highgui/highgui.hpp"

using namespace std;

int main( int argc, char** argv )
{
  IplImage	*img;
  IplImage	*img2;
  IplImage	*tpl;
  CvPoint		minloc, maxloc;
  double		minval, maxval;
  int			img_width, img_height;
  int			tpl_width, tpl_height;
  int			res_width, res_height;

  /* check for arguments */
  if( argc < 3 ) {
    fprintf( stderr, "Usage: template_match <reference> <template>\n" );
    return 1;
  }

  /* load reference image */
  img = cvLoadImage( argv[1], CV_LOAD_IMAGE_GRAYSCALE );

  /* always check */
  if( img == 0 ) {
    fprintf( stderr, "Cannot load file %s!\n", argv[1] );
    return 1; 
  }

  /* load template image */
  tpl = cvLoadImage( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

  /* always check */
  if( tpl == 0 ) {
    fprintf( stderr, "Cannot load file %s!\n", argv[2] );
    return 1;
  }

  /* get image's properties */
  img_width  = img->width;
  img_height = img->height;
  tpl_width  = tpl->width;
  tpl_height = tpl->height;

  int region_offset_x = 0;
  int region_offset_y = 0;
  int region_width = img_width / 4;
  int region_height = img_height / 4;

  //res_width  = img_width - tpl_width + 1;
  //res_height = img_height - tpl_height + 1;

  res_width  = region_width - tpl_width + 1;
  res_height = region_height - tpl_height + 1;

  /* create new image for template matching computation */

  IplImage	*res;
  res = cvCreateImage( cvSize( res_width, res_height ), IPL_DEPTH_32F, 1 );
  /* choose template matching method to be used */
  /*cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF_NORMED );
    cvMatchTemplate( img, tpl, res, CV_TM_CCORR );
    cvMatchTemplate( img, tpl, res, CV_TM_CCORR_NORMED );
    cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF );
    cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF_NORMED );*/

  string file_start("template_matches/template_match_");
  string file_ending(".jpg");

  int index = 0;
  bool change = false;
  bool final_row = false;

  while(1)
  {
    cvSetImageROI(img,cvRect(region_offset_x, region_offset_y, region_width, region_height));

    cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF_NORMED );
     cvMinMaxLoc( res, &minval, &maxval, &minloc, &maxloc, 0 );

     // cvRectangle( img, 
     //     cvPoint( minloc.x, minloc.y ), 
     //     cvPoint( minloc.x + tpl_width, minloc.y + tpl_height ),
     //     cvScalar( 0, 0, 255, 0 ), 1, 0, 0 );	

  
     cvResetImageROI(img);

     int actual_x = region_offset_x + minloc.x;
     int actual_y = region_offset_y + minloc.y;
     cvSetImageROI(img, 
         cvRect(actual_x, actual_y, tpl_width, tpl_height));

     cout << actual_x << "," << actual_y << endl;
     img2 = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
     cvCopy(img, img2, NULL);

     stringstream filename;
     filename << file_start;
     filename << index;
     filename << file_ending;

     cvSaveImage(filename.str().c_str(),img2);
     cvResetImageROI(img);

     if(change)
     {
       region_offset_y += region_height / 2;
       region_offset_x = 0;
       change = false;
       if(region_offset_y + region_height > img_height)
       {
         region_offset_y = img_height - region_height;
         final_row = true;
       }
           
     }
     else
     {
       region_offset_x += region_width / 2;
       if(region_offset_x + (region_width)  > img_width)
       {
         region_offset_x = img_width - region_width;
         change = true;
         if(final_row)
         {
           break;
         }
       }
     }
     index++;
  }

  /* display images */
  //cvNamedWindow( "reference", CV_WINDOW_AUTOSIZE );
  //cvNamedWindow( "template", CV_WINDOW_AUTOSIZE );
  //cvShowImage( "reference", img );
  //cvShowImage( "template", tpl );

  /* wait until user press a key to exit */
  //cvWaitKey( 0 );

  //cvSaveImage("match.jpg",img);
  /* free memory */
  //cvDestroyWindow( "reference" );
  //cvDestroyWindow( "template" );
  cvReleaseImage( &img );
  cvReleaseImage( &img2 );
  cvReleaseImage( &tpl );
  cvReleaseImage( &res );

  return 0;
}

