//#include <EXTRA.h>
#include "MatrixFunctionality.h"
#include <Slider.h>

const int CONST_Qapp = 1;
const int CONST_Qapp2 = 3;
const int CONST_NewMatrix = 2;
const int CONST_MirrorFunc = 4;
const int CONST_ResizeMatrix = 5;
const int CONST_ResizeMatrixOK = 6;
const int CONST_EXEC = 7;
const int CONST_MirrorMajor = 8;
const int CONST_MirrorMinor = 9;
// const int CONST... = 10 is reserved
const int CONST_MOD = 11;
// 12 is reserved
// 13 is reserved
const int CONST_OpenMatrixFiles = 14;
const int CONST_SaveMatrixToFile = 15;
const int CONST_Clear = 16;
const int CONST_FieldResize = 17;
const int CONST_A1 = 18;
const int CONST_A2 = 19;
const int CONST_A3 = 20;
const int CONST_A4 = 21;
const int CONST_A5 = 22;
const int CONST_A6 = 23;
const int CONST_A7 = 24;
const int CONST_A8 = 25;

class DetMatWin : public alQAWin
{
public:

alBitmapButton *TCBb1;
alBitmapButton *TCBb2;
alBitmapButton *TCBb3;
alBitmapButton *TCBb4;
alBitmapButton *TCBb5;
alBitmapButton *TCBb6;
alSwitchButton *TCBs7;
alScrollControl *ScrollControl;
BSlider *matrixcontrol;

bool MatrixCreated;
MatrixControl *mxCon;

DetMatWin();
~DetMatWin();
};

class IntegerInput : public alQAWin
{
public:
IntegerInput(DetMatWin *target);
~IntegerInput();
DetMatWin *ziel;
BString *buf;
alHButtonBar *alpha;
alHButtonBar *alpha1;
alHButtonBar *alpha2;
alHButtonBar *alpha3;

alQuickView *showview;
virtual void MouseDown( BPoint x );
void CheckBuf();
};


namespace code_plug_server
{
void CONTROL ( BMessage *message , alHandler *thisone );
void InInCon(BMessage *message , alHandler *thisone);
void IntegerInput_userDraw ( alQuickView *me );
};