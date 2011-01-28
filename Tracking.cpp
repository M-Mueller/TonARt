#include "Tracking.h"

float Tracking::sobelKernelArray[3][3] = {{-1.0, 0.0, 1.0}, {-2.0, 0.0, 2.0}, {1.0, 0.0, -1.0}};

Tracking::Tracking():
		sobelKernel(3, 3, CV_32FC1, Tracking::sobelKernelArray)
{
	video = new cv::VideoCapture(0);

	if(!video->isOpened())
	{
		std::cerr << "Could not find camera, trying video..." << std::endl;
		video->open("../MarkerMovie.mpg");
		if(!video->isOpened())
		{
			std::cerr << "Could not load video" << std::endl;
		}
	}

	cv::namedWindow("window", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("marker", CV_WINDOW_NORMAL);
}

cv::Mat Tracking::getFrame()
{
	cv::Mat frame;
	(*video) >> frame;
	return frame;
}

float Tracking::length(cv::Point p)
{
	return sqrt((double)(p.x*p.x + p.y*p.y));
}

int Tracking::subpixSampleSafe ( const IplImage* pSrc, cv::Point2f p )
{
	int x = int( floorf ( p.x ) );
	int y = int( floorf ( p.y ) );

	if ( x < 0 || x >= pSrc->width  - 1 ||
		 y < 0 || y >= pSrc->height - 1 )
		return 127;

	int dx = int ( 256 * ( p.x - floorf ( p.x ) ) );
	int dy = int ( 256 * ( p.y - floorf ( p.y ) ) );

	unsigned char* i = ( unsigned char* ) ( ( pSrc->imageData + y * pSrc->widthStep ) + x );
	int a = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
	i += pSrc->widthStep;
	int b = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
	return a + ( ( dy * ( b - a) ) >> 8 );
}

cv::Point2f Tracking::getEdgePos(cv::Mat stripe)
{
	double max;
	cv::Point maximumLoc;
	cv::minMaxLoc(stripe, NULL, &max, NULL, &maximumLoc);

	unsigned char y0 = stripe.at<unsigned char>(maximumLoc.y, maximumLoc.x);	//x=0

	unsigned char y1;
	if(maximumLoc.x<stripe.cols-1)
		y1 = stripe.at<unsigned char>(maximumLoc.y, maximumLoc.x+1); //x=1
	else
		y1 = y0;

	unsigned char y2;
	if(maximumLoc.x>=1)
		y2 = stripe.at<unsigned char>(maximumLoc.y, maximumLoc.x-1); //x=-1
	else
		y2 = y0;

	if(! (y0>=y1 && y0>=y2) )	//bug in cv??
	{
		return maximumLoc;
	}

	//f(x) = ax²+bx+c
	//f'(x) = 2ax+b
	//maximum: f'(x)=0 => x= -b/2a
	float a = (float(y2) + float(y1) - 2.0*float(y0) )/2.0;
	float b = float(y1) - float(y0) - a;

	if(a==0)
		return maximumLoc;
	else
	{
		return cv::Point2f(maximumLoc.x-(b/(2.0*a)), maximumLoc.y);
	}
}

cv::Point2f Tracking::getSubpixelBorderPosFromStripe(cv::Mat &input, cv::Point2f divCenter, cv::Point2f stripeX, cv::Point2f stripeY, int stripeWidth, int stripeHeight)
{
	cv::Point2f stripeStart = divCenter - floor(stripeWidth/2.0)*stripeX - floor(stripeHeight/2.0)*stripeY;	//upper, left corner of the stripe in the original image

	cv::Mat stripe(stripeWidth, stripeHeight, input.type());

	for(int y=0; y<stripeHeight; y++)
	{
		for(int x=0; x<stripeWidth; x++)
		{
			stripe.at<unsigned char>(x,y) = subpixSampleSafe(&(IplImage) input, (stripeStart + x*stripeX + y*stripeY));//input.at<char>(QVectorToCvPoint(stripeStart + x*stripeX + y*stripeY));
		}
	}

	cv::Mat stripeSobel(stripe.size().width, stripe.size().height, stripe.type());	//image containing the edge information

	cv::filter2D(stripe, stripeSobel, -1, sobelKernel);

	cv::Point2f stripeEdgePos = getEdgePos(stripeSobel); //subpixel edge coordinates in stripe

	return (stripeStart + stripeEdgePos.x*stripeY + stripeEdgePos.y*stripeX);
}

//calculates the subpixel accurate border along a line from start to end
cv::Vec4f Tracking::getAccurateBorder(cv::Point start, cv::Point end, cv::Mat &input, cv::Mat &output)
{
	//draw 6 subdivisions per line
	cv::Point d = (end-start);	//direction vector from s to e
	int stripeWidth=3;
	int stripeHeight=(int)(0.2 * length(d));

	if(stripeHeight<0.1)	//cv::filter2D crashs with too small values
		return cv::Vec4f();

	cv::Point2f stripeX(d.x/length(d), d.y/length(d)); //stripe x orientation

	//vector orthogonal to the line
	cv::Point2f stripeY(stripeX.y, -stripeX.x);

	std::vector<cv::Point2f> edgePositions;

	d=d*(1.0/7.0);	//scale d to length between two segments
	for(int l=1; l<7; l++) //calculate a stripe for each segment and calculate the subpixel accurate edge
	{
		cv::Point2f divCenter = start+(l*d);
		edgePositions.push_back(getSubpixelBorderPosFromStripe(input, divCenter, stripeX, stripeY, stripeWidth, stripeHeight));

#ifdef DEBUG
		cv::circle(output, edgePositions.at(edgePositions.size()-1), 4, CV_RGB(0,0, 255),2); //segment center
#endif
	}

	cv::Vec4f border;
	cv::fitLine(cv::Mat(edgePositions), border, CV_DIST_L2, 0, 0.01, 0.01);

	return border;
}

void Tracking::getCorners(cv::Vec4f* borders, cv::Point2f* corners)
{
	//for each corner intersect 2 lines
	for(int c=0; c<4; c++)
	{
		cv::Point2f v(borders[c][0], borders[c][1]); //direction vector of line one
		cv::Point2f a(borders[c][2], borders[c][3]); //center of line one

		cv::Point2f w; //direction vector of line two
		cv::Point2f b; //center of line two

		if(c<3)
		{
			w = cv::Point2f(borders[c+1][0], borders[c+1][1]); //direction vector of line two
			b = cv::Point2f(borders[c+1][2], borders[c+1][3]); //center of line two
		}
		else	//last line is intersected with first one
		{
			w = cv::Point2f(borders[0][0], borders[0][1]); //direction vector of line two
			b = cv::Point2f(borders[0][2], borders[0][3]); //center of line two
		}

		int s = ( (v.y*b.x - v.y*a.x + v.x*a.y - v.x*b.y)/v.x ) / (w.y - (v.y*w.x)/v.x);

		corners[c] = s*w + b;
	}
}

unsigned short Tracking::getMarkerID(cv::Mat &marker, int* rotations)
{
	unsigned short results[4];//at the end contains all possible ids of this marker (all rotations)

	unsigned short id=0;//temp storage for id
	for(int x=0; x<marker.size().width; x++)
	{
		for(int y=0; y<marker.size().height; y++)
		{
			//discard markers without black border
			if(y==0 || y==marker.size().height-1)//top and bottom
			{
				if(marker.at<unsigned char>(x,y)!=0)
					return -1;
				else
					continue;
			}
			if(x==0 || x==marker.size().width-1)//right and left
			{
				if(marker.at<unsigned char>(x,y)!=0)
					return -1;
				else
					continue;
			}

			id = id << 1;
			if(marker.at<unsigned char>(x,y)==0)	//if marker is black (== 0)
			{
				id |= 1;
			}
		}
	}
	results[0]=id;

	//other rotations:
	for(int y=1; y<marker.size().height-1; y++)
	{
		for(int x=marker.size().width-2; x>0; x--)
		{
			id = id << 1;
			if(marker.at<unsigned char>(x,y)==0)	//if marker is black (== 0)
			{
				id |= 1;
			}
		}
	}
	results[1]=id;

	for(int x=marker.size().width-2; x>0; x--)
	{
		for(int y=marker.size().height-2; y>0; y--)
		{
			id = id << 1;
			if(marker.at<unsigned char>(x,y)==0)	//if marker is black (== 0)
			{
				id |= 1;
			}
		}
	}
	results[2]=id;

	for(int y=marker.size().height-2; y>0; y--)
	{
		for(int x=1; x<marker.size().width-1; x++)
		{
			id = id << 1;
			if(marker.at<unsigned char>(x,y)==0)	//if marker is black (== 0)
			{
				id |= 1;
			}
		}
	}
	results[3]=id;

	//find minimum
	int temp1, temp2;
	results[0]<results[1] ? temp1=0 : temp1=1;
	results[2]<results[3] ? temp2=2 : temp2=3;

	results[temp1]<results[temp2] ? *rotations=temp1 : *rotations=temp2;

	return results[*rotations];
}

void Tracking::toCstyle(cv::Point2f* in, CvPoint2D32f* out, int length)
{
	for(int i=0; i<length; i++)
	{
		out[i]=cvPoint2D32f(in[i].x, in[i].y);
	}
}

void Tracking::getMarkers(std::vector<Marker> &markersFound)
{
	cv::Mat frame;
	(*video) >> frame;

	cv::Mat greyScale;
	cv::cvtColor(frame, greyScale, CV_RGB2GRAY);

	cv::Mat threshold;
	//cv::threshold(greyScale, threshold, 128, 255, cv::THRESH_BINARY);
	cv::adaptiveThreshold(greyScale, threshold, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 55, 15);

	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(threshold, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE); //modifies threshold!

	for(unsigned int i=0; i<contours.size(); i++)
	{
		std::vector<cv::Point> poly;
		cv::approxPolyDP(cv::Mat(contours.at(i)), poly, arcLength(cv::Mat(contours.at(i)), true)*0.02, true);

		if(poly.size()!=4)
			continue;

		cv::Rect rect = cv::boundingRect(cv::Mat(poly));
		if (rect.height < 10 || rect.width < 10 || rect.height >= frame.size().height - 10 || rect.width >= frame.size().width - 10)
		{
			continue;
		}

		cv::Vec4f borders[4];

		cv::Point s = poly.at(0);//line start
		cv::Point e = poly.at(0);//line end
		for(int k=0; k<4; k++)//calculate accurate borders
		{
			//set start and end points of the line
			s=e;
			if(k<3)
				e=poly.at(k+1);
			else
				e=poly.at(0);//connect last with first

			borders[k] = getAccurateBorder(s, e, greyScale, frame);

#ifdef DEBUG
			cv::line(frame, cv::Point(borders[k][2]-borders[k][0]*150, borders[k][3]-borders[k][1]*150), cv::Point(borders[k][2]+borders[k][0]*150, borders[k][3]+borders[k][1]*150), CV_RGB(0, 255,255), 2);
#endif
		}

		//calculate corners with the previous borders
		cv::Point2f corners[4];
		getCorners(borders, corners);

		//get marker image
		cv::Mat markerTemp; //temporary marker image
		cv::Mat marker; //final binary marker
		cv::Point2f markerCorners[4] = {cv::Point2f(5.5,-0.5), cv::Point2f(5.5,5.5), cv::Point2f(-0.5,5.5), cv::Point2f(-0.5,-0.5)};

		cv::Mat perspectiveTransformation = cv::getPerspectiveTransform(corners, markerCorners);
		cv::warpPerspective(greyScale, markerTemp, perspectiveTransformation, cv::Size(6,6), cv::INTER_NEAREST);

		cv::threshold(markerTemp, marker, 128, 255, cv::THRESH_BINARY);

		int rotations;
		unsigned int markerID=getMarkerID(marker, &rotations);
		if(markerID== 65535 || rotations > 3) //invalid marker
			continue;

		//shift corners to represent the minimal ID
		for(int i=0; i<rotations; i++)
		{
			cv::Point2f temp=corners[3];
			corners[3]=corners[2];
			corners[2]=corners[1];
			corners[1]=corners[0];
			corners[0]=temp;
		}

#ifdef DEBUG
		int colorInc=0;
		for(int c=0; c<4; c++)
		{
			cv::circle(frame, corners[c], 5, CV_RGB(colorInc, 0, 0), 2);
			colorInc+=64;
		}
#endif

		//move origin to the image center
		for(int i=0; i<4; i++)
		{
			corners[i].x -= frame.cols/2;
			corners[i].y = -corners[i].y + frame.rows/2;
		}

		float markerPosition[16];
		CvPoint2D32f cornersCStyle[4];
		toCstyle(corners, cornersCStyle, 4);
		estimateSquarePose(markerPosition, cornersCStyle, 0.042);

		markersFound.push_back(Marker(markerID, markerPosition));

#ifdef DEBUG
		cv::imshow("marker", marker);
		cv::imshow("window", frame);
		if(cv::waitKey(30) >= 0) break;
#endif
	}
}

