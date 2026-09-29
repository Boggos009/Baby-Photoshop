#include "Image_Class.h"
#include <iomanip>

/*
    Name            ID      Section     What he did

Bavly Boules    20250113                1,5


Bemwa Bakhet    20250125                2,6,10


Zeyad Sayed     20250247                3,15


Martin Rashad   20250513                4,8


*/

using namespace std;

//some global variables
Image currentImg;
string currentFIle="";
Image backupImg;

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
void DarkandLight()
{
    cout<<"Will be Implemented Soon....\n";
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
void Merge()
{
    cout<<"Will be Implemented Soon....\n";
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
    cout<<"Will be Implemented Soon....\n";
}


// Filter 12
void Blur()
{
    cout<<"Will be Implemented Soon....\n";
}


// Filter 13
void Sunlight()
{
    cout<<"Will be Implemented Soon....\n";
}

// Filter 14
void OldTv()
{
    cout<<"Will be Implemented Soon....\n";
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
    cout<<"Will be Implemented Soon....\n";
}




int main()
{
    ChooseImg();
    int op;
    while ((op = showMenu())<=16)
    {
        rediriction(op);
        system("pause");
    }
    Exit();
}


//Auxiliary Funtions
void successFilterImplementation()
{
    cout<<"Filter has been implemented successfully.\n";
}

void ChooseImg()
{
    cout<<"Choose an image please with any of the following extensions: (.JPG, .JPEG, .BMP, .PNG, .TGA)\n";
    string name;cin>>name;
    currentImg.loadNewImage(name);
    currentFIle = name;
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
    cout<<"Exit (any number >16)\n";
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