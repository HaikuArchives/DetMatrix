#include "MatrixFunctionality.h"

extern ShowProgressWin *theone;

Determinante::Determinante(int SizeX)
{
isParent = true;
Size = SizeX;
Position = -1;
Range_start = 0;
Range_end = Size - 1;
}


Determinante::~Determinante()
{
}


void Determinante::SetMyParent( Determinante *ParentX)
{
isParent = false;
Parent = ParentX;
}

float pot(int num)
{
int i;
int a = 1;

if (num == 0) return 1;

for (i=1; i <= num; ++i) a = a * (-1);

return (float) a;	
}


float Determinante::GetAtPosition( int a, int b)
{
int c,d;
c=a;
d=b;

if (c >= Position) ++c;
++d;

switch (isParent)
	{
	case true:
		{
		float ap;
		
		ap = Control->GetAtPosition(c,d);
		return ap;
		}
	case false:
		{
		return Parent->GetAtPosition(c,d);
		}
	}
return 0;	
}


float Determinante::CalculateDeterminante()
{
Position = Range_start;
float result = 0;
float puffer = 0;
float ap = 0;
Determinante *UnterDet;
BMessage *mes;

if (isParent == true)
	{
	mes = new BMessage(67);
	}

do
	{
	if (isParent == false) {ap = Parent->GetAtPosition(Position,0); }
					  else {ap = Control->GetAtPosition(Position,0) * Koeffizient;}
	
	ap *= pot(Position);
	
	if (Size == 1) {puffer = 1;}
	else
		{
		if (ap != 0.0)
			{
			UnterDet = new Determinante(Size -1);
			UnterDet->SetMyParent(this);

			
			puffer = UnterDet->CalculateDeterminante();
			delete UnterDet;
			}
		}
	
	result += (puffer * ap);
	++Position;
	
	if (isParent == true) 
			{
			theone->PostMessage(mes);
			}
	
	} while (Position <= Range_end);
	

if (isParent == true) 
	{
	delete mes;
	delete this;
	}
	
return result;		
}