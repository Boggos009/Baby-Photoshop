#include "Image_Class.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
using namespace std;

/*
    Name            ID      Section     What he did     common work

Bavly Boules    20250113                1,5,9,13        


Bemwa Bakhet    20250125                2,6,10,14
                                                        additional filters
                                                    +   GUI
Zeyad Sayed     20250247                3,7,11,15


Martin Rashad   20250513                4,8,12,16


*/


//some global variables
Image currentImg;
string currentFIle="";
Image backupImg;
Image originalImg;

//some definitions
bool rediriction(int op);
int showMenu();
void Exit();
void ChooseImg();
void Saving();
void successFilterImplementation();
int dy [9] = {-1 , -1 , -1 , 0 , 0 , 0 , 1 , 1 , 1};
int dx [9] = {-1 ,  0 ,  1 ,-1 , 0 , 1 ,-1 , 0 , 1};
//for smoothing 
int gaussianKernel[9] = {
    1,2,1,
    2,4,2,
    1,2,1
};
//sobel technique for edge detection
int sobelgxKernel[9] = {
    -1 , 0 , 1,
    -2 , 0 , 2,
    -1 , 0 , 1
};
int sobelgyKernel[9] = {
    -1 , -2 , -1,
    0 , 0 , 0,
    1 , 2 , 1
};

//Filters Functions

//Filter 1 
void GrayscaleConversion()
{

    for (int i = 0; i < currentImg.width; i++)
    {
        for ( int j = 0; j < currentImg.height; j++) 
        {
        
            int avg = 0;
            
            for (int k = 0; k < currentImg.channels; k++)
            {
                avg += currentImg(i, j, k);
			}
            
            avg /= 3;
            
            for (int k = 0; k < currentImg.channels; k++)
            {
				currentImg(i, j, k) = avg;
            }
        }
    }
    successFilterImplementation();
}


//Filter 2
void BlackandWhite()
{
    for(int i=0;i<currentImg.width;i++)
    {
        for(int j=0;j<currentImg.height;j++)
        {
            int avg = 0;
            for(int k=0;k<currentImg.channels;k++)
            
                avg+=currentImg(i,j,k);
            
            avg/=3;
            if(avg>=125)
                avg = 255;
            else
                avg = 0;
            
            for(int k=0;k<currentImg.channels;k++)
                currentImg(i,j,k) = avg;
        }
    }
    successFilterImplementation();
}


// Filter 3
void InvertImage()
{
    for(int i=0;i<currentImg.width;i++)
    {
        for(int j=0;j<currentImg.height;j++)
        {
            currentImg(i,j,0)=255-currentImg(i, j, 0);
            currentImg(i,j,1)=255-currentImg(i, j, 1);
            currentImg(i,j,2)=255-currentImg(i, j, 2);
        }
    }
    successFilterImplementation();
}


// Filter 4
void classicframe()
{
    int thickness = 0;
    if (currentImg.width <= currentImg.height)
    {
        thickness = currentImg.width * 0.03;
    }
    else
    {
        thickness = currentImg.height * 0.03;
    }

    if (thickness < 1)
    {
        thickness = 1;
    }

    cout << "choose the frame colour (0 for black, 1 for white): ";
    int colour;
    cin >> colour;

    Image classicframe(currentImg.width + 2 * thickness, currentImg.height + 2 * thickness);

    for (int i = 0; i < classicframe.width; i++)
    {
        for (int j = 0; j < classicframe.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (i < thickness || i >= classicframe.width - thickness || j < thickness || j >= classicframe.height - thickness)
                {
                    classicframe(i, j, k) = 255 * colour;
                }
                else
                {
                    classicframe(i, j, k) = currentImg(i - thickness, j - thickness, k);
                }
            }
        }
    }
    currentImg = classicframe;
}

void ornateframe()
{
    int thickness = 0, line = 0, blackframe = 0;
    if (currentImg.width <= currentImg.height)
    {
        thickness = currentImg.width * 0.03;
    }
    else
    {
        thickness = currentImg.height * 0.03;
    }

    if (thickness < 1)
    {
        thickness = 1;
    }

    blackframe = thickness * 0.33;

    Image ornateframe(currentImg.width + 2 * thickness, currentImg.height + 2 * thickness);

    for (int i = 0; i < ornateframe.width; i++)
    {
        for (int j = 0; j < ornateframe.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (i < thickness || i >= ornateframe.width - thickness || j < thickness || j >= ornateframe.height - thickness)
                {
                    ornateframe(i, j, k) = 255;
                }
                else
                {
                    ornateframe(i, j, k) = currentImg(i - thickness, j - thickness, k);
                }
                if (i > blackframe && i < blackframe * 2 || j > blackframe && j < blackframe * 2 || i > ornateframe.width - blackframe * 2 && i < ornateframe.width - blackframe || j > ornateframe.height - blackframe * 2 && j < ornateframe.height - blackframe)
                {
                    ornateframe(i, j, k) = 0;
                }
            }
        }
    }
    currentImg = ornateframe;
    
}

void AddingFrame()
{

    int frameType;
    cout << "Choose a frame type (0 for classic, 1 for ornate): ";
    cin >> frameType;

    if (frameType == 0) { classicframe(); }
    else if (frameType == 1) { ornateframe(); }
    else { cout << "Invalid frame type." << endl; return;}
    successFilterImplementation();
}



// Filter 5
void FlipVertical() {

    for (int i = 0; i < currentImg.height / 2; i++)
    {
        for (int j = 0; j < currentImg.width; j++)
        {
            for (int k = 0; k < currentImg.channels; k++)
            {
                unsigned char temp = currentImg(j, i, k);
                currentImg(j, i, k) = currentImg(j, currentImg.height - 1 - i, k);
                currentImg(j, currentImg.height - 1 - i, k) = temp;
            }
        }
    }
}

void FlipHorizontal() {

    for (int y = 0; y < currentImg.height; y++) {

        for (int x = 0; x < currentImg.width / 2; x++) {

            for (int k = 0; k < currentImg.channels; k++) {

                unsigned char temp = currentImg(x, y, k);
                currentImg(x, y, k) = currentImg(currentImg.width - 1 - x, y, k);
                currentImg(currentImg.width - 1 - x, y, k) = temp;
            
            }
        
        }
    
    }

}

void FlipImage()
{

    cout << "Choose flip direction (1 = Horizontal / 2 = Vertical)\n";
    int choice; cin >> choice;
    
    switch (choice)
    {
    case 1:
        FlipHorizontal();
        break;
    
    case 2:
        FlipVertical();
        break;
    
    default:
        cout << "Invalid choice\n";
        return;
    }
    successFilterImplementation();
}

// Filter 6
void ninetyRotation()
{
    Image img2(currentImg.height, currentImg.width);
    
    for(int y=0;y<currentImg.height;y++)
    {
        for(int x=0;x<currentImg.width;x++)
        {
            for(int k=0;k<currentImg.channels;k++)
            {
                img2(currentImg.height-1-y, x, k) = currentImg(x, y, k);
            }
        }
    }
    currentImg = img2;
}

void onehundredEightyRotation()
{
    Image img2(currentImg.width, currentImg.height);
    for(int y=0;y<currentImg.height;y++)
    {
        for(int x=0;x<currentImg.width;x++)
        {
            for(int k=0;k<currentImg.channels;k++)
            {
                img2(currentImg.width-1-x, currentImg.height-1-y, k) = currentImg(x, y, k);
            }
        }
    }
    currentImg = img2;
}

void twohundredseventyRotation()
{
    Image img2(currentImg.height, currentImg.width);
    
    for(int y=0;y<currentImg.height;y++)
    {
        for(int x=0;x<currentImg.width;x++)
        {
            for(int k=0;k<currentImg.channels;k++)
            {
                img2(y, currentImg.width-1-x, k) = currentImg(x, y, k);
            }
        }
    }
    currentImg = img2;
}
void RotateImage()
{
    cout<<"Choose the degree of rotating (90/180/270)\n";
    int degree;cin>>degree;
    switch (degree)
    {
    case 90:
        ninetyRotation();
        break;
    case 180:
        onehundredEightyRotation();
        break;
    case 270:
        twohundredseventyRotation();
        break;
    default:
        cout<<"Invalid choice\n";
        return;
        break;
    }
    successFilterImplementation();
}


// Filter 7

void applyBrightness(double factor){
<<<<<<< HEAD
    for(int i=0;i<currentImg.width;i++)
=======
     for(int i=0;i<currentImg.width;i++)
>>>>>>> fdc4f32b8651f252b80330dd7ff4edb53151b10b
    {
        for(int j=0;j<currentImg.height;j++)
        {
            for(int channel = 0; channel < currentImg.channels; channel++)
            {
                int value = (int)(currentImg(i, j, channel) * factor);
                if (value > 255) value = 255;
                currentImg(i, j, channel) = (unsigned char)value;
            }
        }
    }
}
void Dark(int percent)
{
    double factor = 1.0 - (percent / 100.0);
    applyBrightness(factor);
}

void Light(int percent)
{
    double factor = 1.0 + (percent / 100.0);
    applyBrightness(factor);

}

void DarkandLight(int choice, int percent)
{
    switch (choice)
    {
        case 0:
            Dark(percent);
            break;

        case 1:
            Light(percent);
            break;

        default:
            cout << "Invalid choice\n";
            return;
    }
}

void DarkandLight()
{
    cout << "Choose:\n";
    cout << "0. Dark\n";
    cout << "1. Light\n";

    int choice;
    cin >> choice;

    cout << "Enter percentage (0 - 100): ";

    int percent;
    cin >> percent;

    if (percent < 0 || percent > 100 || choice < 0 || choice > 1)
    {
        cout << "Invalid input\n";
        return;
    }

    DarkandLight(choice,percent);
    
    successFilterImplementation();

}


// Filter 8
void resizing(int newWidth , int newHeight )
{
    Image resizedImage1(newWidth, newHeight);
    for (int i = 0; i < newWidth; i++)
    {
        for (int j = 0; j < newHeight; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                int originalX = i * currentImg.width / newWidth;
                int originalY = j * currentImg.height / newHeight;
                resizedImage1(i, j, k) = currentImg(originalX, originalY, k);
            }
        }
    }
    currentImg = resizedImage1;
}

void Resize()
{

    cout << "enter the way of resizing the image (0 for writing the new width and height, 1 for writing the scale factor): ";
    int choice;
    cin >> choice;
    int width = currentImg.width , height = currentImg.height;
    if (choice == 0)
    {
        cout<<"Enter new width: ";cin>>width;cout<<"\n";
        cout<<"Enter new height: ";cin>>height;cout<<"\n";
    }
    else if (choice == 1)
    {
        double scale;cin>>scale;
        width*=scale;height*=scale;
    }
    else
    {
        cout << "Invalid choice." << endl;
    }
    resizing(width , height);
    successFilterImplementation();
}


// Filter 9

Image Opacity(Image const& first, Image const& second, double opacity , int newWidth, int newHeight) {

    Image result(newWidth, newHeight);

    for (int i = 0; i < newWidth; i++) {
        
        for (int j = 0; j < newHeight; j++) {
            
            for (int k = 0; k < first.channels; k++) {
        
                result(i, j, k) = static_cast<unsigned char>(
                    opacity * first(i, j, k) + (1 - opacity) * second(i, j, k) + 0.5
                );

            }

        }

    }

    return result;
}

Image ResizedImage(Image const& img, int newWidth , int newHeight ) {

    Image resizedImage1(newWidth, newHeight);
    for (int i = 0; i < newWidth; i++)
    {
        for (int j = 0; j < newHeight; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                int originalX = i * img.width / newWidth;
                int originalY = j * img.height / newHeight;
                resizedImage1(i, j, k) = img(originalX, originalY, k);
            }
        }
    }
    return resizedImage1;
}

Image Merge_biggest(Image const& first, Image const& second, double opacity) {

    if (first.width == second.width && first.height == second.height) {
        
        return Opacity(first, second, opacity, first.width, first.height);
    
    }

    else {
        
        int newWidth = max(first.width, second.width);
        int newHeight = max(first.height, second.height);

        Image resizedFirst = ResizedImage(first, newWidth, newHeight);
        Image resizedSecond = ResizedImage(second, newWidth, newHeight);

        return Opacity(resizedFirst, resizedSecond, opacity, newWidth, newHeight);

    }
    
}

Image Merge_smallest(Image const& first, Image const& second, double opacity) {

    if (first.width == second.width && first.height == second.height) {
        
        return Opacity(first, second, opacity, first.width, first.height);
    
    }
    
    else{
        
        int newWidth = min(first.width, second.width);
        int newHeight = min(first.height, second.height);

        return Opacity(first, second, opacity, newWidth, newHeight);
    
    }
    
}

void Merge() {
    
    Image secondImage;
    while (true) {

        cout << "\nChoose the image to merge with the current image:\n";
        string filename;
        cin >> filename;

        try
        {
            secondImage.loadNewImage(filename);
            break;
        }

        catch (const std::exception& x)
        {
            cout << "\nPlease enter a valid existing image filename.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    }

    int method;
    while (true) {
    
        cout << "Choose the merge method:\n"
         << "1. Resize both images to largest dimensions\n"
         << "2. Merge common area (smallest dimensions)\n"
         << "Choice: ";

        if (cin >> method && (method == 1 || method == 2)) break;
    

        cout << "\n\nInvalid choice! Please enter 1 or 2.\n\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    }

    double opacity;
    while (true) {
        
        cout << "Choose the opacity for the first image (0.0 to 1.0): ";
        if (cin >> opacity && opacity >= 0.0 && opacity <= 1.0) {
            break;
        }
        cout << "Invalid input! Please enter a decimal number between 0.0 and 1.0.\n\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }

    switch (method) {
        case 1:
            currentImg = Merge_biggest(currentImg, secondImage, opacity);
            break;
        case 2:
            currentImg = Merge_smallest(currentImg, secondImage, opacity);
            break;
        default:
            cout << "Invalid choice." << endl;
            return;
    }

    successFilterImplementation();
}


// Filter 10

//need to smooth the image first to reduce the noise --> guassian blur
Image GuassianBlur()
{
    Image img2(currentImg.width , currentImg.height);
    for(int y= 0;y<currentImg.height;y++)
    {
        for(int x=0;x<currentImg.width;x++)
        {
            for(int k=0;k<currentImg.channels;k++)
            {
                int avg = 0;
                for(int d=0;d<9;d++)
                {
                    int nx = x+dx[d] , ny = y+dy[d];
                    if(nx<0||ny<0||nx>=currentImg.width||ny>=currentImg.height)
                        continue;
                    avg+=(currentImg(nx,ny,k)*gaussianKernel[d]);
                }
                avg/=16;
                avg = max(0, avg);
                avg = min(255 , avg);
                img2(x,y,k) = avg;
            }
        }
    }
    return img2;
}



void DetectEdges()
{
    cout<<"choose whether you want to smooth it first or without smoothing (0 for no / 1 for yes)\n";
    int op;cin>>op;
    if(op)
        currentImg = GuassianBlur();
    Image img2(currentImg.width , currentImg.height);

    for(int y= 0;y<currentImg.height;y++)
    {
        for(int x=0;x<currentImg.width;x++)
        {
            int avg = 0;
            int gx = 0 , gy= 0;
            for(int k=0;k<currentImg.channels;k++)
            {
                for(int d=0;d<9;d++)
                {
                    int nx = x+dx[d] , ny = y+dy[d];
                    if(nx<0||ny<0||nx>=currentImg.width||ny>=currentImg.height)
                        continue;
                    gx+=(currentImg(nx,ny,k))*sobelgxKernel[d];
                    gy+=(currentImg(nx,ny,k))*sobelgyKernel[d];
                }
                int magnitude = sqrt(gx*gx+gy*gy);
                avg+=magnitude;
            }
            avg/=currentImg.channels;
            
            if(avg>=100)
            {
                img2(x,y,0) = 0;
                img2(x,y,1) = 0;
                img2(x,y,2) = 0;
            }
            
            else
            {
                img2(x,y,0) = 255;
                img2(x,y,1) = 255;
                img2(x,y,2) = 255;
            }
            
        }
    }
    currentImg = img2;
    successFilterImplementation();
}


// Filter 11
void Crop()
{
    int startX, startY, newWidth, newHeight;

    cout << "Enter starting point (x,y): ";
    cin >> startX >> startY;

    cout << "Enter crop dimensions (Width,Height): ";
    cin >> newWidth >> newHeight;

    if (startX < 0 || startY < 0 || newWidth <= 0 || newHeight <= 0 ||
        (startX + newWidth) > currentImg.width ||
        (startY + newHeight) > currentImg.height)
    {
        cerr << "Invalid crop parameters\n";
        return;
    }

    Image croppedImage(newWidth, newHeight);
    for (int y = 0; y < newHeight; ++y)
    {
        for (int x = 0; x < newWidth; ++x)
        {
            for (int channel = 0; channel < currentImg.channels; ++channel)
            {
                croppedImage(x, y, channel) =
                    currentImg(startX + x, startY + y, channel);
            }
        }
    }

    currentImg = croppedImage;
    successFilterImplementation();
}


// Filter 12
void Blur()
{
    cout << "please choose the level of bluring, from 1 to 5\n";
    int r;
    cin >> r;

    int R = currentImg.width > currentImg.height ? r * currentImg.height * 0.01 : r * currentImg.width * 0.01;

    if (r < 1 || r > 5)
    {
        cout << "Invalid input. Please enter a number between 1 and 5." << endl;
        return;
    }
    else
    {
        Image tempImg(currentImg.width, currentImg.height);
        Image blurredImg(currentImg.width, currentImg.height);

        for (int i = 0; i < currentImg.width; i++)
        {
            for (int j = 0; j < currentImg.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    int sum = 0;
                    int count = 0;

                    for (int x = -R; x <= R; x++)
                    {
                        int newX = i + x;
                        if (newX >= 0 && newX < currentImg.width)
                        {
                            sum += currentImg(newX, j, k);
                            count++;
                        }
                    }

                    tempImg(i, j, k) = sum / count;
                }
            }
        }

        for (int i = 0; i < currentImg.width; i++)
        {
            for (int j = 0; j < currentImg.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    int sum = 0;
                    int count = 0;

                    for (int y = -R; y <= R; y++)
                    {
                        int newY = j + y;
                        if (newY >= 0 && newY < currentImg.height)
                        {
                            sum += tempImg(i, newY, k);
                            count++;
                        }
                    }

                    blurredImg(i, j, k) = sum / count;
                }
            }
        }
        currentImg = blurredImg;
    }
    successFilterImplementation();
}


// Filter 13
void Sunlight() {

    for (int i = 0; i < currentImg.width; i++) {

        for (int j = 0; j < currentImg.height; j++) {

            int r = currentImg(i, j, 0);
            int g = currentImg(i, j, 1);
            int b = currentImg(i, j, 2);

            r = (int)(r * 1.15);
            g = (int)(g * 1.08);
            b = (int)(b * 0.85);

            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            if (r < 0) r = 0;
            if (g < 0) g = 0;
            if (b < 0) b = 0;

            currentImg(i, j, 0) = (unsigned char)r;
            currentImg(i, j, 1) = (unsigned char)g;
            currentImg(i, j, 2) = (unsigned char)b;

        }

    }

    successFilterImplementation();

}

// Filter 14
void OldTv()
{
    for(int y=0;y<currentImg.height;y++)
    {
        for(int x=0;x<currentImg.width;x++)
        {
            for(int k=0;k<currentImg.channels;k++)
            {
                if(y%2==0)
                    currentImg(x,y,k) *=0.3;
            }
        }
    }
    successFilterImplementation();
}


// Filter 15
void PurpleWano()
{
    for(int i=0;i<currentImg.width;i++)
    {
        for(int j=0;j<currentImg.height;j++)
        {
            int r = currentImg(i,j, 0);
            int g = currentImg(i,j, 1);
            int b = currentImg(i,j, 2);

            r = (int)(r * 1.15);
            g = (int)(g * 0.72); 
            b = (int)(b * 1.18); 

            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;

            currentImg(i, j, 0) = (unsigned char)r;
            currentImg(i, j, 1) = (unsigned char)g;
            currentImg(i, j, 2) = (unsigned char)b;

        }
    }
    successFilterImplementation();
}


// Filter 16
void Infrared()
{
    Image newImage(currentImg.width, currentImg.height);

    for (int i = 0; i < currentImg.width; i++)
    {
        for (int j = 0; j < currentImg.height; j++)
        {
            int r = currentImg(i, j, 0);
            int g = currentImg(i, j, 1);
            int b = currentImg(i, j, 2);

            int red = (r + g + b) / 3;

            newImage(i, j, 0) = 255 - red;
            newImage(i, j, 1) = 0;
            newImage(i, j, 2) = 0;
        }
    }
    currentImg = newImage;
    successFilterImplementation();
}

// Filter 17
void Skew()
{
    cout<<"Enter the degree of skewing: ";
    int theta;cin>>theta;
    double angle = theta * M_PI / 180;

    int offset = ceil((currentImg.height - 1) * abs(tan(angle)));
    int newWidth = currentImg.width + offset;

    Image output(newWidth, currentImg.height);

    for(int y=0;y<output.height;y++)
    {
        for(int x=0;x<output.width;x++)
        {
            for(int k=0;k<3;k++)
            {
                output(x,y,k) = 255;
            }
        }
    }

    for(int y=0;y<output.height;y++)
    {
        for(int x=0;x<output.width;x++)
        {
            int oldX = round(x - offset + y * abs(tan(angle)));

            if(oldX<0 || oldX>=currentImg.width)
                continue;

            for(int k=0;k<3;k++)
            {
                output(x,y,k) = currentImg(oldX,y,k);
            }
        }
    }

    currentImg = output;
    successFilterImplementation();
}

//Filter 18
void OilPainting()
{
    int radius = 5;
    int levels = 20;

    Image output(currentImg.width, currentImg.height);

    for(int y=0;y<currentImg.height;y++)
    {
        for(int x=0;x<currentImg.width;x++)
        {
            int sumR[20] = {};
            int sumG[20] = {};
            int sumB[20] = {};
            int count[20] = {};

            for(int ny=y-radius;ny<=y+radius;ny++)
            {
                for(int nx=x-radius;nx<=x+radius;nx++)
                {
                    if(nx<0 || nx>=currentImg.width ||
                    ny<0 || ny>=currentImg.height)
                        continue;

                    int r = currentImg(nx,ny,0);
                    int g = currentImg(nx,ny,1);
                    int b = currentImg(nx,ny,2);

                    int degree = (r+g+b)/3;
                    int bin = degree * levels / 256;

                    count[bin]++;
                    sumR[bin] += r;
                    sumG[bin] += g;
                    sumB[bin] += b;
                }
            }

            int mx = 0;

            for(int i=1;i<levels;i++)
            {
                if(count[i] > count[mx])
                    mx = i;
            }

            output(x,y,0) = sumR[mx] / count[mx];
            output(x,y,1) = sumG[mx] / count[mx];
            output(x,y,2) = sumB[mx] / count[mx];
        }
    }

    currentImg = output;
    successFilterImplementation();
}



int main()
{
    ChooseImg();
    int op;
    while ((op = showMenu())<=18)
    {
        rediriction(op);
        system("pause");
    }
    Exit();
    return 0;
}


//Auxiliary Funtions
void successFilterImplementation()
{
    cout<<"Filter has been implemented successfully.\n";
    cout<<"Press 1 if u want to save your changes now\n";
    bool ok;cin>>ok;
    if(ok)
        Saving();
}

void ChooseImg()
{
    cout<<"Choose an image please with any of the following extensions: (.JPG, .JPEG, .BMP, .PNG, .TGA)\n";
    string name;cin>>name;
    currentImg.loadNewImage(name);
    currentFIle = name;
    originalImg = currentImg;
}

//saving options (Edit the current image, or create a new one with the same edits.)
void Saving()
{
    cout<<"Would you like to edit the current image or create a new one with these edits?(0 for edit /1 for create another one)\n";
    int op;
    cin>>op;
    if(op)
    {
        cout<<"Please enter the new image's name: ";
        string name;cin>>name;
        int dot = name.find_last_of('.');
        string extension = (dot==string::npos? "" : name.substr(dot , name.size()-dot));
        if(dot==string::npos||(extension!="jpeg"&&extension!="jpg"&&extension!="bmp"&&extension!="png"&&extension!="tga"))
        {
            if(dot!=string::npos)name = name.substr(0 , dot)+".jpg";
            else name+=".jpg";
        }
        currentImg.saveImage(name);
    }
    else
        currentImg.saveImage(currentFIle);
    cout<<"Image saved successfully\n";
}

void Undo()
{
    if(backupImg.imageData!=nullptr)
    {
        currentImg = backupImg;
        cout<<"Last filter removed successfully.\n";
    }
    else
        cout<<"Invalid operation\n";
}


bool rediriction(int op)
{
    switch (op)
    {
    case -3:
        system("cls");
        cout<<"Successfull operation.\n";
        currentImg = originalImg;
        backupImg.imageData = nullptr;
        break;
    case -2:
        system("cls");
        Saving();
        break;
    case -1:
        system("cls");
        Undo();
        break;
    case 0:
        system("cls");
        ChooseImg();
        break;
    case 1:
        backupImg = currentImg;
        system("cls");
        GrayscaleConversion();
        break;
    
    case 2:
        backupImg = currentImg;
        system("cls");
        BlackandWhite();
        break;
    
    case 3:
        backupImg = currentImg;
        system("cls");
        InvertImage();
        break;
    
    case 4:
        backupImg = currentImg;
        system("cls");
        AddingFrame();
        break;
    
    case 5:
        backupImg = currentImg;
        system("cls");
        FlipImage();
        break;
    
    case 6:
        backupImg = currentImg;
        system("cls");
        RotateImage();
        break;
    
    case 7:
        backupImg = currentImg;
        system("cls");
        DarkandLight();
        break;
    
    case 8:
        backupImg = currentImg;
        system("cls");
        Resize();
        break;
    
    case 9:
        backupImg = currentImg;
        system("cls");
        Merge();
        break;
    
    case 10:
        backupImg = currentImg;
        system("cls");
        DetectEdges();
        break;
    
    case 11:
        backupImg = currentImg;
        system("cls");
        Crop();
        break;
    
    case 12:
        backupImg = currentImg;
        system("cls");
        Blur();
        break;
    
    case 13:
        backupImg = currentImg;
        system("cls");
        Sunlight();
        break;
    
    case 14:
        backupImg = currentImg;
        system("cls");
        OldTv();
        break;
    
    case 15:
        backupImg = currentImg;
        system("cls");
        PurpleWano();
        break;
    
    case 16:
        backupImg = currentImg;
        system("cls");
        Infrared();
        break;
    
    case 17:
        backupImg = currentImg;
        system("cls");
        Skew();
        break;
    
    case 18:
        backupImg = currentImg;
        system("cls");
        OilPainting();
        break;
    
    default:
        return false;
        break;
    }
    return true;
}

int showMenu()
{
    system("cls");
    cout<<"\t\t----------------------------\n";
    cout<<"\t\tWelcome to Baby Photoshop v0";
    if(currentImg.imageData !=nullptr)
        cout<<"\t\tCurrent Image: "<<currentFIle;
    cout<<"\n\t\t----------------------------\n";
    cout<<"\t\tChoose one of the following options:\n";
    cout<<"\t\t----------------------------\n";
    cout<<"Back to the original picture (press -3)\n";
    cout<<"Saving the current edits (press -2)\n";
    cout<<"Undo the last filter (press -1 , valid for 1 time only)\n";
    cout<<"Load new image (press 0)\n";
    cout<<"Filter 1: Grayscale Conversion\n";
    cout<<"Filter 2: Black and White\n";
    cout<<"Filter 3: Invert Image\n";
    cout<<"Filter 4: Adding a Frame to the Picture\n";
    cout<<"Filter 5: Flip Image\n";
    cout<<"Filter 6: Rotate Image\n";
    cout<<"Filter 7: Darken and Lighten Image\n";
    cout<<"Filter 8: Resizing Image\n";
    cout<<"Filter 9: Merge Images\n";
    cout<<"Filter 10: Detect Image Edges\n";
    cout<<"Filter 11: Crop Image\n";
    cout<<"Filter 12: Blur Image\n";
    cout<<"Filter 13: Wano natural sunlight.\n";
    cout<<"Filter 14: Wano old TV\n";
    cout<<"Filter 15: Purple Wano\n";
    cout<<"Filter 16: Infrared photography\n";
    cout<<"Filter 17: Skew image\n";
    cout<<"Filter 18: Oil painting\n";
    cout<<"Exit (any number >18)\n";
    cout<<"\t\t----------------------------\n";

    int op = 0;cin>>op;
    return op;
}

void Exit()
{
    cout<<"Save your edits before exit? (1 for yes / 0 for no)\n";
    int op;cin>>op;
    if(op)
        Saving();
    system("cls");
    cout<<"\t\t---------------------------------\n";
    cout << "\t\t" << setw(30) << "See you in the next time\n";
    cout<<"\t\t---------------------------------\n";
}
