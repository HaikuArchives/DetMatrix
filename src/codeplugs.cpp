#include "DetMatrix.h"
#include <stdlib.h>
#include <FilePanel.h>
#include <NodeInfo.h>

BFilePanel *OpenPanel;
BFilePanel *SavePanel;
extern int OpenWindows;
extern BBitmap *res_pic[8];
BBitmap *Ikone[2];
extern IntegerInput *masterinput;
extern bool oneisrunning;

extern alQuickApp *prog;
extern alHandler *anyH;
BMessage *startRef;
bool RefOnStart;

extern unsigned char *extra_arrow[70];
extern unsigned char *extra_busy[70];
extern int GLOBAL_ACCURACY;


void OpenMatrixFiles(BMessage *message)
{
long Files;
type_code code;
BFile *Datei;
entry_ref Referenz;
long dimension,lenge;
float dim;
MatrixControl *con;
ssize_t see;

message->GetInfo("refs",&code, &Files);

long i;
for (i=0; i<Files; ++i)
	{
	message->FindRef("refs",i, &Referenz);
	Datei = new BFile(&Referenz, B_READ_WRITE);	
	
	if (Datei->InitCheck() == B_NO_ERROR)
		{
		DetMatWin *win;
		win = new DetMatWin;
	
		win->ResizeTo(500,400);
		win->Lock();
		win->mxCon->RemoveSelf();
		delete win->mxCon;
		delete win->ScrollControl;
		
		lenge = sizeof(float);
		Datei->Read(&dim,lenge);	
		dimension = (long) dim;

		con= new MatrixControl(dimension,70);				
		win->mxCon = con;
		win->AddChild(con);
		
		win->ScrollControl = new alScrollControl (con,true,true);
		
		
		Datei->Read(&con->Koeffizient,lenge);
		
		see = Datei->Read(con->float_matrix,lenge*dimension*dimension);		
		
		con->Invalidate();
			
		win->Unlock();
		win->Show();
		++OpenWindows;
		}
	delete Datei;
	}
}

void SaveMatrixToFile(BMessage *message)
{
BFile targetfile;
status_t result;
MatrixControl *con;
void **con_void;

entry_ref dir;
const char *filename = NULL;

message->FindRef("directory",&dir);
message->FindString("name", &filename);
message->FindPointer("MatrixControl", con_void);

con = (MatrixControl *) *con_void;

BDirectory directory(&dir);
if (directory.InitCheck() == B_NO_ERROR) 
	{

	result = directory.CreateFile(filename, &targetfile,true);

	if (result != B_OK)
		{
		extra::ShowFileError(&result);
		return;
		}
		
	if (targetfile.InitCheck() == B_NO_ERROR)
		{
		BNodeInfo FileInfo(&targetfile);
		if (FileInfo.InitCheck() == B_NO_ERROR)
			{
			FileInfo.SetType("Application/DetMatrix-Matrix");
			FileInfo.SetPreferredApp("Application/DetMatrix",B_OPEN);
			FileInfo.SetIcon( Ikone[0], B_LARGE_ICON );
			FileInfo.SetIcon( Ikone[1], B_MINI_ICON );
			
			float *FileContent;
			long i,k,pos;
			long FileLength=0;
			
			FileLength = sizeof(float)*((con->matrix_dimension * con->matrix_dimension) +2);
			FileContent = new float[FileLength];
			
			*FileContent = (float) con->matrix_dimension;
			*(FileContent+1) = con->Koeffizient;
			
			for (i=0; i<con->matrix_dimension;++i)
				{
				for (k=0; k<con->matrix_dimension;++k)
					{
					pos = (i * con->matrix_dimension) +k;
					*(FileContent+2+pos) = con->GetAtPosition(k,i);
					}
				}
			
			targetfile.Write(FileContent, FileLength);
			delete FileContent;
			}
		}
	}
	
}


void code_plug_server::InInCon(BMessage *message , alHandler *thisone)
{
switch (message->what)
	{
	case 1:
		 {
		 masterinput->buf->Append("1");
		 masterinput->showview->Refresh();
		 break;
		 }
	case 2:
		 {
		 masterinput->buf->Append("2");
 		 masterinput->showview->Refresh();
		 break;
		 }
	case 3:
		 {
		 masterinput->buf->Append("3");
		 masterinput->showview->Refresh();
		  break;
		 }
	case 4:
		 {
		 masterinput->buf->Append("4");
		 masterinput->showview->Refresh();		 
		 break;
		 }
	case 5:
		 {
		 masterinput->buf->Append("5");
		 masterinput->showview->Refresh();		 
		 break;
		 }
	case 6:
		 {
		 masterinput->buf->Append("6");
		 masterinput->showview->Refresh();		 
		 break;
		 }
	case 7:
		 {
		 masterinput->buf->Append("7");
 		 masterinput->showview->Refresh();
		 break;
		 }
	case 8:
		 {
		 masterinput->buf->Append("8");
		 masterinput->showview->Refresh();		 
		 break;
		 }
	case 9:
		 {
		 masterinput->buf->Append("9");
		 masterinput->showview->Refresh();		 
		 break;
		 }
	case 10:
		 {
		 masterinput->buf->Append("0");
		 masterinput->showview->Refresh();		 
		 break;
		 }
	case 11:
		 {
		 masterinput->buf->Truncate(masterinput->buf->Length()-1,true);
		 masterinput->showview->Refresh();
		 break;
		 }
	case 12:
		{
		int32 x = atoi(masterinput->buf->String());
		
		BMessage *mes = new BMessage(CONST_ResizeMatrixOK);
		mes->AddInt32("parameter",x);
		masterinput->ziel->PostMessage(mes);
		delete mes;
		break;
		}
	}
masterinput->CheckBuf();
}


void code_plug_server::CONTROL ( BMessage *message , alHandler *thisone )
{
DetMatWin *win;
win = (DetMatWin *) prog->GetActiveWindow();

switch (message->what)
		{
		case CONST_MainFrameQuits:
			{
			--OpenWindows;
			if (OpenWindows == 0) prog->PostMessage(B_QUIT_REQUESTED);			
			break;
			}
		case CONST_AppReadyToRun:
			{
			const char *pth =  prog->MyFilePosition->Path();
			res_pic[0] = extra::GetResourceBitmap(pth ,'BMPX',1);
			res_pic[1] = extra::GetResourceBitmap(pth,'BMPX',2);
			res_pic[2] = extra::GetResourceBitmap(pth,'BMPX',3);
			res_pic[3] = extra::GetResourceBitmap(pth,'BMPX',4);
			res_pic[4] = extra::GetResourceBitmap(pth,'BMPX',5);
			res_pic[5] = extra::GetResourceBitmap(pth,'BMPX',6);
			res_pic[6] = extra::GetResourceBitmap(pth,'BMPX',7);
			res_pic[7] = extra::GetResourceBitmap(pth,'BMPX',8);
			Ikone[0] = extra::GetResourceBitmap(pth,'ICON',101);
			Ikone[1] = extra::GetResourceBitmap(pth,'MICN',101);
			
			if (RefOnStart == true)
				{
				OpenMatrixFiles(startRef);
				delete startRef;
				RefOnStart = false;
				}
			
			if (OpenWindows == 0)
				{
				DetMatWin *win;
				win = new DetMatWin();
				OpenWindows = 1;
				win->Show();
				}
				
	 		OpenPanel = new BFilePanel (B_OPEN_PANEL, NULL, NULL, B_FILE_NODE,true,NULL,NULL,true,true);
			SavePanel = new BFilePanel (B_SAVE_PANEL, NULL, NULL, B_FILE_NODE,false,NULL,NULL,true,true);
			
			prog->CursorArrow();			
			break;
			}
		case CONST_Qapp2:
			{
			extra::WaitMilisecs(100);
			win->TCBb1->SetValue(0);
			}//hier kommt kein BREAK;
		case CONST_Qapp:
			{
			int x;
			if (OpenWindows > 0)
				{
				x = extra::YesNoBox("Do you really want to quit?");
				prog->CheckCursor();
				}
				else x = 0;
			
			if (x > 0) break;
			
			OpenPanel->Window()->Quit();
			//delete OpenPanel;
			SavePanel->Window()->Quit();
			//delete SavePanel;
			prog->PostMessage(B_QUIT_REQUESTED);
			break;
			}
		case CONST_NewMatrix:
			{
			DetMatWin *win;
			win = new DetMatWin();

			win->mxCon->Invalidate();
			win->Show();
			++OpenWindows;
			break;
			}
		case CONST_MirrorFunc:
			{
			int x;
			x = extra::CBox2("Major axis","Minor axis");
			prog->CursorArrow();
			extra::WaitMilisecs(100);

			win->TCBb2->SetValue(0);
			if (x == 0) win->mxCon->SpiegelnHaupt();
			if (x == 1) win->mxCon->SpiegelnNeben();
			win->mxCon->Invalidate();			
			break;
			}
		case CONST_FieldResize:
			{
			int nv = win->matrixcontrol->Value();
			win->mxCon->square_size = nv;
			win->mxCon->Invalidate();
			break;
			}
		case CONST_MirrorMinor:
			{
			win->mxCon->SpiegelnNeben();
			win->mxCon->Invalidate();
			break;
			}			
		case CONST_MirrorMajor:
			{
			win->mxCon->SpiegelnHaupt();
			win->mxCon->Invalidate();
			break;
			}			
		case CONST_ResizeMatrix:
			{			
			extra::WaitMilisecs(100);
			win->TCBb3->SetValue(0);
			
			IntegerInput *x = new IntegerInput(win);
			win->TCBs7->SetValue(0);
			x->ziel = win;
			x->Show();
			break;
			}
		case CONST_ResizeMatrixOK:
			{
			int32 x;
			message->FindInt32("parameter", &x);			

			win = masterinput->ziel;
			masterinput->Quit();					
			win->ResizeTo(500,400);

			win->Lock();
			delete win->ScrollControl;
			win->mxCon->RemoveSelf();
			delete win->mxCon;

			win->mxCon = new MatrixControl(x,70);
			win->AddChild(win->mxCon);
			
			win->ScrollControl = new alScrollControl(win->mxCon, true,true);

			win->Unlock();
			break;
			}
		case CONST_EXEC:
			{
			if (oneisrunning == true) break;
			extra::WaitMilisecs(100);
			win->TCBb6->SetValue(0);
			win->TCBb6->SetEnabled(false);
			win->mxCon->Run(win);
			
			prog->CursorBusy();
			break;
			}	
		case 10:
			{
			win = (DetMatWin *) thisone->Parent;
			win->TCBb6->SetEnabled(true);
			
			if (win->mxCon->answers == win->mxCon->loaded_threads)
				{
				extra::OKBox ( extra::fStringOutOf( win->mxCon->MatrixResult,3).String() );
				}

			prog->CursorArrow();				
			break;
			}	
		case CONST_MOD:
			{
			win->mxCon->SwitchMode();
			break;
			}	
		case CONST_SaveMatrixToFile:
			{
			extra::WaitMilisecs(100);
			win->TCBb5->SetValue(0);
			
			BMessage *mes;
			mes = new BMessage(B_SAVE_REQUESTED);
			mes->AddPointer("MatrixControl",win->mxCon);
			
			SavePanel->SetMessage(mes);
			delete mes;
			
			SavePanel->Show();
			break;
			}
		case B_SAVE_REQUESTED:
			{
			SaveMatrixToFile(message);
			break;
			}
		case CONST_OpenMatrixFiles:
			{
			extra::WaitMilisecs(100);
			win->TCBb4->SetValue(0);
			
			OpenPanel->Show();
			break;
			}
		case B_REFS_RECEIVED:
			{
			if (prog->IsLaunching() == true) 
				{
				startRef = new BMessage( *message );
				RefOnStart = true;
				break;
				}
			OpenMatrixFiles(message);
			break;
			}
		case CONST_Clear:
			{
			win->mxCon->ClearMatrix();
			win->mxCon->Invalidate();
			break;
			}
		case CONST_A1:
			{
			GLOBAL_ACCURACY = 1;
			win->mxCon->Invalidate();
			break;
			}
		case CONST_A2:
			{
			GLOBAL_ACCURACY = 2;
			win->mxCon->Invalidate();
			break;
			}
		case CONST_A3:
			{
			GLOBAL_ACCURACY = 3;
			win->mxCon->Invalidate();
			break;
			}
		case CONST_A4:
			{
			GLOBAL_ACCURACY = 4;
			win->mxCon->Invalidate();
			break;
			}
		case CONST_A5:
			{
			GLOBAL_ACCURACY = 5;
			win->mxCon->Invalidate();
			break;
			}
		case CONST_A6:
			{
			GLOBAL_ACCURACY = 6;
			win->mxCon->Invalidate();
			break;
			}
		case CONST_A7:
			{
			GLOBAL_ACCURACY = 7;
			win->mxCon->Invalidate();
			break;
			}
		case CONST_A8:
			{
			GLOBAL_ACCURACY = 8;
			win->mxCon->Invalidate();
			break;
			}

		}
}


void code_plug_server::IntegerInput_userDraw ( alQuickView *me )
{
masterinput->Lock();

escapement_delta delta;
delta.space = 0;
delta.nonspace = 0;

me->SetHighColor(255,255,255,255);
me->FillRect( me->Frame(), B_SOLID_HIGH);
me->SetHighColor(0,0,0,255);
me->DrawString (masterinput->buf->String(), BPoint(10,25), &delta );
masterinput->Unlock();
}