#include <EXTRA.h>
#include <StatusBar.h>
#include <ScrollView.h>

const int CONST_dragrow = 12;
const int CONST_dragcolumn = 13;

class ShowProgressWin : public alNormalWindow
{
public:
ShowProgressWin(float width, float height, const char *text, float range);
~ShowProgressWin();
void SetKnobTo(float value);

alNormalWindow *tonotify;
alLooper *toshutLooper;
BStatusBar *stat_bar;
thread_id *threadid; //Pointer to array
int32 threads;
};


class MatrixControl : public alControl
{
public:
float *float_matrix;
float Koeffizient;
long matrix_dimension;
int square_size;
int posx,posy;
bool showcursor;
bool cursoron;
bool DrawfromPulse;
bool CursorOnKoeff;
float MatrixResult;
int loaded_threads, answers;
bool DataEnterMode;
BString *edit;

MatrixControl(int elements, int size);
~MatrixControl();

virtual void AttachedToWindow();
virtual void Draw (BRect restrictiveRect);
virtual void MouseDown( BPoint x );
virtual void Pulse();
virtual void KeyDown ( const char *what, int32 numKeys );
virtual void SwitchRows( int source, int target );
virtual void SwitchColumns ( int source, int target );
virtual void MessageReceived( BMessage *message); 
virtual void FrameResized(float width, float height);
virtual void NewHScrollValue( float x );
virtual void NewVScrollValue( float y );
void SpiegelnHaupt();
void SpiegelnNeben();
void Run(alNormalWindow *win);
float GetAtPosition(int a, int b);
void SwitchMode();
void ClearMatrix();

};


class Determinante
{
public:
Determinante *Parent;
MatrixControl *Control;
int Position, Size;
int Range_start, Range_end;
float Koeffizient;
bool isParent;


Determinante(int SizeX);
~Determinante();

void SetMyParent(Determinante *ParentX);
float GetAtPosition( int a, int b);
float CalculateDeterminante();
};


