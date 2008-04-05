#include "curtain.h"
#include "blackglobals.h"
#include "Point2D.h"
#include <string.h>

static Buffer2D<PixelRGB> barim;
static Buffer2D<bool> boolimg;
static Buffer2D<bool> boolarr;

void RGBtoBool (Buffer2D<PixelRGB> img,Buffer2D<PixelRGB>& dst) {
	int ii;
	int buffLength = img->width * img->height;

	/*standard boolean conversion*/
	for(ii = 0; ii < buffLength; ii++){
		if(img[ii].r > 0 || img[ii].g > 0 || img[ii].b > 0)
			dst[ii] == 1;
		else
			dst[ii] == 0;
	}
}

void dialate1D (Buffer2D<bool> arr) {
	int ii;
	int buffLength = arr->width * arr->height;

	/*if current pos or neighbor pos is 1, then current pos is 1*/
	for(ii = 1; ii < buffLength-1; ii++){
		if(arr[ii-1] || arr[ii] || arr[ii+1){
			boolarr[ii] = 1;
		}
		else
			boolarr[ii] = 0;
	}	
	memcpy(arr.data, boolarr.data, sizeof(bool)*buffLength);
}

b2drgb& curtain (Buffer2D<PixelRGB> whim, Buffer2D<PixelRGB> orim) {
	
	int ii = 0;
	int orangeIndex = 0;
	int buffLength = orim->width * orim->height;
	bool isOrange = true;
	
	/*get boolean orange image*/
	boolim = RGBtoBool(orim);
	
	/*loop untill no more orange pixels*/
	while(isOrange){
	
		/*locate first index of first orange pixel*/
		orangeIndex = -1;
		for(ii = 0; ii < buffLength; ii++){
			if(boolim){
				orangeIndex = ii;
				break;
			}
		}//end first orange index
		
		/*quit if there is no more orange*/
		if(orangeIndex == -1){
			isOrange == false;
			break;
		}
		
		
	

		
		
	}//end while



	return barim;
}

Buffer2D<bool>* cutout(int x,int y,Buffer2D<bool>& img) {
	static Point2D<int>* plst=0;		// pixel list
	static int plsz=0;					// pixel list size
	static Buffer2D<bool>  cr;			//current region of interest
	Point2D<int> xmax(x, y);
	Point2D<int> xmin(x, y);
	Point2D<int> ymax(x, y);
	Point2D<int> ymin(x, y);
	const int w=img.width;
	//const int h=img.height;
	
	if (img.numElements()!=cr.numElements()) {
		plst = new Point2D<int>[img.numElements()];
		plsz = img.numElements();
		cr.resizeToMatch (img) ;
	}
	
	//fill cr with 0's
		// Free old data buffer (if one was allocated)
		if (cr.data != NULL) {
			delete[] cr.data;
		}

		// Allocate new data buffer (if width & height are non-zero)
		if ( (img.width != 0) && (img.height != 0)) {
			cr.data = (bool*)malloc(sizeof(bool)*width * height);
		} else {
			cr.data = NULL;
		}
		
	// lets cut!
	int cp=0;		//current point index
	int np=1;		//num points so far found, starts with 1
	
	plst[cp].x=x;	//read in initial point
	plst[cp].y=y;
	
	img.data[plst[cp].x+plst[cp].y*w]=0;		//clear initial point
	for(;cp<np;cp++){						//run till out of points
		//clear and record 4 neighbors
		{
			//cache base x and y (hopefully in registers)			 
				int cx=plst[cp].x;
				int cy=plst[cp].y;
			
			//white top
			{
				if(img.data[cx+(cy+1)*w]){
					img.data[cx+(cy+1)*w]=0;	//clear it
					plst[np].y=cy+1;			//add to found list
					plst[np].x=cx;
					np++;
				}
			}
			//white right
			{
				if(img.data[cx+1+(cy)*w]){		
					img.data[cx+1+(cy)*w]=0;	//clear it
					plst[np].y=cy;			//add to found list
					plst[np].x=cx+1;
					np++;
				}
			}
			//white bottom
			{
				if(img.data[cx+(cy-1)*w]){
					img.data[cx+(cy-1)*w]=0;	//clear it
					plst[np].y=cy-1;			//add to found list
					plst[np].x=cx;
					np++;
				}
			}
			//white left
			{
				if(img.data[cx-1+(cy)*w]){		
					img.data[cx-1+(cy)*w]=0;	//clear it
					plst[np].y=cy;			//add to found list
					plst[np].x=cx-1;
					np++;
				}
			}
		}
	}
	//pump out region to cr
	for(;cp<np;cp++){
		cr.data[plst[cp].x+plst[cp].y*w]=0;
	}
	return &cr;
}