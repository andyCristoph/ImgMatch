#include <cstdlib>
#include <cmath>

#include "ModScale.h"
#include "Logger.h"


const float ModScale::MaxDistance = 441.673f;  // Euclidean distance between black and white = sqrt(3*255^2)


ModScale::~ModScale()
{
    LOG("Deleting cache!");
    for ( std::map<std::string, Image*>::iterator i = mScaledImagesCache.begin();
          i != mScaledImagesCache.end(); ++i )
    {
        delete i->second;
    }
}


float ModScale::Compare( const std::string& img1path, const std::string& img2path )
{
    Image *scaledImg1, *scaledImg2;

    std::map<std::string, Image*>::iterator i = mScaledImagesCache.find(img1path);
    if ( i != mScaledImagesCache.end() )
    {
//        LOG("Image " << i->first << " found in cache!");
        scaledImg1 = i->second;
    }
    else
    {
//        LOG("Image " << img1path << " NOT found in cache!");
        scaledImg1 = Image::ConstuctImage(img1path, Image::PIXEL_RGB24);  // Check for errors when opening the images!
//        int ScaledHeight = scaledImg1->GetHeight() * ScaledWidth / scaledImg1->GetWidth();
        scaledImg1->Scale( ScaledWidth, ScaledHeight, Image::ASPR_IGNORE );  // Was Image::ASPR_KEEP
        mScaledImagesCache[img1path] = scaledImg1;
    }

    i = mScaledImagesCache.find(img2path);
    if ( i != mScaledImagesCache.end() )
    {
//        LOG("Image " << i->first << " found in cache!");
        scaledImg2 = i->second;
    }
    else
    {
//        LOG("Image " << img2path << " NOT found in cache!");
        scaledImg2 = Image::ConstuctImage(img2path, Image::PIXEL_RGB24);
        scaledImg2->Scale( ScaledWidth, ScaledHeight, Image::ASPR_IGNORE );
        mScaledImagesCache[img2path] = scaledImg2;
    }

    float distance=0.0f, r, g, b;
    PixelRGB pixel1, pixel2;

    for( int x=0; x<ScaledWidth; x++ )
    {
        for( int y=0; y<ScaledHeight; y++ )
        {
            pixel1 = scaledImg1->GetPixelRGB(x, y);
            pixel2 = scaledImg2->GetPixelRGB(x, y);

            r = (float)pixel1.R - (float)pixel2.R;
            g = (float)pixel1.G - (float)pixel2.G,
            b = (float)pixel1.B - (float)pixel2.B;

            distance += ::sqrt(r*r + g*g + b*b);
        }
    }

    distance /= ScaledWidth*ScaledHeight;  // The average
//    LOG("img1=" << img1path << ", img2=" << img2path << ", distance=" << distance);

    return (1 - distance / MaxDistance);  // %
}
