#include "Image_Class.h"

/*
    Name            ID      Section     What he did

Bavly Boules    20250113                1,4


Bemwa Bakhet    20250125                2,6


Zeyad Sayed     20250247                3,7


Martin Rashad   20250513                4,8


*/

using namespace std;

Image currentImg;
string currentFIle="";

//some definitions
bool rediriction(int op);
int showMenu();
void Exit();


//Filters Functions

//Filter 1 
void GrayscaleConversion()
{
    cout<<"Will be Implemented Soon....\n";
}


//Filter 2
void BlackandWhite()
{
    cout<<"Will be Implemented Soon....\n";
}


// Filter 3
void InvertImage()
{
    cout<<"Will be Implemented Soon....\n";
}


// Filter 4
void AddingFrame()
{
    cout<<"Will be Implemented Soon....\n";
}


// Filter 5
void FlipImage()
{
    cout<<"Will be Implemented Soon....\n";
}


// Filter 6
void RotateImage()
{
    cout<<"Will be Implemented Soon....\n";
}


// Filter 7
void DarkandLight()
{
    cout<<"Will be Implemented Soon....\n";
}


// Filter 8
void Resize()
{
    cout<<"Will be Implemented Soon....\n";
}


// Filter 9
void Merge()
{
    cout<<"Will be Implemented Soon....\n";
}


// Filter 10
void DetectEdges()
{
    cout<<"Will be Implemented Soon....\n";
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
    cout<<"Will be Implemented Soon....\n";
}


// Filter 16
void Infrared()
{
    cout<<"Will be Implemented Soon....\n";
}




int main()
{
    currentImg = Image(200,200);
    currentImg.saveImage("ez.jpg");
    currentFIle = "ez.jpg";
    int op;
    while ((op = showMenu())<=16)
    {
        rediriction(op);
        system("pause");
    }
    Exit();
}


//Auxiliary Funtions
bool rediriction(int op)
{
    switch (op)
    {
    case 1:
        system("cls");
        GrayscaleConversion();
        break;
    
    case 2:
        system("cls");
        BlackandWhite();
        break;
    
    case 3:
        system("cls");
        InvertImage();
        break;
    
    case 4:
        system("cls");
        AddingFrame();
        break;
    
    case 5:
        system("cls");
        FlipImage();
        break;
    
    case 6:
        system("cls");
        RotateImage();
        break;
    
    case 7:
        system("cls");
        DarkandLight();
        break;
    
    case 8:
        system("cls");
        Resize();
        break;
    
    case 9:
        system("cls");
        Merge();
        break;
    
    case 10:
        system("cls");
        DetectEdges();
        break;
    
    case 11:
        system("cls");
        Crop();
        break;
    
    case 12:
        system("cls");
        Blur();
        break;
    
    case 13:
        system("cls");
        Sunlight();
        break;
    
    case 14:
        system("cls");
        OldTv();
        break;
    
    case 15:
        system("cls");
        PurpleWano();
        break;
    
    case 16:
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
    system("cls");
    cout<<"\t\t---------------------------------\n";
    cout<<"\t\tSee you in the next time\n";
    cout<<"\t\t---------------------------------\n";
}

