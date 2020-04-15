
#include "stats.h"
#include <iostream>
using namespace std;

stats::stats(PNG & im){
 
   int height = im.height();
   int width = im.width();

   sumRed.resize(height, vector<long>(width));
   sumGreen.resize(height, vector<long>(width));
   sumBlue.resize(height, vector<long>(width));
   sumsqRed.resize(height, vector<long>(width));
   sumsqGreen.resize(height, vector<long>(width));
   sumsqBlue.resize(height, vector<long>(width));

   for (int i = 0; i < height; i++){
      for (int j = 0; j < width; j++){
         RGBAPixel *pixel = im.getPixel(j, i);
         if(i == 0 && j == 0){
           sumRed[i][j] = pixel->r;
	        sumGreen[i][j] = pixel->g;
	        sumBlue[i][j] = pixel->b;
	        sumsqRed[i][j] = pixel->r * pixel->r;
	        sumsqGreen[i][j] = pixel->g * pixel->g;
	        sumsqBlue[i][j] = pixel->b * pixel->b;
         }
         else if(i == 0 && j > 0){ 
            sumRed[i][j] = pixel->r + sumRed[i][j - 1];
	        sumGreen[i][j] = pixel->g + sumGreen[i][j - 1];
	        sumBlue[i][j] = pixel->b + sumBlue[i][j - 1];
	        sumsqRed[i][j] = pixel->r * pixel->r + sumsqRed[i][j - 1];
	        sumsqGreen[i][j] = pixel->g * pixel->g + sumsqGreen[i][j - 1];
	        sumsqBlue[i][j] = pixel->b * pixel->b + sumsqBlue[i][j - 1];
         }else if(i > 0 && j == 0){
            sumRed[i][j] = pixel->r + sumRed[i - 1][j];
	        sumGreen[i][j] = pixel->g + sumGreen[i - 1][j];
	        sumBlue[i][j] = pixel->b + sumBlue[i - 1][j];
	        sumsqRed[i][j] = pixel->r * pixel->r + sumsqRed[i - 1][j];
	        sumsqGreen[i][j] = pixel->g * pixel->g + sumsqGreen[i - 1][j];
	        sumsqBlue[i][j] = pixel->b * pixel->b + sumsqBlue[i - 1][j];
         }else{
            sumRed[i][j] = pixel->r + sumRed[i - 1][j] + sumRed[i][j - 1] - sumRed[i - 1][j - 1];
	        sumGreen[i][j] = pixel->g + sumGreen[i - 1][j] + sumGreen[i][j - 1] - sumGreen[i - 1][j - 1];
	        sumBlue[i][j] = pixel->b + sumBlue[i - 1][j] + sumBlue[i][j - 1] - sumBlue[i - 1][j - 1];
	        sumsqRed[i][j] = pixel->r * pixel->r + sumsqRed[i - 1][j] + sumsqRed[i][j - 1] - sumsqRed[i - 1][j - 1];
	        sumsqGreen[i][j] = pixel->g * pixel->g + sumsqGreen[i - 1][j] + sumsqGreen[i][j - 1] - sumsqGreen[i - 1][j - 1];
	        sumsqBlue[i][j] = pixel->b * pixel->b + sumsqBlue[i - 1][j] + sumsqBlue[i][j - 1] - sumsqBlue[i - 1][j - 1];
         }

      }
   }

}

long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){

long ret = 0;

if(channel == 'r'){

     if(ul.first == 0 && ul.second == 0){
        ret = sumRed[lr.second][lr.first];  
     }else if(ul.first == 0 && ul.second > 0){
        ret = sumRed[lr.second][lr.first] - sumRed[ul.second - 1][lr.first];
     }else if(ul.first > 0 && ul.second == 0){
        ret = sumRed[lr.second][lr.first] - sumRed[lr.second][ul.first - 1];
     } else{
        ret = sumRed[lr.second][lr.first] - sumRed[lr.second][ul.first - 1] - sumRed[ul.second - 1][lr.first] + sumRed[ul.second - 1][ul.first - 1];
     }

}else if (channel == 'g'){

     if(ul.first == 0 && ul.second == 0){
        ret = sumGreen[lr.second][lr.first];  
     }else if(ul.first == 0 && ul.second > 0){
        ret = sumGreen[lr.second][lr.first] - sumGreen[ul.second - 1][lr.first];
     }else if(ul.first > 0 && ul.second == 0){
        ret = sumGreen[lr.second][lr.first] - sumGreen[lr.second][ul.first - 1];
     } else{
        ret = sumGreen[lr.second][lr.first] - sumGreen[lr.second][ul.first - 1] - sumGreen[ul.second - 1][lr.first] + sumGreen[ul.second - 1][ul.first - 1];
     }

}else if (channel == 'b'){

    if(ul.first == 0 && ul.second == 0){
        ret = sumBlue[lr.second][lr.first];  
     }else if(ul.first == 0 && ul.second > 0){
        ret = sumBlue[lr.second][lr.first] - sumBlue[ul.second - 1][lr.first];
     }else if(ul.first > 0 && ul.second == 0){
        ret = sumBlue[lr.second][lr.first] - sumBlue[lr.second][ul.first - 1];
     } else{
        ret = sumBlue[lr.second][lr.first] - sumBlue[lr.second][ul.first - 1] - sumBlue[ul.second - 1][lr.first] + sumBlue[ul.second - 1][ul.first - 1];
     }

}
   return ret;
}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){

long ret = 0;
if(channel == 'r'){

     if(ul.first == 0 && ul.second == 0){
        ret = sumsqRed[lr.second][lr.first];  
     }else if(ul.first == 0 && ul.second > 0){
        ret = sumsqRed[lr.second][lr.first] - sumsqRed[ul.second - 1][lr.first];
     }else if(ul.first > 0 && ul.second == 0){
        ret = sumsqRed[lr.second][lr.first] - sumsqRed[lr.second][ul.first - 1];
     } else{
        ret = sumsqRed[lr.second][lr.first] - sumsqRed[lr.second][ul.first - 1] - sumsqRed[ul.second - 1][lr.first] + sumsqRed[ul.second - 1][ul.first - 1];
     }

}else if (channel == 'g'){

     if(ul.first == 0 && ul.second == 0){
        ret = sumsqGreen[lr.second][lr.first];  
     }else if(ul.first == 0 && ul.second > 0){
        ret = sumsqGreen[lr.second][lr.first] - sumsqGreen[ul.second - 1][lr.first];
     }else if(ul.first > 0 && ul.second == 0){
        ret = sumsqGreen[lr.second][lr.first] - sumsqGreen[lr.second][ul.first - 1];
     } else{
        ret = sumsqGreen[lr.second][lr.first] - sumsqGreen[lr.second][ul.first - 1] - sumsqGreen[ul.second - 1][lr.first] + sumsqGreen[ul.second - 1][ul.first - 1];
     }

}else if (channel == 'b'){

    if(ul.first == 0 && ul.second == 0){
        ret = sumsqBlue[lr.second][lr.first];  
     }else if(ul.first == 0 && ul.second > 0){
        ret = sumsqBlue[lr.second][lr.first] - sumsqBlue[ul.second - 1][lr.first];
     }else if(ul.first > 0 && ul.second == 0){
        ret = sumsqBlue[lr.second][lr.first] - sumsqBlue[lr.second][ul.first - 1];
     } else{
        ret = sumsqBlue[lr.second][lr.first] - sumsqBlue[lr.second][ul.first - 1] - sumsqBlue[ul.second - 1][lr.first] + sumsqBlue[ul.second - 1][ul.first - 1];
     }

}
   return ret;
}


long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

   return ((lr.second - ul.second + 1) * (lr.first - ul.first + 1));

}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
long stats::getScore(pair<int,int> ul, pair<int,int> lr){

   long redTotal = getSum('r', ul, lr);
   long greenTotal = getSum('g', ul, lr);
   long blueTotal = getSum('b', ul, lr);

   long redsqTotal = getSumSq('r', ul, lr);
   long greensqTotal = getSumSq('g', ul, lr);
   long bluesqTotal = getSumSq('b', ul, lr);

   long area = rectArea(ul, lr);
   
   long redSD = redsqTotal - (redTotal * redTotal / area);
   long greenSD = greensqTotal - (greenTotal * greenTotal / area);
   long blueSD = bluesqTotal - (blueTotal * blueTotal / area);

   return (redSD + greenSD + blueSD);

}
		
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

   long area = rectArea(ul,lr);
   long redTotal = getSum('r', ul, lr);
   long greenTotal = getSum('g', ul, lr);
   long blueTotal = getSum('b', ul, lr);

   RGBAPixel *ret = new RGBAPixel(redTotal/area, greenTotal/area, blueTotal/area);
   return *ret;
}
