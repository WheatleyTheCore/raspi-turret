#include <iostream>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <wiringSerial.h>
#include <raspicam/raspicam_cv.h>

using namespace cv;

double min_size = 20;
double max_size = 200;
double fx = 1;
std::vector<Rect> faces;
int state = 0;
int count;
int init = 1;

void randomPlay()
{
	int randomNum = std::rand() % 4;
	if (randomNum == 0)
	{
		system("omxplayer --no-keys /home/pi/OPENCV/sounds/data/takemewithyou.mp3 &");
	} else if (randomNum == 1) 
	{
		system("omxplayer --no-keys /home/pi/OPENCV/sounds/data/searching.mp3 &");
	} else if (randomNum == 2) {
		system("omxplayer --no-keys /home/pi/OPENCV/sounds/data/targetlost.mp3 &");
	} else if (randomNum == 3) {
		system("omxplayer --no-keys /home/pi/OPENCV/sounds/data/Idonthateyou.mp3 &");
	}
}

void handleCount()
{
	if (init == 0)
	{
        	switch(count)
                	{
                        	case 10 : system("omxplayer --no-keys /home/pi/OPENCV/sounds/data/Stillthere.mp3 &");
				break;
				case 50 : randomPlay();
				break;
		       	}
	}
}

int main(int argc, char** argv)
{
	Mat output;

	raspicam::RaspiCam_Cv Camera;

	if( !Camera.open() )
        {
                std::cout << "Error initializing camera \n";
                return 0;
        }


	system("omxplayer /home/pi/OPENCV/sounds/data/Hellofriend.mp3 &");

	CascadeClassifier cascade;
	cascade.load("/home/pi/OPENCV/opencv-3.4.0/data/haarcascades/haarcascade_frontalface_alt.xml");


	while(1)
	{
		Camera.grab();
		Camera.retrieve(output);

		Mat gray, resizedImg;

	        cvtColor( output, gray, COLOR_BGR2GRAY );

        	resize( gray, resizedImg, Size(), fx, fx, INTER_LINEAR );

        	equalizeHist( resizedImg, resizedImg );

        	cascade.detectMultiScale( gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(min_size, max_size) );

		if (faces.size() > 0 && state == 0) 
		{
			system("omxplayer --no-keys /home/pi/OPENCV/sounds/data/Iseeyou.mp3 &");
			state = true;
			init = 0;
			count = 0;
		}

		if (faces.size() <= 0)
		{
			count++;
			if (count == 3)
			{
				state = 0;
			}
		}

		handleCount();

        	for ( size_t i = 0; i < faces.size(); i++ ) 
        	{
             		Rect r = faces[i]; 
                	Mat smallImgROI; 
               		std::vector<Rect> nestedObjects; 
               		Point center; 
               		Scalar color = Scalar(255, 0, 0); 
            		rectangle( output, cvPoint(cvRound(r.x), cvRound(r.y)), 
                       		cvPoint(cvRound((r.x + r.width-1)),  
        	        	cvRound((r.y + r.height-1))), color, 3, 8, 0); 
        	}


		imshow("piCam", output);

		char c = (char)waitKey(10);
		if( c == 27 )
		{
			system("omxplayer /home/pi/OPENCV/sounds/data/goodnight.mp3 &");
			break;
		}
	}
}
