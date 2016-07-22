#include "MatrixFunctionality.h"
#include <stdlib.h>
#include <ScrollBar.h>

ShowProgressWin *theone;
MatrixControl *MasterMatrix;
bool oneisrunning;
extern int GLOBAL_ACCURACY;


void MatrixControl::AttachedToWindow()
{
Window()->SetPulseRate(1200000);
SetViewColor(200,200,200,255);
}

MatrixControl::MatrixControl(int elements, int size):alControl(10,80,480,370,"MatrixControl","none", NULL, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_PULSE_NEEDED | B_FULL_UPDATE_ON_RESIZE | B_NAVIGABLE | B_FRAME_EVENTS)
{
MatrixResult = 0.0;
matrix_dimension = elements;
square_size = size;
if (matrix_dimension < 1) matrix_dimension = 1;

x_maxRange = (matrix_dimension+1) * square_size -480;
if (x_maxRange < 0)  x_maxRange = 0;
y_maxRange = (matrix_dimension+1) * square_size - 370;
if (y_maxRange < 0)  y_maxRange = 0;
CheckBars();

posx=0;
posy=0;
showcursor = true;
cursoron = true;

float_matrix = new float[elements*elements];
DataEnterMode = true;

int i;
int a;
a = elements * elements;
Koeffizient = 1.0;

for (i=0;i<a;++i) float_matrix[i]= i;
edit = new BString(extra::fStringOutOf(float_matrix[0],GLOBAL_ACCURACY).String());
DrawfromPulse = false;
}

MatrixControl::~MatrixControl()
{
delete [] float_matrix;
}

void MatrixControl::Draw (BRect restrictiveRect)
{

if (IsFocus() == false) showcursor=false;
Window()->Lock();

float orx = GetScrollX();
float ory = GetScrollY();

SetHighColor(0,0,0,255);
StrokeRect(BRect(orx,ory,Frame().Width()+orx,Frame().Height()+ory),B_SOLID_HIGH);

int i,k;
int32 a,b;
BString out;

escapement_delta delta;
delta.space = 0;
delta.nonspace = 0;


SetDrawingMode( B_OP_OVER );
SetFont( be_bold_font );

if ((CursorOnKoeff == true) && (cursoron == true)) 
	{
	SetHighColor(255,255,255,255);
	}

if (CursorOnKoeff) DrawString( edit->String() , BPoint(10,20), &delta); 
	else if (!DrawfromPulse) DrawString( extra::fStringOutOf( Koeffizient,GLOBAL_ACCURACY ).String()  , BPoint(10,20), &delta);


if /* A */ (DrawfromPulse == false)
	{

	for /* B */ (i=0;i<matrix_dimension;++i)
		{
		if (!DataEnterMode)
			{
			SetHighColor(255,40,40,255);
			FillRect(BRect( (i+1)*square_size-10, 20 ,(i+1)*square_size-5 , square_size * matrix_dimension), B_SOLID_HIGH );
			SetHighColor(40,40,255,255);
			FillRect(BRect( 20, (i+1)*square_size+5, square_size * (matrix_dimension+1) ,(i+1)*square_size+10  ), B_SOLID_HIGH );
			
			SetHighColor(0,0,0,255);
			StrokeRect(BRect( (i+1)*square_size-10, 20 ,(i+1)*square_size -5 , square_size * matrix_dimension), B_SOLID_HIGH );
			StrokeRect(BRect( 20, (i+1)*square_size+5, square_size * (matrix_dimension+1) ,(i+1)*square_size +10 ), B_SOLID_HIGH );
			}
				
		for /* C */(k=0;k<matrix_dimension;++k)
			{
			out.SetTo(extra::fStringOutOf(float_matrix[i*matrix_dimension +k],GLOBAL_ACCURACY).String());
			SetHighColor(255,50,127,255);
			if /* D */ ((showcursor == true) && (posx == k) && (posy == i) && (cursoron == true) && (CursorOnKoeff == false)) SetHighColor(255,255,255,255); // D
			
			a = (k+1)*square_size;
			b = (i+1)*square_size;
			DrawString(out.String(),BPoint(a,b), &delta);
			} // C
		} // B
	} // A
	else // A
	{
	 		//out.SetTo(extra::fStringOutOf(float_matrix[posy*matrix_dimension +posx],3).String());
	 		if (CursorOnKoeff == false)
	 		{
			bool optimizer;
			optimizer = ((showcursor == true) && (cursoron == true));
			
			SetHighColor(200,200,200,255);
			FillRect(BRect((posx+1)*square_size-10, posy*square_size+10, (posx+2)*square_size-10, (posy+1)*square_size+10));
			SetHighColor(255,50,127,255);
			DrawString(edit->String(),BPoint((posx+1)*square_size,(posy+1)*square_size), &delta);							
			if /* B */(optimizer == true) 
				{
				SetHighColor(255,255,255,255);
				DrawString("|", &delta);
				}// B
			}
	
	 } // A

Window()->Sync();
Window()->Unlock();
} 





void MatrixControl::MouseDown( BPoint x )
{
if (IsFocus() == false) MakeFocus(true);

x.y += GetScrollY();
x.x += GetScrollX();

if (!DataEnterMode)
	{
	int32 i;
	BRect *rect;
	BMessage *nachricht;
		
	for (i=0; i<matrix_dimension; ++i)
		{
		rect = new BRect( (i+1)*square_size-10, 20 ,(i+1)*square_size -5 , square_size * matrix_dimension);
		
		if (rect->Contains(x)==true)
			{
			nachricht = new BMessage(CONST_dragcolumn);	
			
			nachricht->AddInt32("column",i);
			nachricht->AddPointer("matrix_control",this);
			
			DragMessage(nachricht,BRect( (i+1)*square_size-10, 20 ,(i+1)*square_size -5 , square_size * matrix_dimension),NULL);
			delete nachricht;
			//Drag around a data row
			return;
			}
	
		delete rect;
		
		rect = new BRect( 20, (i+1)*square_size+5, square_size * (matrix_dimension+1) ,(i+1)*square_size +10 );
		
		if (rect->Contains(x)==true)
			{
			nachricht = new BMessage(CONST_dragrow);	
			
			nachricht->AddInt32("row",i);
			nachricht->AddPointer("matrix_control",this);
			
			DragMessage(nachricht,BRect( 20, (i+1)*square_size+5, square_size * (matrix_dimension+1) ,(i+1)*square_size +10 ),NULL);
			delete nachricht;
			
			return;
			}
			
		delete rect;
		}
	}

if (DataEnterMode)
{
int half;
half = square_size / 2;
posx = int ((x.x  / square_size) - 1);
posy = int (((x.y + half) / square_size) - 1);


showcursor = true;
if ((posx < 0) || (posx >= matrix_dimension)) {MakeFocus(false);posx = 0;}
if ((posy < 0) || (posy >= matrix_dimension)) {MakeFocus(false);posy = 0;}

if (x.y < 40) 
	{
	CursorOnKoeff = true; 
	edit->SetTo(extra::fStringOutOf(Koeffizient,GLOBAL_ACCURACY).String());	
	}
else 
	{
	CursorOnKoeff = false;
	edit->SetTo(extra::fStringOutOf(float_matrix[posy*matrix_dimension +posx],GLOBAL_ACCURACY).String());
	}
} // If (DataEnterMode)


if (IsFocus() == true)
	{
	DrawfromPulse = false;
	Invalidate();
	}
	

}


void MatrixControl::NewVScrollValue( float y )
{
}


void MatrixControl::NewHScrollValue( float x )
{
}


void MatrixControl::Pulse()
{
if (cursoron == true) cursoron=false; else cursoron = true;

if (!DataEnterMode) return;

DrawfromPulse = true;
Draw(BRect(0,0,0,0));
DrawfromPulse = false;
}


void MatrixControl::SpiegelnHaupt()
{
float *pmat;

pmat = new float[matrix_dimension*matrix_dimension];

int i,k;
int pos1, pos2;

for (i=0; i < matrix_dimension; ++i )
	for (k=0; k < matrix_dimension; ++k)
		{
		pos1 = (i * matrix_dimension) + k;
		pos2 = (k * matrix_dimension) + i;
		*(pmat + pos2) = *(float_matrix + pos1);
		}

delete [] float_matrix;
float_matrix = pmat;

Draw( BRect(0,0,0,0));		
}

void MatrixControl::SpiegelnNeben()
{
float *pmat;

pmat = new float[matrix_dimension*matrix_dimension];

int i,k;
int pos1, pos2;

for (i=0; i < matrix_dimension; ++i )
	for (k=0; k < matrix_dimension; ++k)
		{
		pos1 = (i * matrix_dimension) + k;
		pos2 = ((matrix_dimension - i - 1) * matrix_dimension) + (matrix_dimension - k - 1);
		*(pmat + pos2) = *(float_matrix + pos1);
		}

delete [] float_matrix;
float_matrix = pmat;

SpiegelnHaupt();
}


void MatrixControl::KeyDown ( const char *what, int32 numKeys )
{

unsigned int a = (unsigned int) *(what + numKeys - 1);
switch (a)
	{
	case '0': { edit->Append("0");break;}
	case '1': { edit->Append("1");break;}
	case '2': { edit->Append("2");break;}
	case '3': { edit->Append("3");break;}
	case '4': { edit->Append("4");break;}
	case '5': { edit->Append("5");break;}
	case '6': { edit->Append("6");break;}
	case '7': { edit->Append("7");break;}
	case '8': { edit->Append("8");break;}
	case '9': { edit->Append("9");break;}
	case '.': { edit->Append(".");break;}
	case '-': { edit->Append("-");break;}
	case B_BACKSPACE: { edit->Truncate(edit->Length()-1,true);break; }
	case B_ENTER: { 
				  float r;
				  r = atof ( edit->String() );
				  
				  if (CursorOnKoeff==true)
				  	{
				  	Koeffizient = r;
				  	}
				  else float_matrix[posy*matrix_dimension+posx] = r;
				  break;
				  }
	}
DrawfromPulse = true;
Draw(BRect(0,0,0,0));
}


int32 ThreadStart (void *data)
{
int32 *parameter;
parameter = (int32 *) data;

Determinante *OberDet= new Determinante(MasterMatrix->matrix_dimension);
OberDet->Koeffizient = MasterMatrix->Koeffizient;
OberDet->Control = MasterMatrix;
OberDet->Range_start = *parameter;
OberDet->Range_end = *(parameter+1);

MasterMatrix->loaded_threads++;
float result = OberDet->CalculateDeterminante();
MasterMatrix->MatrixResult += result;
MasterMatrix->answers++;

alLooper *x = (alLooper *) *(parameter+2);
x->PostMessage(3);
delete parameter;
return 0;
}


void MasterMatrixReceiver ( BMessage *m, alHandler *thisone)
{
alLooper *mmc;
mmc = (alLooper *) thisone->Parent;


switch (m->what)
	{
	case 1: 
		{
		int32 CPUs;
		system_info info;
		get_system_info(&info);
		CPUs = info.cpu_count;

		int planned_threads;
		int width_per_thread;
		thread_id tid;
		int32 parameter[2];
		int32 *param;
		BString thread_name;
		MatrixControl *mcon;
		thread_id *p;
		
		mcon = (MatrixControl *) mmc->Parent; 
		planned_threads = mcon->matrix_dimension; //With these two lines we find out the best count of threads to perform this task.
		if (planned_threads > CPUs) planned_threads = CPUs;
		
		width_per_thread = mcon->matrix_dimension / planned_threads;
		if (width_per_thread == 0) width_per_thread = 1;
		
		theone->threadid = new int32[planned_threads];
		theone->threads = planned_threads;
		p = theone->threadid;
		
		parameter[0]=0;
		for (int i=1; i<= planned_threads; ++i)
			{
			param = new int32[3];
			parameter[1] = parameter[0] - 1 + width_per_thread;
			
			if (i==planned_threads) 
					{
					parameter[1] = mcon->matrix_dimension -1; //Stellen Rundungsfehler sicher
					}
			
			*(param) = parameter[0];
			*(param+1) = parameter[1];
			*(param+2) = (int32) mmc;
			thread_name.SetTo("matrix thread ");
			thread_name += extra::StringOutOf(i);
			tid = spawn_thread(&ThreadStart,thread_name.String(),B_LOW_PRIORITY, param);
			*(p + i -1) = tid;
			resume_thread(tid);
			parameter[0] = parameter[1] + 1;
			}		
		break;
		}
	case 2:
		{
		oneisrunning = false;
		mmc->Quit();
		break;				
		}
	case 3:
		{
		MatrixControl *mcon;
		mcon = (MatrixControl *) mmc->Parent;		
		if (mcon->answers == mcon->loaded_threads)
			{
			oneisrunning=false;
			theone->Quit();		
			}
		break;
		}		
	}
}






void MatrixControl::Run(alNormalWindow *win)
{
//Optimizer Section
int i ,k, x;
int counted_zeros[matrix_dimension];
int bestline,val;
float value;

for (i=0; i < matrix_dimension; ++i)
	{
	counted_zeros[i] = 0;
	for (x=0; x < matrix_dimension; ++x)
		{
		value = float_matrix[i*matrix_dimension +x];
		if (value == 0.0) ++counted_zeros[i];
		}
	}


for (i=0; i < matrix_dimension; ++i)
	{
	bestline = -1;
	val = 0;
	for (k = i; k < matrix_dimension; ++k)
		{
		if (counted_zeros[k] > val) 
			{
			val = counted_zeros[k]; 
			bestline = k;
			}
		}
	if (bestline > i)
		SwitchRows(bestline, i);		
	}
//Optimizer End

oneisrunning = true;

MasterMatrix = this;
loaded_threads = 0;
answers = 0;
MatrixResult = 0.0;

BString nam ("Progress ");
nam += extra::StringOutOf(matrix_dimension);
nam.Append("x");
nam += extra::StringOutOf(matrix_dimension);


alLooper *master_matrix_calculator = new alLooper(&MasterMatrixReceiver,"Master Matrix Calculator",B_LOW_PRIORITY);
theone = new ShowProgressWin(400,100,nam.String(),100);

theone->stat_bar->SetMaxValue( (float) matrix_dimension );
theone->tonotify = win;
theone->toshutLooper = master_matrix_calculator;
theone->Show();



master_matrix_calculator->Parent = (char *) this;
master_matrix_calculator->Run();
master_matrix_calculator->PostMessage(1);
}


float MatrixControl::GetAtPosition( int a, int b )
{
float c;
int32 p;

p = (b * matrix_dimension) + a;
c = *(float_matrix + p);
return c;
}


void MatrixControl::SwitchRows( int source, int target )
{
float a;

int i;

for (i=0; i < matrix_dimension; ++i)
	{
	a = float_matrix[target * matrix_dimension +i];
	float_matrix[target * matrix_dimension +i] = float_matrix[source * matrix_dimension +i];
	float_matrix[source * matrix_dimension +i] = a;
	}
	
Koeffizient *= (-1);	
}

void MatrixControl::SwitchColumns ( int source, int target )
{
float a;

int i;

for (i=0; i < matrix_dimension; ++i)
	{
	a = float_matrix[i * matrix_dimension +target];
	float_matrix[i * matrix_dimension +target] = float_matrix[i * matrix_dimension +source];
	float_matrix[i * matrix_dimension +source] = a;
	}
	
Koeffizient *= (-1);
}



void MatrixControl::SwitchMode()
{
if (DataEnterMode) DataEnterMode = false; else DataEnterMode=true;
Invalidate();
}


void MatrixControl::MessageReceived( BMessage *message)
{
switch (message->what)
	{
	case CONST_dragrow:
		{
		int32 source_row;
		BPoint target;
		BRect *rect;
		MatrixControl *con;
		void **x;
		
		message->FindInt32("row", &source_row);
		message->FindPoint("_drop_point_", &target);
		message->FindPointer("matrix_control", x);
		con = (MatrixControl *) *x;
    	if (con != this) {
    					 extra::OKBox ("You can´t drag from another window!");
    					 break;
    					 }
		target = ConvertFromScreen(target);

		
		for (int i=0; i<matrix_dimension; ++i)
			{
			rect = new BRect( 20, (i+1)*square_size+5, square_size * (matrix_dimension+1) ,(i+1)*square_size+10 );
			
			if (i != source_row)
			{
			if (rect->Contains(target)==true)
				{
				SwitchRows(source_row, i);
				Invalidate();
				} // if clause
			}
			
			delete rect;
			} // for loop

		} //case diferentiation
	case CONST_dragcolumn:
		{
		int32 source_row;
		BPoint target;
		BRect *rect;
		MatrixControl *con;
		void **x;
		
		message->FindInt32("column", &source_row);
		message->FindPoint("_drop_point_", &target);
		message->FindPointer("matrix_control", x);
		con = (MatrixControl *) *x;
    	if (con != this) 
    			{
    			extra::OKBox("You can´t drag from another window!");
    			break;
    			}

		target = ConvertFromScreen(target);
		
		for (int i=0; i<matrix_dimension; ++i)
			{
			rect = new BRect((i+1)*square_size-10, 20 ,(i+1)*square_size-5 , square_size * matrix_dimension);
			
			if (i != source_row)
			{
			if (rect->Contains(target)==true)
				{
				SwitchColumns(source_row, i);
				Invalidate();
				} // if clause
			}
			
			delete rect;
			} // for loop

		} //case diferentiation
	} //switch statement 
} //function




void MatrixControl::ClearMatrix()
{
int i;

for (i=0; i < matrix_dimension*matrix_dimension; ++i) float_matrix[i] = 0.0;
}


void MatrixControl::FrameResized(float width, float height)
{
x_maxRange = (matrix_dimension+1) * square_size - width;
if (x_maxRange < 0)  x_maxRange = 0;
y_maxRange = (matrix_dimension+1) * square_size - height;
if (y_maxRange < 0)  y_maxRange = 0;
CheckBars();
}