#include "DetMatrix.h"


int OpenWindows;
BBitmap *res_pic[8];
IntegerInput *masterinput;
extern bool oneisrunning;
extern bool RefOnStart;
int GLOBAL_MATRIXSIZE = 100;
int GLOBAL_ACCURACY = 4;

alQuickApp *prog;
alHandler *anyH;

DetMatWin::DetMatWin():alQAWin(anyH,500,400,"DetMatrix")
{

BMenu *menu;

menu = AddMenuToFrame("Matrix");
AddItemToMenu("New", new BMessage(CONST_NewMatrix),menu);
AddItemToMenu("Clear", new BMessage(CONST_Clear),menu);
AddItemToMenu("Open", new BMessage(CONST_OpenMatrixFiles),menu);
AddItemToMenu("Save", NULL,menu);
AddItemToMenu("Save As", new BMessage(CONST_SaveMatrixToFile),menu);
AddItemToMenu("Mirror along major axis",new BMessage(CONST_MirrorMajor),menu);
AddItemToMenu("Mirror along minor axis", new BMessage(CONST_MirrorMinor),menu);
AddItemToMenu("Result of Matrix",new BMessage( CONST_EXEC ) ,menu);
AddItemToMenu("Quit",new BMessage(CONST_Qapp),menu);


menu = AddMenuToFrame("Precision");
AddItemToMenu("1 digit", new BMessage(CONST_A1),menu);
AddItemToMenu("2 digits", new BMessage(CONST_A2),menu);
AddItemToMenu("3 digits", new BMessage(CONST_A3),menu);
AddItemToMenu("4 digits", new BMessage(CONST_A4),menu);
AddItemToMenu("5 digits", new BMessage(CONST_A5),menu);
AddItemToMenu("6 digits", new BMessage(CONST_A6),menu);
AddItemToMenu("7 digits", new BMessage(CONST_A7),menu);
AddItemToMenu("8 digits", new BMessage(CONST_A8),menu);


ShowMenu();

MatrixCreated = false;

alHPictureBar TopCommandBar(10,30,this);
TCBb1 = TopCommandBar.AddButton(res_pic[0],res_pic[0] ,new BMessage(CONST_Qapp2));
TCBb2 = TopCommandBar.AddButton(res_pic[1],res_pic[1], new BMessage(CONST_MirrorFunc));
TCBb3 = TopCommandBar.AddButton(res_pic[2],res_pic[2], new BMessage(CONST_ResizeMatrix));
TCBb4 = TopCommandBar.AddButton(res_pic[3],res_pic[3], new BMessage(CONST_OpenMatrixFiles));
TCBb5 = TopCommandBar.AddButton(res_pic[4],res_pic[4], new BMessage(CONST_SaveMatrixToFile));
TCBb6 = TopCommandBar.AddButton(res_pic[5],res_pic[5], new BMessage(CONST_EXEC ));
TCBs7 = TopCommandBar.AddSwitch(res_pic[6],res_pic[7], new BMessage(CONST_MOD),  new BMessage(CONST_MOD));

SetFlags(B_PULSE_NEEDED);
mxCon = new MatrixControl(3,GLOBAL_MATRIXSIZE);
AddChild(mxCon);
ScrollControl = new alScrollControl(mxCon, true,true );


matrixcontrol = new BSlider (BRect(350,30,500,60),"non","Field size", new BMessage( CONST_FieldResize) , 30, 100, B_TRIANGLE_THUMB);
AddChild(matrixcontrol);
}

DetMatWin::~DetMatWin()
{
matrixcontrol->RemoveSelf();
delete matrixcontrol;

TCBb1->RemoveSelf();
delete TCBb1;

TCBb2->RemoveSelf();
delete TCBb2;

TCBb3->RemoveSelf();
delete TCBb3;

TCBb4->RemoveSelf();
delete TCBb4;

TCBb5->RemoveSelf();
delete TCBb5;

TCBb6->RemoveSelf();
delete TCBb6;

TCBs7->RemoveSelf();
delete TCBs7;

delete ScrollControl;
}



IntegerInput::IntegerInput( DetMatWin *target):alQAWin(NULL,270,150,"Eine Ganzzahl bitte:")
{

showview = new alQuickView(1.0,1.0,390.0,30.0 );
showview->userDraw = &code_plug_server::IntegerInput_userDraw;
showview->DoDraw = true;
AddChild(showview);

alpha = new alHButtonBar(10,30,390);
alpha1= new alHButtonBar(10,60,390);
alpha2= new alHButtonBar(10,90,390);
alpha3= new alHButtonBar(10,120,390);

buf = new BString ("1");
ziel = target;


alHandler *xh = new alHandler("One good fellow");
xh->MessageReceived = &code_plug_server::InInCon;
this->AnyMessageHandler = xh;

AddChild( alpha);
AddChild( alpha1);
AddChild( alpha2);
AddChild( alpha3);


alpha->AddButton("0",new BMessage(10));
alpha->AddButton("1",new BMessage(1));
alpha->AddButton("2",new BMessage(2));
alpha1->AddButton("3",new BMessage(3));
alpha1->AddButton("4",new BMessage(4));
alpha1->AddButton("5",new BMessage(5));
alpha2->AddButton("6",new BMessage(6));
alpha2->AddButton("7",new BMessage(7));
alpha2->AddButton("8",new BMessage(8));
alpha3->AddButton("9",new BMessage(9));
alpha3->AddButton("<-",new BMessage(11));
alpha3->AddButton("OK",new BMessage(12));


SetLook(B_BORDERED_WINDOW_LOOK);
SetFeel(B_MODAL_APP_WINDOW_FEEL);
masterinput = this;
}

IntegerInput::~IntegerInput()
{
this->DeleteMyHandler(true);
delete buf;
alpha->RemoveSelf();
delete alpha;
alpha1->RemoveSelf();
delete alpha1;
alpha2->RemoveSelf();
delete alpha2;
alpha3->RemoveSelf();
delete alpha3;
}

void IntegerInput::MouseDown( BPoint x )
{

}

void IntegerInput::CheckBuf()
{
if ((buf->Length() > 1) && (buf[0] == "0"))
		{
		buf->RemoveFirst("0");
		}
}



//Main program
int main()
{
extra::fStringOutOf(-1.2345,6);
RefOnStart = false;
oneisrunning = false;
anyH = new alHandler();
anyH->MessageReceived = &code_plug_server::CONTROL;
prog = new alQuickApp(anyH,"Application/to be overridden");
prog->Run();
}