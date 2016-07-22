#include <EXTRA.h>
#include <TranslationUtils.h>
#include <Resources.h>
#include <OS.h>
#include <ScrollBar.h>
#include <math.h>

//Mouse Pointer

unsigned char extra_arrow[70] = {16,1,0,0,
0x80,0,0xC0,0,0xA0,0,0x90,0,0x88,0,0x84,0,0x82,0,0x81,0,0x80,0x80,0x80,0x40,0x80,0x20,0x87,0xF0,0x9C,0,0xA0,0,0xC0,0,0x80,0,
0x80,0,0xC0,0,0xE0,0,0xF0,0,0xF8,0,0xFC,0,0xFE,0,0xFF,0,0xFF,0x80,0xFF,0xC0,0xFF,0xE0,0xFF,0xF0,0xFC,0,0xA0,0,0xC0,0,0x80,0,0,0};

unsigned char extra_busy[70] = {16,1,7,7,
0x3F,0xFC,0x3F,0xFC,0x10,0x08,0x10,0x08,0x10,0xC8,0x0F,0xF0,0x07,0xE0,0x03,0xC0,0x02,0x40,0x04,0x20,0x09,0x90,0x10,0x08,0x11,0x88,0x13,0xC8,0x3F,0xFC,0x3F,0xFC,
0x3F,0xFC,0x3F,0xFC,0x1F,0xF8,0x1F,0xF8,0x1F,0x38,0x08,0x10,0x04,0x20,0x02,0x40,0x03,0xC0,0x07,0xE0,0x0E,0x70,0x1F,0xF8,0x1E,0x78,0x1C,0x38,0x3F,0xFC,0x3F,0xFC,0,0};

unsigned char extra_cross[70] = {16,1,7,7,
0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x7F,0xFC,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

unsigned char extra_pen[70] = {16,1,15,1,
0x00,0x11,0x00,0x22,0x00,0x44,0x00,0x88,0x01,0x11,0x02,0x22,0x04,0x44,0x0E,0x88,0x13,0x10,0x13,0xA0,0x10,0xC0,0x20,0x80,0x27,0x00,0x78,0x00,0x60,0x00,0x00,0x00,
0x00,0x1F,0x00,0x3F,0x00,0x7F,0x00,0xFF,0x01,0xFF,0x03,0xFE,0x07,0xFC,0x0F,0xF8,0x1F,0xF0,0x1F,0xE0,0x1F,0xC0,0x3F,0x80,0x3F,0x00,0x78,0x00,0x60,0x00,0x00,0x00,0,0};

unsigned char extra_help[70] = {16,1,7,1,
0x03,0xE0,0x07,0xF0,0x0E,0x38,0x0C,0x18,0x0C,0x18,0x00,0x18,0x00,0x38,0x40,0x70,0x60,0xE0,0x70,0xC0,0x58,0xC0,0x4C,0x00,0x46,0x00,0x5C,0xC0,0x70,0xC0,0x40,0x00,
0x03,0xE0,0x07,0xF0,0x0E,0x38,0x0C,0x18,0x0C,0x18,0x00,0x18,0x00,0x38,0x40,0x70,0x60,0xE0,0x70,0xC0,0x78,0xC0,0x7C,0x00,0x7E,0x00,0x7C,0xC0,0x70,0xC0,0x40,0x00,0,0};



alThread::alThread (thr thread_function, const char* thread_name, bool autoresume, int32 priority )
{
successful = true;
ThisThread = spawn_thread ( thread_function, thread_name, priority, (void *) this);
if (ThisThread == B_NO_MORE_THREADS) successful = false;
if (ThisThread == B_NO_MEMORY) successful = false;
stop = false;
syncsign = true;

if (autoresume) resume_thread (ThisThread);
}


alThread::~alThread()
{
thread_info info;
status_t stat = get_thread_info(ThisThread, &info);
if (stat == B_OK)
	{
	kill_thread ( ThisThread );
	}
}

void alThread::Suspend()
{
suspend_thread( ThisThread );
}


void alThread::Resume()
{
resume_thread( ThisThread );
}


void alThread::WaitFor()
{
status_t stat;
wait_for_thread( ThisThread , &stat );
}


bool alThread::Sync()
{
bool x;
x = syncsign;
syncsign = false;
return x;
}






alRect::alRect():BRect(0,0,0,0)
{
}


alRect::alRect ( BRect rxc ):BRect(0,0,0,0)
{
SetLeftTop( rxc.LeftTop() );
SetRightBottom( rxc.RightBottom() );
}

alRect::alRect (float x1, float y1, float x2, float y2):BRect(x1,y1,x2,y2)
{
}

void alRect::ResizeTo (float width, float height)
{
BPoint P;
P = this->LeftTop();
SetRightBottom( BPoint(P.x + width, P.y + height) );
}

void alRect::SetWidth ( float width )
{
BPoint P;
P = LeftTop();
SetRightBottom( BPoint(P.x + width, this->Height()) );
}

void alRect::SetHeight (float height )
{
BPoint P;
P = LeftTop();
SetRightBottom( BPoint(this->Width(), P.y + height) );
}







alHandler::alHandler()
{
initialized=true;
isprotected = true;
if ((HandlerSemaphore = create_sem ( 1 , "Any Handler" )) < B_NO_ERROR ) initialized = false;
HandlerName = "alHandler class";
}


alHandler::alHandler(const char *name)
{
initialized=true;
isprotected = true;
if ((HandlerSemaphore = create_sem ( 1 , "Any Handler" )) < B_NO_ERROR ) initialized = false;
HandlerName = name;
}

alHandler::alHandler(MessageReceiver mr,const char *name)
{
initialized=true;
isprotected = true;
if ((HandlerSemaphore = create_sem ( 1 , "Any Handler" )) < B_NO_ERROR ) initialized = false;
HandlerName = name;
MessageReceived = mr;
}


void alHandler::HandInMessage ( BMessage *message )
{
if (initialized = false) return;

switch (isprotected)
	{
	case true:
		{
		if (acquire_sem(HandlerSemaphore) != B_NO_ERROR) return;

		MessageReceived ( message , this);

		release_sem(HandlerSemaphore);
		break;
		}
	case false:
		{
		MessageReceived ( message, this);
		break;
		}		
	}
}

void alHandler::Open()
{
isprotected=false;
}

void alHandler::Protect()
{
isprotected=true;
}

alHandler::~alHandler()
{
}



















alNormalWindow::alNormalWindow ( alHandler *TH, float x1, float y1, float x2, float y2, const char *Name ):BWindow(BRect(x1,y1,x2,y2), Name , B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_WILL_ACCEPT_FIRST_CLICK)
{
AnyMessageHandler = TH;
op_modus=1;
oldw=x2-x1;
oldh=y2-y1;
skipanyfurther = false;		
killhandler = false;
}

alNormalWindow::alNormalWindow ( alHandler *TH, float width, float height, const char *Name ):BWindow(BRect(0,0,width,height), Name , B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_WILL_ACCEPT_FIRST_CLICK)
{
float x,y;
BPoint ZP;
BScreen *Screen;

Screen = new BScreen ( this );
op_modus=1;
oldh=height;
oldw=width;

x = Screen->Frame().Width();
y = Screen->Frame().Height();

ZP.Set( ((x - width) / 2), ((y-height) / 2) );

MoveTo( ZP );

AnyMessageHandler = TH;
delete Screen;
skipanyfurther = false;		
killhandler = false;
}

alNormalWindow::alNormalWindow ( alHandler *TH, const char *Name ):BWindow(BRect(0,0,100,100), Name , B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_WILL_ACCEPT_FIRST_CLICK )
{
BScreen *Screen;
op_modus=1;

Screen = new BScreen ( this );

ResizeTo(Screen->Frame().Width(), Screen->Frame().Height());
oldh=Screen->Frame().Height();
oldw=Screen->Frame().Width();

MoveTo( BPoint(0,0) );

AnyMessageHandler = TH;
delete Screen;
skipanyfurther = false;		
killhandler = false;
}


void alNormalWindow::CloseWin()
{
AnyMessageHandler->Open();
this->Lock();
this->Quit();
AnyMessageHandler->Protect();
}

void alNormalWindow::MessageReceived ( BMessage *message )
{
if (AnyMessageHandler != NULL) {
AnyMessageHandler->HandInMessage( (BMessage *) message );
					   }
					   
BWindow::MessageReceived (message);					   
}


void alNormalWindow::FrameMoved(BPoint origin)
{
if (skipanyfurther == true) {skipanyfurther = false; return;}
skipanyfurther = true;
if (op_modus==2) 
		{
		Lock();
		BPoint p;
		p = Frame().LeftTop();

		float w,h;
		
		w = Frame().Width();
		h = Frame().Height();
		
		w += (orx - p.x);
		h += (ory - p.y);
			orx = p.x;
			ory = p.y;
		
		ResizeTo(w,h);
		oldw=w;
		oldh=h;
		Unlock();
		}
}

void alNormalWindow::FrameResized(float width, float height)
{
if (skipanyfurther == true) {skipanyfurther = false;	return;}
skipanyfurther = true;

if (op_modus==2) 
		{
		Lock();
		float w,h;
		
		w=(oldw-width)/1.9;
		h=(oldh-height)/1.9;
		MoveBy(w,h);
		BPoint p;
		p = Frame().LeftTop();
		orx = p.x;
		ory = p.y;
		Unlock();
		}
		
oldw=width;
oldh=height;

}


void alNormalWindow::DispatchMessage(BMessage *message, BHandler *target)
{
Lock();
if (message->what == B_MINIMIZE)
			{
			if (op_modus == 1) op_modus = 2; else op_modus = 1;
			BPoint p;
			p = Frame().LeftTop();
			orx = p.x;
			ory = p.y;
			Unlock();			
			return;
			}
			
if (message->what == B_ZOOM)
			{
			if (op_modus == 2) {
				    			Minimize(true);
				    			Unlock();			
								return;
								}
			
			}
if (message->what == B_MOUSE_DOWN)
			{
			Activate();
			}
Unlock();			
BWindow::DispatchMessage(message, target);			
}

alNormalWindow::~alNormalWindow()
{
if (AnyMessageHandler) {
AnyMessageHandler->Open();
AnyMessageHandler->HandInMessage( new BMessage( CONST_MainFrameQuits) );
AnyMessageHandler->Protect();

if (killhandler) delete AnyMessageHandler;
					   }						   
}





void alNormalWindow::OpenHandler()
{
if (AnyMessageHandler) AnyMessageHandler->Open();
}

void alNormalWindow::ProtectHandler()
{
if (AnyMessageHandler) AnyMessageHandler->Protect();
}


void alNormalWindow::KeyDown(const char *bytes, int32 numBytes)
{
//CurrentFocus()->KeyDown ( bytes, numBytes );
}

void alNormalWindow::Delete(BView *any)
{
if (any->RemoveSelf() == true) delete any;
}

void alNormalWindow::FixFrame()
{
SetFlags(B_NOT_MOVABLE | B_NOT_ZOOMABLE | B_NOT_CLOSABLE | B_NOT_MINIMIZABLE | B_NOT_RESIZABLE);
}

void alNormalWindow::FreeFrame()
{
SetFlags(B_WILL_ACCEPT_FIRST_CLICK);
}


void alNormalWindow::Pulse()
{
if (AnyMessageHandler) {
AnyMessageHandler->HandInMessage( new BMessage( CONST_PULSE) );
					   }
}

void alNormalWindow::Modus1()
{
op_modus=1;
			BPoint p;
			p = Frame().LeftTop();
			orx = p.x;
			ory = p.y;
}

void alNormalWindow::Modus2()
{
op_modus=2;
			BPoint p;
			p = Frame().LeftTop();
			orx = p.x;
			ory = p.y;
}

void alNormalWindow::MouseDown ( BPoint p )
{
this->Activate(true);

BView *anyview;

anyview = FindView( p );
anyview->MouseDown( p );
}

void alNormalWindow::DeleteMyHandler(bool yesno)
{
killhandler = true;
}













alQAWin::alQAWin ( alHandler *TH, float x1, float y1, float x2, float y2, const char *Name ):alNormalWindow(TH,x1,y1,x2,y2,Name)
{
MainFrameMenu = new BMenuBar(BRect(0,0,100,30),"main_MainFrameMenuBar",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_ITEMS_IN_ROW,true);
}

alQAWin::alQAWin ( alHandler *TH, float width, float height, const char *Name ):alNormalWindow(TH,width,height,Name)
{
MainFrameMenu = new BMenuBar(BRect(0,0,100,30),"main_MainFrameMenuBar",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_ITEMS_IN_ROW,true);
}

alQAWin::alQAWin ( alHandler *TH, const char *Name ):alNormalWindow(TH,Name)
{
MainFrameMenu = new BMenuBar(BRect(0,0,100,30),"main_MainFrameMenuBar",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_ITEMS_IN_ROW,true);
}



alQAWin::~alQAWin()
{
if (MainFrameMenu)
			{
			MainFrameMenu->RemoveSelf();
			delete MainFrameMenu;
			}
}

BMenu *alQAWin::AddMenuToFrame(const char *title)
{
BMenu *addmenu;
addmenu = new BMenu(title);

MainFrameMenu->AddItem(addmenu);
return addmenu;
}

BMenu *alQAWin::AddMenuToMenu(const char *title, BMenu *parent_menu)
{
BMenu *addmenu;
addmenu = new BMenu(title);

parent_menu->AddItem(addmenu);
return addmenu;
}


BMenuItem *alQAWin::AddItemToMenu(const char *title, BMessage *action_mes, BMenu *parent_menu)
{
BMenuItem *additem;
additem = new BMenuItem(title, (BMessage *) action_mes, 0,0);

parent_menu->AddItem(additem);
return additem;
}

void alQAWin::ShowMenu()
{
AddChild(MainFrameMenu);
}















alQuickApp::alQuickApp(alHandler *TH, const char *name, const char *wintitle, float x1 , float y1, float x2, float y2):BApplication(name)
{
AppMainFrame = new alQAWin(TH,x1,y1,x2,y2,wintitle);
initialized = true;
if (TH) { AnyMessageHandler = TH; } else {initialized=false;}

entry_ref *pfad;
app_info info;
BPath path;
									
this->GetAppInfo( &info );
pfad = &info.ref;
									
BEntry appfile( pfad, false);
appfile.GetPath(&path);
MyFilePosition = new BPath(path);
current_cursor = NULL;
}

alQuickApp::alQuickApp(alHandler *TH, const char *name, const char *wintitle, float width , float height):BApplication(name)
{
AppMainFrame = new alQAWin(TH,width,height,wintitle);
initialized = true;
if (TH) { AnyMessageHandler = TH; } else {initialized=false;}

entry_ref *pfad;
app_info info;
BPath path;
									
this->GetAppInfo( &info );
pfad = &info.ref;
									
BEntry appfile( pfad, false);
appfile.GetPath(&path);
MyFilePosition = new BPath(path);
current_cursor = NULL;
}

alQuickApp::alQuickApp(alHandler *TH, const char *name, const char *wintitle):BApplication(name)
{
AppMainFrame = new alQAWin(TH,wintitle);
initialized = true;
if (TH) { AnyMessageHandler = TH; } else {initialized=false;}

entry_ref *pfad;
app_info info;
BPath path;
									
this->GetAppInfo( &info );
pfad = &info.ref;
									
BEntry appfile( pfad, false);
appfile.GetPath(&path);
MyFilePosition = new BPath(path);

current_cursor = NULL;
}

alQuickApp::alQuickApp(alHandler *TH, const char *name):BApplication(name)
{
AppMainFrame = (alQAWin *) NULL;
initialized = true;

if (TH) { AnyMessageHandler = TH; } else {initialized=false;return;}
entry_ref *pfad;
app_info info;
BPath path;
									
this->GetAppInfo( &info );
pfad = &info.ref;
									
BEntry appfile( pfad, false);
appfile.GetPath(&path);
MyFilePosition = new BPath(path);
current_cursor = NULL;
}


alQuickApp::~alQuickApp()
{
if (initialized = true) {
AnyMessageHandler->HandInMessage( new BMessage(CONST_AppWantsToQuit));
						}

if (AppMainFrame) {
					AppMainFrame->CloseWin();						
					delete AppMainFrame;
				  }
}

void alQuickApp::MessageReceived ( BMessage *message )
{
if (initialized = true) {
AnyMessageHandler->HandInMessage(message );
						}
}

void alQuickApp::RefsReceived ( BMessage *message)
{
if (initialized = true) {
AnyMessageHandler->HandInMessage(message );
						}
}

void alQuickApp::ReadyToRun()
{
unsigned char cursor_arrow[70] = {16,1,0,0,
128,0,224,0,152,0,140,0,134,0,131,128,128,192,64,32,64,96,64,128,64,128,78,64,113,64,1,32,0,144,0,80,
128,0,224,0,248,0,252,0,254,0,255,128,255,192,127,224,127,224,127,128,127,128,127,192,113,192,1,224,0,240,0,112,0,0};

SetCursor(&cursor_arrow);


if (initialized = true) {
AnyMessageHandler->HandInMessage( new BMessage(CONST_AppReadyToRun));
						} else {this->PostMessage(B_QUIT_REQUESTED);}
}

void *alQuickApp::GetActiveWindow()
{
int32 i,k;
BWindow *w;

k = this->CountWindows();
for (i=0; i<k; i++)
	{
	w = this->WindowAt(i);
	
	if (w->IsFront() == true) return w;
	}

return NULL;
}

void alQuickApp::CursorArrow()
{
current_cursor = &extra_arrow[0];
BCursor c(&extra_arrow);
SetCursor(&c);
}

void alQuickApp::CursorBusy()
{
current_cursor = &extra_busy[0];
BCursor c(&extra_busy);
SetCursor(&c);
}

void alQuickApp::CursorCross()
{
current_cursor = &extra_cross[0];
BCursor c(&extra_cross);
SetCursor(&c);
}

void alQuickApp::CursorPen()
{
current_cursor = &extra_pen[0];
BCursor c(&extra_pen);
SetCursor(&c);
}


void alQuickApp::CursorHelp()
{
current_cursor = &extra_help[0];
BCursor c(&extra_help);
SetCursor(&c);
}

void alQuickApp::CheckCursor()
{
if (current_cursor != NULL)
	{
	BCursor c(&current_cursor);
	SetCursor(&c);
	}
}


















alLeftButton::alLeftButton(float x1, float y1, float width, float height, const char *title, BMessage *action):BButton( BRect(x1,y1,x1+width,y1+height),"AnyButton",title, (BMessage *) action,B_FOLLOW_LEFT|B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE)
{
action_message = action;
}

alLeftButton::alLeftButton(float x1, float y1, const char *title, BMessage *action):BButton( BRect(x1,y1,x1+100,y1+20),"AnyButton",title, (BMessage *) action,B_FOLLOW_LEFT|B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE)
{
ResizeToPreferred();
action_message = action;
}

void alLeftButton::ReplaceMessage( BMessage *action )
{
if (action_message) delete action_message;
action_message = action;
}


alLeftButton::~alLeftButton()
{
//if (action_message) delete action_message;
}

void alLeftButton::KeyDown(const char *bytes, int32 numBytes)
{
switch ((int) bytes)
		{
		case B_ENTER:{Invoke ( (BMessage *) action_message );return;}
		case B_SPACE:{Invoke ( (BMessage *) action_message );return;}
		}
		
BButton::KeyDown(bytes,numBytes);
BButton::Draw(Frame());
}
















alRightButton::alRightButton(float x1, float y1, float width, float height, const char *title, BMessage *action):BButton( BRect(x1,y1,x1+width,y1+height),"AnyButton",title, (BMessage *) action,B_FOLLOW_RIGHT|B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE)
{
action_message = action;
}

alRightButton::alRightButton(float x1, float y1, const char *title, BMessage *action):BButton( BRect(x1,y1,x1+100,y1+20),"AnyButton",title, (BMessage *) action,B_FOLLOW_RIGHT|B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE)
{
ResizeToPreferred();
action_message = action;
}

void alRightButton::ReplaceMessage( BMessage *action )
{
if (action_message) delete action_message;
action_message = action;
}


alRightButton::~alRightButton()
{
//if (action_message) delete action_message;
}

void alRightButton::KeyDown(const char *bytes, int32 numBytes)
{
switch ((int) bytes)
		{
		case B_ENTER:{Invoke ( (BMessage *) action_message );return;}
		case B_SPACE:{Invoke ( (BMessage *) action_message );return;}
		}
		
BButton::KeyDown(bytes,numBytes);
BButton::Draw(Frame());

}














alDynButton::alDynButton(float x1, float y1, float x2, float height, const char *title, BMessage *action):BButton( BRect(x1,y1,x2,y1+height),"AnyButton",title, (BMessage *) action,B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP, B_FRAME_EVENTS | B_WILL_DRAW| B_NAVIGABLE)
{
action_message = action;
}

void alDynButton::ReplaceMessage( BMessage *action )
{
if (action_message) delete action_message;
action_message = action;
}


alDynButton::~alDynButton()
{
//if (action_message) {delete action_message;}
}

void alDynButton::KeyDown(const char *bytes, int32 numBytes)
{
switch ((int) bytes)
		{
		case B_ENTER:{Invoke ( (BMessage *) action_message );return;}
		case B_SPACE:{Invoke ( (BMessage *) action_message );return;}
		}

BButton::KeyDown(bytes,numBytes);
BButton::Draw(Frame());
}
















alHButtonBar::alHButtonBar ( float x1, float y1, float width ):BView(BRect(x1,y1,x1+width,y1+30),"alHButtonBar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_FRAME_EVENTS )
{
maxx = 0;
}

void alHButtonBar::AddButton ( const char *title, BMessage *message )
{
alLeftButton *button;
button = new alLeftButton(maxx+1,1,title, message);
maxx += (int) button->Frame().Width() + 10;
if (this->Frame().Height() < button->Frame().Height())
				{
				this->ResizeTo (this->Frame().Width(),button->Frame().Height());
				}
BView::AddChild(button, NULL);				
}

void alHButtonBar::AddChild( BView *none_accepted, BView *before = NULL)
{

}

alHButtonBar::~alHButtonBar()
{
}








alVButtonBar::alVButtonBar ( float x1, float y1, float width, float height ):BView(BRect(x1,y1,x1+width,y1+height),"alVButtonBar",B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, B_FRAME_EVENTS | B_NAVIGABLE)
{
maxy=0;
}

void alVButtonBar::AddButton ( const char *title, BMessage *message )
{
alLeftButton *button;
button = new alLeftButton(1,1+maxy,title, message);
button->ResizeTo(this->Frame().Width(), 20);
maxy += (int) button->Frame().Height() + 10;
BView::AddChild(button, NULL);				
}

void alVButtonBar::AddChild( BView *none_accepted, BView *before = NULL)
{

}

alVButtonBar::~alVButtonBar()
{
}


















int32 extra::OKBox (const char *Output)
{
int32 button_index;
BAlert *myAlert=new BAlert("OKBox",Output,"OK",NULL,NULL,B_WIDTH_AS_USUAL,B_EVEN_SPACING,B_INFO_ALERT);
myAlert->SetShortcut(0,B_ENTER);
button_index = myAlert->Go();
return button_index;
}

int32 extra::YesNoBox (const char *Output)
{
int32 button_index;
BAlert *myAlert=new BAlert("YesNoBox",Output,"Yes","No",NULL,B_WIDTH_AS_USUAL,B_EVEN_SPACING,B_WARNING_ALERT);
myAlert->SetShortcut(0,B_ENTER);
button_index = myAlert->Go();
return button_index;
}

int32 extra::YesNoCancelBox (const char *Output)
{
int32 button_index;
BAlert *myAlert=new BAlert("YesNoCancelBox",Output,"Cancel","No","Yes",B_WIDTH_AS_USUAL,B_OFFSET_SPACING,B_WARNING_ALERT);
myAlert->SetShortcut(2,B_ENTER);
button_index = myAlert->Go();
return button_index;
}


int32 extra::ErrorBox (const char *Output)
{
int32 button_index;
BAlert *myAlert=new BAlert("ErrorBox",Output,"I am warned",NULL,NULL,B_WIDTH_AS_USUAL,B_EVEN_SPACING,B_STOP_ALERT);
myAlert->SetShortcut(0,B_ENTER);
button_index = myAlert->Go();
return button_index;
}


int32 extra::CBox2 ( const char *sel1, const char *sel2 )
{
int32 button_index;
BAlert *myAlert=new BAlert("SelectFrom2Box","Select one of these options!",sel1,sel2,NULL,B_WIDTH_AS_USUAL,B_EVEN_SPACING,B_IDEA_ALERT);
myAlert->SetShortcut(0,B_ENTER);
button_index = myAlert->Go();
return button_index;
}


int32 extra::CBox3 ( const char *sel1, const char *sel2 , const char *sel3)
{
int32 button_index;
BAlert *myAlert=new BAlert("SelectFrom3Box","Select one of these options!",sel1,sel2,sel3,B_WIDTH_AS_USUAL,B_EVEN_SPACING,B_IDEA_ALERT);
myAlert->SetShortcut(0,B_ENTER);
button_index = myAlert->Go();
return button_index;
}


















alHPictureBar::alHPictureBar(float xi, float yi, alNormalWindow *anywin)
{
maximum = 0;
x = xi;
y = yi;
Window = anywin;
}

alHPictureBar::~alHPictureBar()
{
Window = NULL;
}

alSwitchButton *alHPictureBar::AddSwitch(const char *f1, const char *f2, BMessage *on, BMessage *off)
{
alSwitchButton *anyknopf;
anyknopf = new alSwitchButton(x,y,f1,f2,on,off);
x += anyknopf->Frame().Width()+2;
Window->AddChild(anyknopf);
return anyknopf;
}

alBitmapButton *alHPictureBar::AddButton(const char *f1, const char *f2, BMessage *activated)
{
alBitmapButton *anyknopf;
anyknopf = new alBitmapButton(x,y,f1,f2,activated);
x += anyknopf->Frame().Width()+2;
Window->AddChild(anyknopf);
return anyknopf;
}

alSwitchButton *alHPictureBar::AddSwitch(BBitmap *f1, BBitmap *f2, BMessage *on, BMessage *off)
{
alSwitchButton *anyknopf;
anyknopf = new alSwitchButton(x,y,f1,f2,on,off);
x += anyknopf->Frame().Width()+2;
Window->AddChild(anyknopf);
return anyknopf;
}

alBitmapButton *alHPictureBar::AddButton(BBitmap *f1, BBitmap *f2, BMessage *activated)
{
alBitmapButton *anyknopf;
anyknopf = new alBitmapButton(x,y,f1,f2,activated);
x += anyknopf->Frame().Width()+2;
Window->AddChild(anyknopf);
return anyknopf;
}















alVPictureBar::alVPictureBar(float xi, float yi, alNormalWindow *anywin)
{
maximum = 0;
x = xi;
y = yi;
Window = anywin;
}

alVPictureBar::~alVPictureBar()
{
Window = NULL;
}


alSwitchButton *alVPictureBar::AddSwitch(const char *f1, const char *f2, BMessage *on, BMessage *off)
{
alSwitchButton *anyknopf;
anyknopf = new alSwitchButton(x,y,f1,f2,on,off);
y += anyknopf->Frame().Height()+2;
Window->AddChild(anyknopf);
return anyknopf;
}

alBitmapButton *alVPictureBar::AddButton(const char *f1, const char *f2, BMessage *activated)
{
alBitmapButton *anyknopf;
anyknopf = new alBitmapButton(x,y,f1,f2,activated);
y += anyknopf->Frame().Height()+2;
Window->AddChild(anyknopf);
return anyknopf;
}

alSwitchButton *alVPictureBar::AddSwitch(BBitmap *f1, BBitmap *f2, BMessage *on, BMessage *off)
{
alSwitchButton *anyknopf;
anyknopf = new alSwitchButton(x,y,f1,f2,on,off);
y += anyknopf->Frame().Height()+2;
Window->AddChild(anyknopf);
return anyknopf;
}

alBitmapButton *alVPictureBar::AddButton(BBitmap *f1, BBitmap *f2, BMessage *activated)
{
alBitmapButton *anyknopf;
anyknopf = new alBitmapButton(x,y,f1,f2,activated);
y += anyknopf->Frame().Height()+2;
Window->AddChild(anyknopf);
return anyknopf;
}











alBitmapButton::alBitmapButton( float x, float y, const char *f1, const char *f2, BMessage *activated):BControl( BRect(x,y,x+25,y+25), "alBitmapButton", "alBitmapButton", activated, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW)
{
SetValue(0);
B1 = extra::FetchBitmap(f1);

if (B1 != NULL) ResizeTo(B1->Bounds().Width()+6,B1->Bounds().Height()+6);
		else ResizeTo(25,25);

B2 = extra::FetchBitmap(f2);
Nachricht = activated;

backcolor.red = 255;
backcolor.green = 255;
backcolor.blue = 255;
backcolor.alpha = 255;
}


alBitmapButton::alBitmapButton( float x, float y, BBitmap *f1, BBitmap *f2, BMessage *activated):BControl( BRect(x,y,x+25,y+25), "alBitmapButton", "alBitmapButton", activated, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW)
{
SetValue(0);
B1 = new BBitmap(f1);

if (B1 != NULL) ResizeTo(B1->Bounds().Width()+6,B1->Bounds().Height()+6);
		else ResizeTo(25,25);

B2 = new BBitmap(f2);
Nachricht = activated;

backcolor.red = 255;
backcolor.green = 255;
backcolor.blue = 255;
backcolor.alpha = 255;
}


alBitmapButton::~alBitmapButton ()
{
if (B1 != NULL) delete B1;
if (B2 != NULL) delete B2;
//if (Nachricht != NULL) delete Nachricht;
}

void alBitmapButton::MouseDown(BPoint p)
{
if (IsEnabled() == true)
	{
	SetValue(1);
	Draw(BRect(0,0,0,0));
	}
}


void alBitmapButton::MouseUp(BPoint p)
{
if (IsEnabled() == true)
	{
	SetValue(0);
	Draw(BRect(0,0,0,0));
	Invoke(Nachricht);
	}
}


void alBitmapButton::Draw( BRect restriction)
{
Window()->Lock();

SetHighColor(backcolor);
FillRect(BRect(0,0,Frame().Width(),Frame().Height()),B_SOLID_HIGH);

if (IsEnabled() == true)
	{
	SetHighColor(150,150,150,255);
	FillRect(BRect(5,5,Frame().Width(),Frame().Height()),B_SOLID_HIGH);
	}

switch ( Value())
{
case 0:
		{
		SetHighColor(255,255,255,255);
		FillRect(BRect(0,0,Frame().Width()-5,Frame().Height()-5),B_SOLID_HIGH);
		if (B1) DrawBitmap(B1, BPoint(1,1));
		SetHighColor(0,0,0,255);
		StrokeRect(BRect(0,0,Frame().Width()-5,Frame().Height()-5),B_SOLID_HIGH);
		break;
		}
case 1:
		{
		SetHighColor(255,255,255,255);
		FillRect(BRect(3,3,Frame().Width()-2,Frame().Height()-2),B_SOLID_HIGH);
		if (B2) DrawBitmap(B2, BPoint(4,4));
		SetHighColor(0,0,0,255);
		StrokeRect(BRect(3,3,Frame().Width()-2,Frame().Height()-2),B_SOLID_HIGH);		
		break;
		}		
}

Sync();
Window()->Unlock();
}




















alSwitchButton::alSwitchButton( float x, float y, const char *f1, const char *f2, BMessage *N1, BMessage *N2):BControl( BRect(x,y,x+25,y+25), "alBitmapButton", "alBitmapButton", NULL, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW)
{
BRect R(0,0,0,0);
BBitmap *a;

SetValue(0);
a = extra::FetchBitmap(f1);
B1 = a;

R = a->Bounds();
if (B1 != NULL) ResizeTo(R.Width()+6,R.Height()+6); else ResizeTo(25,25);

B2 = extra::FetchBitmap(f2);
this->N1 = N1;
this->N2 = N2;
backcolor.red = 255;
backcolor.green = 255;
backcolor.blue = 255;
backcolor.alpha = 255;
}


alSwitchButton::alSwitchButton( float x, float y, BBitmap *f1, BBitmap *f2, BMessage *N1, BMessage *N2):BControl( BRect(x,y,x+25,y+25), "alBitmapButton", "alBitmapButton", NULL, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW)
{
BRect R(0,0,0,0);
SetValue(0);
B1 = new BBitmap(f1);

R = B1->Bounds();
if (B1 != NULL) ResizeTo(R.Width()+6,R.Height()+6); else ResizeTo(25,25);

B2 = new BBitmap(f2);
this->N1 = N1;
this->N2 = N2;
backcolor.red = 255;
backcolor.green = 255;
backcolor.blue = 255;
backcolor.alpha = 255;
}



alSwitchButton::~alSwitchButton ()
{
if (B1 != NULL) delete B1;
if (B2 != NULL) delete B2;
//if (N1 != NULL) delete N1;
//if (N2 != NULL) delete N2;
}

void alSwitchButton::MouseDown(BPoint p)
{
if (IsEnabled() == true)
	{
	if (Value() == 0)
		{
		SetValue(3);
		Draw(BRect(0,0,0,0));
		Invoke(N1);
		}
	if (Value() == 2)
		{
		SetValue(4);
		Draw(BRect(0,0,0,0));
		Invoke(N2);
		}
	}
		
}


void alSwitchButton::MouseUp(BPoint p)
{
if (Value() == 3) SetValue(2);
if (Value() == 4) SetValue(0);

Draw(BRect(0,0,0,0));
}


void alSwitchButton::Draw( BRect restriction)
{
Window()->Lock();

SetHighColor(backcolor);
FillRect(BRect(0,0,Frame().Width(),Frame().Height()),B_SOLID_HIGH);

if (IsEnabled() == true)
	{
	SetHighColor(150,150,150,255);
	FillRect(BRect(5,5,Frame().Width(),Frame().Height()),B_SOLID_HIGH);
	}

switch ( Value())
{
case 0:
		{
		SetHighColor(255,255,255,255);
		FillRect(BRect(0,0,Frame().Width()-5,Frame().Height()-5),B_SOLID_HIGH);
		if (B1) DrawBitmap(B1, BPoint(1,1));
		SetHighColor(0,0,0,255);
		StrokeRect(BRect(0,0,Frame().Width()-5,Frame().Height()-5),B_SOLID_HIGH);
		break;
		}
case 2:		
		{
		SetHighColor(255,255,255,255);
		FillRect(BRect(3,3,Frame().Width()-2,Frame().Height()-2),B_SOLID_HIGH);
		if (B2) DrawBitmap(B2, BPoint(4,4));
		SetHighColor(0,0,0,255);
		StrokeRect(BRect(3,3,Frame().Width()-2,Frame().Height()-2),B_SOLID_HIGH);		
		break;
		}		
case 3:		
case 4:
		{
		SetHighColor(255,255,255,255);
		FillRect(BRect(4,4,Frame().Width()-2,Frame().Height()-2),B_SOLID_HIGH);
		if (B2) DrawBitmap(B2, BPoint(5,5));
		SetHighColor(0,0,0,255);
		StrokeRect(BRect(4,4,Frame().Width()-1,Frame().Height()-1),B_SOLID_HIGH);		
		break;
		}		
}

Sync();
Window()->Unlock();
}


















alTreeViewOBJ::alTreeViewOBJ(BLooper *TargetLooper):BView(BRect(5,30,50,100),"ProjectTreeView", B_FOLLOW_NONE,B_WILL_DRAW | B_NAVIGABLE)
{
LooperForMessages = TargetLooper;
TreeData = new BList(1);
TreeName = new BString ("New Tree");
Sel=-1;
width=45;
height=70;
}

alTreeViewOBJ::alTreeViewOBJ(BLooper *TargetLooper, float x1,float y1,float x2,float y2):BView(BRect(x1,y1,x2,y2),"ProjectTreeView", B_FOLLOW_NONE,B_WILL_DRAW | B_NAVIGABLE)
{
LooperForMessages = TargetLooper;
TreeData = new BList(1);
TreeName = new BString ("New Tree");
Sel = 1;
width=(long) (x2 - x1);
height = (long) (y2 - y1);
}


alTreeViewOBJ::~alTreeViewOBJ()
{
delete TreeData;
delete TreeName;
}

void alTreeViewOBJ::DrawNeccessary()
{
BRect R;
escapement_delta delta;
long f;
BRegion Region;
BPoint Punkt;
int i,k;
int c,d,e;
rgb_color farbe;


Window()->Lock();


R = Bounds();
f = TreeData->CountItems();
f= (f * 20) + 30;
if  (f > height) { height = f;}
if (height < R.Height()) { height = (long) R.Height();}


if (IsFocus()) {

/*i=Sel;
Region.Set(BRect(0,(i*20) +15, R.Width() ,(i*20)+35));
i=oldSel;
Region.Include(BRect(0,(i*20) +15, R.Width() ,(i*20)+35));*/
i=overfloat;
Region.Set(BRect(0,(i*20) +15, R.Width() ,(i*20)+35));
i=oldoverfloat;
Region.Include(BRect(0,(i*20) +15, R.Width() ,(i*20)+35));

this->ConstrainClippingRegion( &Region);

R= Frame();

SetPenSize(1);
SetHighColor(255,255,255,20);
FillRect(BRect(1,21,width-1 ,height-1), B_SOLID_HIGH);
FillRect(BRect(0,0, width , 19), B_SOLID_HIGH);

if 	((Sel >= 0) && (Sel <= TreeData->CountItems())){
		SetHighColor(0,0,250,255);		
		FillRect(BRect(0,(Sel*20) +15, R.Width() ,(Sel*20)+35), B_SOLID_HIGH);
		}

delta.nonspace = 10;
delta.space = 10;

SetHighColor(000,000,000,255);
SetFontSize(12);
DrawString(TreeName->String(), BPoint(0,14) , &delta);

int32 a=0;
BString *s;
BString *s_part;
BString *s_function;

s_part= new BString("nothing");
s_function = new BString("none");

k = TreeData->CountItems();

for (i=0;i<k;i++)
	{
	s =	(BString *) TreeData->ItemAt(i);
	if (s != NULL) 
		{
		s_function->SetTo ( s->ByteAt(1),1 );
		s_part->SetTo ( s->String() );
		s_part->Remove(0,2); //Funktion ermitteln und lösche Funktionsbytes
		a = s_part->FindLast("*");
		s_part->Remove (0, a +1 ); //Treeposition ermitteln und die Bytes löschen
		
					
		d = k;
		for (c=i+1; c<k; c++)
		{
		s = (BString *) TreeData->ItemAt(c);
		e = s->FindLast("*") - 2;		
			if (a >= e) break;
		}
		
		SetHighColor(0,0,0,255);				
		StrokeLine( BPoint((a-1) * 30 + 5 , (i*20)+20), BPoint((a-1) * 30 + 5, (c*20)), B_SOLID_HIGH );

		StrokeLine ( BPoint((a-2) * 30 + 5 , (i*20)+20), BPoint((a-1) * 30 + 5 , (i*20)+20), B_SOLID_HIGH);
		FillEllipse ( BRect((a-1) * 30, (i*20)+20 ,((a-1) * 30)+ 10,(i*20)+30), B_SOLID_HIGH) ;
		
		this->SetFont ( be_plain_font );
		SetFontSize(10);
		SetHighColor(0,0,0,255);				
		
if (overfloat == i)
		{
		this->SetFont ( be_bold_font );
		SetFontSize(12);
		} 
		
if 	(Sel == i){
		farbe = ViewColor();
		SetViewColor(0,0,255,255);			
		}
		
		if (Sel == i) {SetHighColor(255,255,255,255); }		
		
		SetDrawingMode(B_OP_OVER);
		DrawString ( s_part->String(), BPoint(((a-1) * 30) + 15, i*20 + 30), &delta );		
		SetDrawingMode(B_OP_COPY);
		
if (Sel == i)
		{
		SetViewColor(farbe);	
		}

		}
	}
	
SetHighColor(000,000,000,255);
StrokeRect(BRect(0,20,width ,height), B_SOLID_HIGH);	

Sync();
Window()->Unlock();

delete s_part;
delete s_function;

R = Bounds();
Punkt.Set(width , height);
R.SetRightBottom(Punkt);
Punkt.Set(0 , 0);
R.SetLeftTop(Punkt);
Region.Set(R);
ConstrainClippingRegion(&Region);
}
}

void alTreeViewOBJ::Draw(BRect visRect)
{
BRect R;
escapement_delta delta;
long f;


Window()->Lock();
R = Bounds();
f = TreeData->CountItems();
f= (f * 20) + 30;
if  (f > height) { height = f;}
if (height < R.Height()) { height = (long) R.Height();}



R= Frame();

SetPenSize(1);
SetHighColor(255,255,255,20);
FillRect(BRect(1,21,width-1 ,height-1), B_SOLID_HIGH);
FillRect(BRect(0,0, width , 19), B_SOLID_HIGH);

if 	((Sel >= 0) && (Sel <= TreeData->CountItems())){
		SetHighColor(0,0,250,255);		
		FillRect(BRect(0,(Sel*20) +15, R.Width() ,(Sel*20)+35), B_SOLID_HIGH);
		}

delta.nonspace = 10;
delta.space = 0;

SetHighColor(000,000,000,255);
SetFontSize(12);
DrawString(TreeName->String(), BPoint(0,14) , &delta);

int i,k;
int c,d,e;

int32 a=0;
BString *s;
BString *s_part;
BString *s_function;

s_part= new BString("nothing");
s_function = new BString("none");

k = TreeData->CountItems();

SetFontSize(10);
for (i=0;i<k;i++)
	{
	s =	(BString *) TreeData->ItemAt(i);
	if (s != NULL) 
		{
		s_function->SetTo ( s->ByteAt(1),1 );
		s_part->SetTo ( s->String() );
		s_part->Remove(0,2); //Funktion ermitteln und lösche Funktionsbytes
		a = s_part->FindLast("*");
		s_part->Remove (0, a +1 ); //Treeposition ermitteln und die Bytes löschen
		
					
		d = k;
		for (c=i+1; c<k; c++)
		{
		s = (BString *) TreeData->ItemAt(c);
		e = s->FindLast("*") - 2;		
			if (a >= e) break;
		}
		
		SetHighColor(0,0,0,255);				
		StrokeLine( BPoint((a-1) * 30 + 5 , (i*20)+20), BPoint((a-1) * 30 + 5, (c*20)), B_SOLID_HIGH );

		StrokeLine ( BPoint((a-2) * 30 + 5 , (i*20)+20), BPoint((a-1) * 30 + 5 , (i*20)+20), B_SOLID_HIGH);
		FillEllipse ( BRect((a-1) * 30, (i*20)+20 ,((a-1) * 30)+ 10,(i*20)+30), B_SOLID_HIGH) ;
		
		this->SetFont ( be_plain_font );
if (overfloat == i)
		{
		this->SetFont ( be_bold_font );
		} 
		
		if (Sel == i) {SetHighColor(255,255,255,255); }
		
		SetDrawingMode(B_OP_OVER);
		DrawString ( s_part->String(), BPoint(((a-1) * 30) + 15, i*20 + 30), &delta );		
		SetDrawingMode(B_OP_COPY);
		}
	}
	
SetHighColor(000,000,000,255);
StrokeRect(BRect(0,20,width ,height), B_SOLID_HIGH);	
Sync();
Window()->Unlock();

delete s_part;
delete s_function;


}

void alTreeViewOBJ::Refresh()
{
Draw ( BRect (0,0,1,1) );
}

void alTreeViewOBJ::MouseDown(BPoint point)
{
int32 y;

if (IsFocus())
{
oldSel = Sel;
y = (int) point.y;
Sel = (y-15) / 20;
}

if (Sel != oldSel) Refresh();

this->MakeFocus(true);
}

void alTreeViewOBJ::MouseMoved(BPoint point, uint32 transit,const BMessage *message)
{
int32 y;

oldoverfloat = overfloat;
y = (int) point.y;
overfloat = (y-15) / 20;

if (IsFocus())
{
if (overfloat != oldoverfloat) DrawNeccessary();
}
}


void alTreeViewOBJ::AttachedToWindow(void)
{
this->MakeFocus(true);
Refresh();
}




















//Constructor
alResourceFile::alResourceFile(const char *resource_filename, int32 type, int32 ID)
{
BFile exe (resource_filename, B_READ_ONLY);
BResources res;
status_t err;
size_t i;
char *tempz, *z;
memspace = 0;


if ((err = res.SetTo(&exe, false)) == B_OK) 
	{
	tempz =(char *)  res.LoadResource(type,ID, &size);		
	if (size > 0)
		{
		i = (size / 4096) + 1;
		ma = create_area("alResourceFile area", &zeiger, B_ANY_ADDRESS, B_PAGE_SIZE * i, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA );
		if (ma >= 0)
					{
					memspace = B_PAGE_SIZE * i;
					z = (char *) zeiger;
					
					for (i=0; i < size; i++)
						{
						*(z + i) = *(tempz + i ); 						
						}					
					}
		}
	}
position = 0;

if (zeiger == NULL) delete this;
}

//Destructor
alResourceFile::~alResourceFile()
{
if (memspace != 0) delete_area(ma);
}


ssize_t alResourceFile::Read(void *buffer, size_t numBytes)
{
size_t i,copied;
char *p, *z;
char data;

if (buffer == NULL) return -1;

z = (char *) zeiger;
p = (char *) buffer;
copied = 0;

for (i=position; i < position + numBytes; i++)
	{
	if (i < size)	
				{
				data = *(z + i);
				*(p + copied) = data;
				copied++;
				}
	}

position += copied;
return (ssize_t) copied;
}

ssize_t alResourceFile::ReadAt(off_t posx, void *buffer, size_t numBytes)
{
position = posx;
return Read(buffer, numBytes);
}

off_t alResourceFile::Seek(off_t posx, uint32 mode)
{
switch (mode)
	{
	case SEEK_SET: { position = posx; break; }
	case SEEK_CUR: { position += posx; 
					 if (position > size) position = size;
					 break; }
	case SEEK_END: { position = size; break; }
	}
return position;
}

off_t alResourceFile::Position(void) const
{
return position;
}

ssize_t alResourceFile::WriteAt(off_t posx, const void *buffer, size_t numBytes)
{
size_t i,copied;
char *p, *z;
char data;

z = (char *) zeiger;
p = (char *) buffer;
copied = 0;

for (i=position; i < position + numBytes; i++)
	{
	if (i < size)	
				{
				data = *(p + copied);
				*(z + i) = data;
				copied++;
				}
	}

position += copied;
return (ssize_t) copied;

}


























BBitmap *extra::GetResourceBitmap(const char *file, int32 type, int32 ID )
{

alResourceFile *picture;
/*BTranslatorRoster *roster = BTranslatorRoster::Default();
BBitmapStream stream;
BBitmap *erg = NULL;*/

picture = new alResourceFile(file,type, ID);
if (picture == NULL) return NULL;

/*
if (roster->Translate(picture,NULL,NULL, &stream, B_TRANSLATOR_BITMAP ) != B_OK) return NULL;
stream.DetachBitmap(&erg);
return erg;*/

return BTranslationUtils::GetBitmap ( picture , NULL);
}

void extra::WaitMilisecs( int32 wait )
{
//bigtime_t first,test;

wait *= 1000;
snooze(wait);
/*first = real_time_clock_usecs();
test=0;

while (wait > test)
	{
	test = real_time_clock_usecs() - first;
	} 
*/
}

BBitmap *extra::FetchBitmap(const char *filename)
{ 
BFile file(filename, B_READ_ONLY);
BTranslatorRoster *roster = BTranslatorRoster::Default();
BBitmapStream stream;
BBitmap *result = NULL;
if(roster->Translate(&file,NULL,NULL,&stream,B_TRANSLATOR_BITMAP)<B_OK) return NULL; 
stream.DetachBitmap(&result);
return result;
}


BString extra::StringOutOf(int what)
{
BString erg("");
long testarr[10] = { 1 , 10 , 100 , 1000 , 10000 , 100000 , 1000000 , 10000000 , 100000000 , 1000000000 };

bool startaddzeros;
long i,c,min;

min=0;
startaddzeros = false;

if (what < 0) {erg.SetTo("-"); what = 0 - what;}

for (i=9; i>=0; i--)
{
c = what / testarr[i];
c -= min;

switch (c)
	{
	case 0: {
			if (startaddzeros == true) erg.Append("0");
			break;}
	case 1: {startaddzeros=true; erg.Append("1");break;}
	case 2: {startaddzeros=true;erg.Append("2");break;}
	case 3: {startaddzeros=true;erg.Append("3");break;}
	case 4: {startaddzeros=true;erg.Append("4");break;}
	case 5: {startaddzeros=true;erg.Append("5");break;}
	case 6: {startaddzeros=true;erg.Append("6");break;}
	case 7: {startaddzeros=true;erg.Append("7");break;}
	case 8: {startaddzeros=true;erg.Append("8");break;}
	case 9: {startaddzeros=true;erg.Append("9");break;}
	}
min += c;	
min *= 10;
}

if (startaddzeros == false) erg.SetTo("0");
return erg;
}








BString extra::fStringOutOf(float what, int dez)
{
BString erg("");
float testarr[10] = { 0.1 , 0.01 , 0.001 , 0.0001 , 0.00001 , 0.000001 , 0.0000001 , 0.00000001 , 0.000000001 , 0.0000000001 };

long i,c;
float min;

c = (long) what;
if (what < 0) {what = 0 - what;}

erg.SetTo( StringOutOf(c) );
erg.Append(".");
what -= c;

c=(long) fabs(c);

min=0;
dez -= 1;


for (i=0; i<=dez; i++)
{
c = (long) (what / testarr[i]);
c -= (long) min;

switch (c)
	{
	case 0: {erg.Append("0");break;}
	case 1: {erg.Append("1");break;}
	case 2: {erg.Append("2");break;}
	case 3: {erg.Append("3");break;}
	case 4: {erg.Append("4");break;}
	case 5: {erg.Append("5");break;}
	case 6: {erg.Append("6");break;}
	case 7: {erg.Append("7");break;}
	case 8: {erg.Append("8");break;}
	case 9: {erg.Append("9");break;}
	}
min += c;	
min *= 10;
}

return erg;
}


void extra::ShowFileError( status_t *result)
{
BString msg("File error occured during a write/read operation: ");

if (*result == B_BAD_VALUE) msg.Append("Bad value");
if (*result == B_BUSY) msg.Append("Busy node could not be accessed");
if (*result == B_ENTRY_NOT_FOUND) msg.Append("Entry not found");
if (*result == B_FILE_ERROR) msg.Append("File system error");
if (*result == B_FILE_EXISTS) msg.Append("File already exists");
if (*result == B_LINK_LIMIT) msg.Append("Cyclic loop of links detected");
if (*result == B_NAME_TOO_LONG) msg.Append("Name too long");
if (*result == B_NO_MEMORY) msg.Append("Operation failed due lack of memory");
if (*result == B_NO_MORE_FDS) msg.Append("I looked around and I couldn´t find any file descriptors");
if (*result == B_IS_A_DIRECTORY) msg.Append("File is Directory?");
if (*result == B_NOT_A_DIRECTORY) msg.Append("Component in the path structure is not a directory");
if (*result == B_NOT_ALLOWED) msg.Append("This medium is read only!");
if (*result == B_PERMISSION_DENIED) msg.Append("This path is difficult, it doesn´t want you on its property.");
}
























alQuickView::alQuickView(float x1, float y1, float x2, float y2):BView(BRect(x1,y1,x2,y2),"alQuickView",B_FOLLOW_NONE, B_NAVIGABLE | B_WILL_DRAW)
{
DoDraw = false;
DoMouseDown = false;
DoMouseUp = false;
DoKeyDown = false;
DoKeyUp = false;
}

void alQuickView::Draw (BRect restricted)
{
if (DoDraw == true) userDraw(this);
}

void alQuickView::MouseDown( BPoint p )
{
if (DoMouseDown == true) userMouseDown(this,p);
}

void alQuickView::MouseUp( BPoint p)
{
if (DoMouseUp == true) userMouseUp(this,p);
}

void alQuickView::KeyDown( const char *what, int32 code)
{
if (DoKeyDown == true) userKeyDown(this, what, code);
}

void alQuickView::KeyUp ( const char *what, int32 code)
{
if (DoKeyUp == true) userKeyUp(this, what, code);
}

void alQuickView::Refresh()
{
if (DoDraw == true) userDraw(this);
}




















alLooper::alLooper(func Function,const char *thread_name, int32 priority):BLooper(thread_name, priority, B_LOOPER_PORT_DEFAULT_CAPACITY)
{
myHandler = new alHandler();
myHandler->MessageReceived = Function;
myHandler->Parent = (char *) this;
}

alLooper::~alLooper()
{
delete myHandler;
}

void alLooper::MessageReceived(BMessage *message)
{
myHandler->HandInMessage( message );					   
BLooper::MessageReceived (message);	
}

void alLooper::SetPriority( int32 p)
{
thread_id id;
id = Thread();

if (p > 120) p = 120;
if (p < 1) p = 1;

set_thread_priority(id,p);
}



















alControl::alControl(float x1,float y1, float x2,float y2, const char *name, const char *label, BMessage *message, uint32 resMode, uint32 flags):BControl(BRect(x1,y1,x2,y2),name,label,message,resMode, flags)
{
showagain = true;
tooltipstyle = CONST_TTNotShow;
tooltipinited = false;
//floater = NULL;
x_minRange = 0;
x_maxRange = 0;
y_minRange = 0;
y_maxRange = 0;
HasScroller = false;
isattached = false;
custom_cursor = NULL;
}

alControl::~alControl()
{

}



void alControl::ActivateToolTip(const char *tip)
{
if (tooltipinited)
	{
	delete ToolTipText;
	if (showagain == false) delete floater;
	}

ToolTipText = new BString (tip);
tooltipinited = true;
showagain = true;
CheckToolTip(2,BPoint(0,0));
}


void alControl::DeactivateToolTip()
{
tooltipinited = false;
delete floater;
}


void alControl::SetToolTipStyle( int style )
{
tooltipstyle = style;
}


void alControl::CheckBars()
{
if (HasScroller)
	{
	Parent->UpdateScrolls();
	}
}



void alControl::CheckToolTip (int transit, BPoint p)
{
if (IsAttached()==false) return;
if (Window()->IsActive() == false) return;


p = ConvertToScreen(p);
if ((transit==2) && (showagain == false)) 
			{
			delete floater;
			showagain = true;
			}
			
if ((transit==1) && (showagain == true))
	{
	if (tooltipinited)
		{
		showagain = false;
		floater = new alToolTip(ToolTipText,tooltipstyle,p);
		floater->TTWindow->AddToSubset(Window());
		floater->TTWindow->Show();
		}
	}
}

void alControl::MouseMoved(BPoint p, uint32 transit, const BMessage *message)
{
if (transit != oldtransit)
	{
	CheckToolTip(transit,p);
	if (custom_cursor != NULL) SetViewCursor(custom_cursor,false);
	oldtransit = transit;
	}
}

void alControl::MakeAttached()
{
isattached = true;
}

void alControl::SetCursor(void *buf)
{
custom_cursor = new BCursor(buf);
}

float alControl::GetScrollX()
{
float x;
x=0;
if (HasScroller)
	{
	if (Parent->HasHorizontal)
		{
		x = ScrollBar(B_HORIZONTAL)->Value();
		}
	}
return x;
}

float alControl::GetScrollY()
{
float y;
y=0;
if (HasScroller)
	{
	if (Parent->HasVertical)
		{
		y = ScrollBar(B_VERTICAL)->Value();
		}
	}
return y;
}



void alControl::NewHScrollValue( float value)
{
//Draw(BRect(0,0,0,0));
}

void alControl::NewVScrollValue (float value)
{
//Draw(BRect(0,0,0,0));
}


















alScrollControl::alScrollControl(alControl *target,BView *ANY, bool horizontal, bool vertical)
{
anyparentview = ANY;
anywindow = NULL;
anyview = target;
target->Parent = this;
target->HasScroller = true;
HasHorizontal = horizontal;
HasVertical = vertical;

float x,y,width,height;


if (HasHorizontal)
	{
	x=target->Frame().LeftBottom().x;
	y=target->Frame().LeftBottom().y+1;
	width = target->Frame().Width();
	height = B_H_SCROLL_BAR_HEIGHT;
	hBar = new CustomScroll(this,BRect(x,y,x+width,y+height),"no name",target,target->x_minRange,target->x_maxRange,B_HORIZONTAL);
	hBar->SetRange(target->x_minRange,target->x_maxRange);
	anyparentview->AddChild(hBar);
	}
	
if (HasVertical)
	{
	x=target->Frame().RightTop().x+1;
	y=target->Frame().RightTop().y;
	width = B_V_SCROLL_BAR_WIDTH;
	height = target->Frame().Height();
	vBar = new CustomScroll(this,BRect(x,y,x+width,y+height),"no name",target,target->y_minRange,target->y_maxRange,B_VERTICAL);
	vBar->SetRange(target->y_minRange,target->y_maxRange);
	anyparentview->AddChild(vBar);
	}
}

alScrollControl::alScrollControl(alControl *target, bool horizontal, bool vertical)
{
anywindow = target->Window();
anyparentview = NULL;
anywindow->Lock();
anyview = target;
target->Parent = this;
target->HasScroller = true;
HasHorizontal = horizontal;
HasVertical = vertical;

float x,y,width,height;


if (HasHorizontal)
	{
	x=target->Frame().LeftBottom().x;
	y=target->Frame().LeftBottom().y+1;
	width = target->Frame().Width();
	height = B_H_SCROLL_BAR_HEIGHT;
	hBar = new CustomScroll(this, BRect(x,y,x+width,y+height),"no name",target,target->x_minRange,target->x_maxRange,B_HORIZONTAL);
	hBar->SetRange(target->x_minRange,target->x_maxRange);
	anywindow->AddChild(hBar);
	}
	
if (HasVertical)
	{
	x=target->Frame().RightTop().x+1;
	y=target->Frame().RightTop().y;
	width = B_V_SCROLL_BAR_WIDTH;
	height = target->Frame().Height();
	vBar = new CustomScroll(this,BRect(x,y,x+width,y+height),"no name",target,target->y_minRange,target->y_maxRange,B_VERTICAL);
	vBar->SetRange(target->y_minRange,target->y_maxRange);
	anywindow->AddChild(vBar);
	}
	
anywindow->Unlock();
}



alScrollControl::~alScrollControl()
{
if (HasHorizontal)
	{
	delete hBar;
	}

if (HasVertical)
	{
	delete vBar;
	}

anyview->Parent=NULL;
anyview->HasScroller = false;
}

void alScrollControl::UpdateScrolls()
{
if (HasHorizontal)
	{
	hBar->SetRange(anyview->x_minRange,anyview->x_maxRange);
	}
	
if (HasVertical)
	{
	vBar->SetRange(anyview->y_minRange, anyview->y_maxRange);
	}
}















void ToolTipHandler ( BMessage *message, alHandler *handler )
{

}

void ToolTipDraw(alQuickView *me)
{
alToolTip *any;
any =(alToolTip *) me->DataServer;
me->SetHighColor(0,0,0,0);
me->DrawString(any->tipx->String(), BPoint(5,any->offset));
}


void KillToolTipOnMouseAction(alQuickView *me,BPoint p)
{
alNormalWindow *x;
x = (alNormalWindow *) me->Window();
x->CloseWin();
}


alToolTip::alToolTip( BString *tip, int style, BPoint p)
{
int assumed_width, assumed_height;
const BFont *xFont;
alHandler *myHandler;

p.y += 20;
p.x += 20;

// init TipX
tipx = tip;

//Get String dimensions
xFont = be_plain_font;
assumed_width = (int) xFont->StringWidth(tip->String());

font_height font_data;
xFont->GetHeight(&font_data);
assumed_height = (int) (font_data.ascent-font_data.descent + font_data.leading * 2);



//Setup window to dispplay that string
int carry = assumed_height / 2;
myHandler = new alHandler( &ToolTipHandler, "ToolTipHandler" );
TTWindow = new alNormalWindow(myHandler,p.x,p.y,p.x+assumed_width+10,p.y+assumed_height,"ToolTip Information");
TTWindow->SetLook(B_BORDERED_WINDOW_LOOK);
TTWindow->SetFeel(B_FLOATING_SUBSET_WINDOW_FEEL);
assumed_height = (int) TTWindow->Frame().Height();
offset = (assumed_height / 2) + carry;

qView = new alQuickView ( 0,0, assumed_width+10, assumed_height );
qView->SetResizingMode( B_FOLLOW_ALL_SIDES);
qView->SetViewColor(200,200,30);
qView->userDraw = &ToolTipDraw;
qView->DoDraw = true;
qView->userMouseDown = &KillToolTipOnMouseAction;
qView->DoMouseDown = true;
qView->DataServer = (void *) this;
qView->SetDrawingMode(B_OP_OVER);

TTWindow->AddChild(qView);
}


alToolTip::~alToolTip()
{
TTWindow->Lock();
qView->RemoveSelf();
delete qView;

alHandler *myHandler;

myHandler = TTWindow->AnyMessageHandler;
TTWindow->CloseWin();
delete myHandler;
}












CustomScroll::CustomScroll(alScrollControl *mom, BRect r,const char *name, BView *any, float min, float max, orientation direction):BScrollBar(r,name,any,min,max,direction)
{
Mutter = mom;
}

CustomScroll::~CustomScroll()
{
RemoveSelf();
}

void CustomScroll::ValueChanged( float value )
{
if ( Orientation() == B_HORIZONTAL) 
		{
		BPoint o;
		o = Mutter->anyview->Origin();
		o.x = -value;
		Mutter->anyview->SetOrigin(o);		
		Mutter->anyview->Invalidate();
		Mutter->anyview->NewHScrollValue( value );
		}

if ( Orientation() == B_VERTICAL) 
		{
		BPoint o;
		o = Mutter->anyview->Origin();
		o.y = -value;
		Mutter->anyview->SetOrigin(o);
		Mutter->anyview->Invalidate();
		Mutter->anyview->NewVScrollValue( value );
		}
}



























//here starts the alBitmap implementation

alBitmap::alBitmap(int width, int height)
{
BitmapData = new unsigned char[width*height*4];
BitmapWidth = width;
BitmapHeight = height;

save_changes = false;
max_saveable_changes = 0;
saved_changes = new BList(0);
originx = 0;
originy = 0;
Aliasing = false;
}


alBitmap::alBitmap(BBitmap *source)
{
int width,height;
width = (int)(source->Bounds().RightTop().x - source->Bounds().LeftTop().x + 1);
height = (int)(source->Bounds().RightBottom().y - source->Bounds().LeftTop().y + 1);
BitmapWidth = width;
BitmapHeight = height;

BitmapData = new unsigned char[width*height*4];

int len = source->BitsLength();
int i;
int *a,*s;
a = (int *) BitmapData;
s = (int *) source->Bits();

for (i=0; i*4 < len; ++i)
	{
	*(a+i) = *(s+i);
	}
	
save_changes = false;
max_saveable_changes = 0;
saved_changes = new BList(0);
originx = 0;
originy = 0;
Aliasing = false;
}

alBitmap::alBitmap(const char *filename)
{
BFile Datei(filename, B_READ_WRITE);
if (Datei.InitCheck() == B_OK)
	{
	BNodeInfo DateiInfo(&Datei);
	if (DateiInfo.InitCheck() == B_OK)
		{
			off_t offset;
			Datei.ReadAttr("BITMAP:WIDTH",B_UINT32_TYPE, offset, &BitmapWidth, 4);
			if (BitmapWidth < 0) BitmapWidth = 1;
			Datei.ReadAttr("BITMAP:HEIGHT",B_UINT32_TYPE, offset, &BitmapHeight, 4);
			if (BitmapHeight < 0) BitmapHeight = 1;			
			
			BitmapData = new unsigned char[BitmapWidth*BitmapHeight*4];
			
			Datei.Read(BitmapData, BitmapWidth*BitmapHeight*4);

		} // DateiInfo.InitCheck()
	} //Datei.InitCheck()

save_changes = false;
max_saveable_changes = 0;
saved_changes = new BList(0);
originx = 0;
originy = 0;
Aliasing = false;
} // method

alBitmap::alBitmap(BFile Datei)
{
status_t stat;


if (Datei.InitCheck() == B_OK)
	{
	BNodeInfo DateiInfo(&Datei);
	if (DateiInfo.InitCheck() == B_OK)
		{
		off_t offset;
		stat = Datei.ReadAttr("BITMAP:WIDTH",B_UINT32_TYPE, offset, &BitmapWidth, 4);
		if (BitmapWidth < 0) BitmapWidth = 1;
		stat = Datei.ReadAttr("BITMAP:HEIGHT",B_UINT32_TYPE, offset, &BitmapHeight, 4);
		if (BitmapHeight < 0) BitmapHeight = 1;
			
		BitmapData = new unsigned char[BitmapWidth*BitmapHeight*4];
			
		Datei.Read(BitmapData, BitmapWidth*BitmapHeight*4);
		} // DateiInfo.InitCheck()
	} //Datei.InitCheck()

save_changes = false;
max_saveable_changes = 0;
saved_changes = new BList(0);
originx = 0;
originy = 0;
Aliasing = false;
} // method


alBitmap::~alBitmap()
{
delete [] BitmapData;
ClearCache();
delete saved_changes;
}

void alBitmap::PaintBitmap(int x, int y, alBitmap *source)
{
unsigned char red,green,blue,alpha;
int i,k,w,h;

if ((x > BitmapWidth) || (y > BitmapHeight)) return;
if ((x < -BitmapWidth) || (y < -BitmapHeight)) return;
w = source->BitmapWidth;
h = source->BitmapHeight;

for (i=0; i < h; ++i)
	for (k=0; k < w; ++k)
		{
		source->GetPixel(k,i, &red , &green, &blue, &alpha);
		SetPixel(k+x, i+y,red,green,blue,alpha);
		}
}

void alBitmap::PaintBitmap(int x, int y, unsigned char alph, alBitmap *source)
{
unsigned char red,green,blue,alpha;
int i,k,w,h,calpha;

if ((x > BitmapWidth) || (y > BitmapHeight)) return;
if ((x < -BitmapWidth) || (y < -BitmapHeight)) return;
w = source->BitmapWidth;
h = source->BitmapHeight;

for (i=0; i < h; ++i)
	for (k=0; k < w; ++k)
		{
		source->GetPixel(k,i, &red , &green, &blue, &alpha);
		calpha = alph + alpha - 255;
		if (calpha < 0) alpha =0; else alpha = (int) calpha;
		SetPixel(k+x, i+y,red,green,blue,alpha);
		}
}


void alBitmap::PaintBitmap(int x, int y, alBitmap *source, BRect constrain)
{
unsigned char red,green,blue,alpha;
int i,k,w,h;

if ((x > BitmapWidth) || (y > BitmapHeight)) return;
if ((x < -BitmapWidth) || (y < -BitmapHeight)) return;
w = source->BitmapWidth;
h = source->BitmapHeight;

for (i=0; i < h; ++i)
	for (k=0; k < w; ++k)
		{
		source->GetPixel(k,i, &red , &green, &blue, &alpha);
		if (constrain.Contains(BPoint(k+x,i+y))) SetPixel(k+x, i+y,red,green,blue,alpha);
		}
}


void alBitmap::PaintBitmap(int x, int y, unsigned char alph, alBitmap *source, BRect constrain)
{
unsigned char red,green,blue,alpha;
int i,k,w,h,calpha;

if ((x > BitmapWidth) || (y > BitmapHeight)) return;
if ((x < -BitmapWidth) || (y < -BitmapHeight)) return;
w = source->BitmapWidth;
h = source->BitmapHeight;

for (i=0; i < h; ++i)
	for (k=0; k < w; ++k)
		{
		source->GetPixel(k,i, &red , &green, &blue, &alpha);
		calpha = alph + alpha - 255;
		if (calpha < 0) alpha =0; else alpha = (int) calpha;
		if (constrain.Contains(BPoint(k+x,i+y))) SetPixel(k+x, i+y,red,green,blue,alpha);
		}
}


void alBitmap::MakeBitmapAlpha(unsigned int alpha)
{
int i,k;
int data,pos;
int *s = (int *) BitmapData;

alpha *= 0x1000000;
for (i = 0; i < BitmapHeight; ++i)
	{
	pos = i*BitmapWidth;
	for (k = 0; k < BitmapWidth; ++k)
		{
		data = *(s+pos);
		data = data & 0x00FFFFFF;
		data = data | alpha;
		*(s+pos) = data;
		++pos;
		}
	}

}


void alBitmap::ClearBitmap()
{
int i;
int *data;

data = (int *) BitmapData;
int len = BitmapWidth * BitmapHeight;

for (i=0; i < len; ++i)
	{
	*(data+i) = 0xFFFFFFFF;
	}
}


void alBitmap::ClearBitmap(unsigned int dat)
{
int i;
unsigned int *data;

data = (unsigned int *) BitmapData;
int len = BitmapWidth * BitmapHeight;

for (i=0; i < len; ++i)
	{
	*(data+i) = dat;
	}
}



void alBitmap::ClearBitmap(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
unsigned int data;

data = Unify(red,green,blue,alpha);
ClearBitmap(data);
}


BBitmap *alBitmap::GetBBitmap()
{
BBitmap *target = new BBitmap ( BRect(1,1,BitmapWidth,BitmapHeight) , B_RGB32);

int *t,*a;
int i,len = target->BitsLength();
target->Lock();
t = (int *) target->Bits();
a = (int *) BitmapData;

for (i=0; i*4 < len; i++) 
	{
	*(t+i) = *(a+i);
	}
			
target->Unlock();
return target;
}

BBitmap *alBitmap::GetBBitmap(BRect segment)
{
int w,h;
w = (int) segment.Width();
h = (int) segment.Height();
BBitmap *target = new BBitmap ( BRect(1,1,w,h) , B_RGB32);

unsigned int *t;
int i,k,y1,y2,x1,x2;
target->Lock();
t = (unsigned int *) target->Bits();
int pos;

y2 = (int) segment.LeftBottom().y;
y1 = (int) segment.LeftTop().y;
x2 = (int) segment.RightTop().x;
x1 = (int) segment.LeftTop().x;

for (i=y1;  i <= y2 ; ++i) 
	{
	for (k = x1; k <= x2; ++k)
		{
		pos = (i - y1) * w + (k - x1);
		*(t+pos) = 	GetPixel(k,i);
		}	
	}
			
target->Unlock();
return target;
}


//BBitmpa PaintBitmap functions
void alBitmap::PaintBitmap(int x, int y, BBitmap *source)
{
unsigned char red,green,blue,alpha;
int i,k,w,h;

w = (int)(source->Bounds().RightTop().x - source->Bounds().LeftTop().x + 1);
h = (int)(source->Bounds().RightBottom().y - source->Bounds().LeftTop().y + 1);
if ((x > BitmapWidth) || (y > BitmapHeight)) return;
if ((x < 0) || (y < 0)) return;

unsigned char *where, *save;
where = (unsigned char *) source->Bits();
save = BitmapData;
int bw = BitmapWidth;

for (i=0; i < h; ++i)
	for (k=0; k < w; ++k)
		{
		BitmapData=where;
		BitmapWidth = w;
		GetPixel(k,i, &red , &green, &blue, &alpha);
		BitmapWidth = bw;
		BitmapData=save;
		SetPixel(k+x, i+y,red,green,blue,255);
		}
}

void alBitmap::PaintBitmap(int x, int y, unsigned char alph, BBitmap *source)
{
unsigned char red,green,blue,alpha;
int i,k,w,h;

w = (int)(source->Bounds().RightTop().x - source->Bounds().LeftTop().x + 1);
h = (int)(source->Bounds().RightBottom().y - source->Bounds().LeftTop().y + 1);

if ((x > BitmapWidth) || (y > BitmapHeight)) return;
if ((x < -BitmapWidth) || (y < -BitmapWidth)) return;

unsigned char *where, *save;
where = (unsigned char *) source->Bits();
save = BitmapData;
int bw = BitmapWidth;
int calpha;

for (i=0; i < h; ++i)
	for (k=0; k < w; ++k)
		{
		BitmapData = where;
		BitmapWidth = w;
		GetPixel(k,i, &red , &green, &blue, &alpha);
		calpha = alph + alpha - 255;
		if (calpha < 0) alpha =0; else alpha = (int) calpha;
		BitmapData = save;
		BitmapWidth = bw;
		SetPixel(k+x, i+y,red,green,blue,alpha);
		}
}


void alBitmap::PaintBitmap(int x, int y, BBitmap *source, BRect constrain)
{
unsigned char red,green,blue,alpha;
int i,k,w,h;

w = (int)(source->Bounds().RightTop().x - source->Bounds().LeftTop().x + 1);
h = (int)(source->Bounds().RightBottom().y - source->Bounds().LeftTop().y + 1);

if ((x > BitmapWidth) || (y > BitmapHeight)) return;
if ((x < 0) || (y < 0)) return;

unsigned char *where, *save;
where = (unsigned char *)source->Bits();
save = BitmapData;
int bw = BitmapWidth;

for (i=0; i < h; ++i)
	for (k=0; k < w; ++k)
		{
		BitmapData = where;
		BitmapWidth = w;
		GetPixel(k,i, &red , &green, &blue, &alpha);
		BitmapData = save;
		BitmapWidth = bw;
		if (constrain.Contains(BPoint(k+x,i+y))) SetPixel(k+x, i+y,red,green,blue,alpha);
		}
}


void alBitmap::PaintBitmap(int x, int y, unsigned char alph, BBitmap *source, BRect constrain)
{
unsigned char red,green,blue,alpha;
int i,k,w,h;

w = (int)(source->Bounds().RightTop().x - source->Bounds().LeftTop().x + 1);
h = (int)(source->Bounds().RightBottom().y - source->Bounds().LeftTop().y + 1);

if ((x > BitmapWidth) || (y > BitmapHeight)) return;
if ((x < 0) || (y < 0)) return;

unsigned char *where, *save;
where = (unsigned char *) source->Bits();
save = BitmapData;
int bw = BitmapWidth;
int calpha;

for (i=0; i < h; ++i)
	for (k=0; k < w; ++k)
		{
		BitmapData = where;
		BitmapWidth = w;
		GetPixel(k,i, &red , &green, &blue, &alpha);
		calpha = alph + alpha - 255;
		if (calpha < 0) alpha =0; else alpha = (int) calpha;
		BitmapData = save;
		BitmapWidth = bw;
		if (constrain.Contains(BPoint(k+x,i+y))) SetPixel(k+x, i+y,red,green,blue,alpha);
		}
}


void alBitmap::WriteToFile(entry_ref *path,const char *filename)
{
BDirectory Pfad(path);
BFile Datei;

if (Pfad.InitCheck() == B_OK)
	{
	Pfad.CreateFile(filename, &Datei);
	if (Datei.InitCheck() == B_OK)
		{
		BNodeInfo DateiInfo( &Datei );
		DateiInfo.SetType("image/alBitmap");
		off_t offset;
		Datei.WriteAttr("BITMAP:WIDTH",B_UINT32_TYPE, offset, &BitmapWidth, 4);
		Datei.WriteAttr("BITMAP:HEIGHT",B_UINT32_TYPE, offset, &BitmapHeight, 4);
		Datei.Write(BitmapData,BitmapWidth*BitmapHeight*4);
		}
	}
}


alBitmap *alBitmap::Copy(BRegion *tobecopied)
{
int x,y,ex,ey,i,k,pos,data;
int *s;

BRect total;
total = tobecopied->Frame();

alBitmap *result;
x = (int) total.Width();
y = (int) total.Height();
result = new alBitmap(x, y);
result->ClearBitmap(0,0,0,0);


y = (int) total.LeftTop().y;
x = (int) total.LeftTop().x;
ey = (int) total.RightBottom().y;
ex = (int) total.RightBottom().x;
s = (int *) BitmapData;

for (i=y; i < ey; ++i)
	{
	pos = i*BitmapWidth+x;
	for (k=x; k < ex; ++k)
		{
		if (tobecopied->Contains(BPoint(k,i)) == true)
			{
			data = *(s+pos);
			result->CopyPixel(k-x,i-y,data);
			}
		++pos;
		}
	}
		
return result;
}

alBitmap *alBitmap::Cut(BRegion *tobecopied)
{
int x,y,ex,ey,i,k,pos,data,fill;
int *s;

BRect total;
total = tobecopied->Frame();

alBitmap *result;
x = (int) total.Width();
y = (int) total.Height();
result = new alBitmap(x, y);
result->ClearBitmap(0,0,0,0);

x = (int) color_red;
y = (int) color_green;
i = (int) color_blue;
k = (int) color_alpha;

fill = i;
fill += y * 0x100;
fill += x * 0x10000;
fill += k * 0x1000000;

y = (int) total.LeftTop().y;
x = (int) total.LeftTop().x;
ey = (int) total.RightBottom().y;
ex = (int) total.RightBottom().x;
s = (int *) BitmapData;

for (i=y; i < ey; ++i)
	{
	pos = i*BitmapWidth+x;
	for (k=x; k < ex; ++k)
		{
		if (tobecopied->Contains(BPoint(k,i)) == true)
			{
			data = *(s+pos);
			*(s+pos) = fill;
			result->CopyPixel(k-x,i-y,data);
			}
		++pos;
		}
	}
		
return result;
}



void alBitmap::SaveChanges(bool save, bool clear_cache, int max_changes_cacheable)
{
max_saveable_changes = max_changes_cacheable;
save_changes = save;
if (clear_cache) ClearCache();
}

void alBitmap::ClearCache()
{
int i,k;
alBitmap *store;

k = saved_changes->CountItems();
for (i=0; i<k; ++i)
	{
	if (saved_changes->RemoveItem(store)) delete store;
	}
}

void alBitmap::SubmitChange(alBitmap *savedRegion)
{
if (save_changes)
	{
	saved_changes->AddItem(savedRegion);
	if (saved_changes->CountItems() > max_saveable_changes) saved_changes->RemoveItem((int32) 0);
	}
}

void alBitmap::Undo()
{
alBitmap *toberestored;

int i = saved_changes->CountItems() - 1;
int w,h,k;

if (i >= 0)
	{
	toberestored = (alBitmap *) saved_changes->ItemAt(i);
	saved_changes->RemoveItem(i);
	int x,y;
	unsigned int data;
	x = toberestored->originx;
	y = toberestored->originy;
	

	if ((x > BitmapWidth) || (y > BitmapHeight)) return;
	if ((x < -BitmapWidth) || (y < -BitmapHeight)) return;
	w = toberestored->BitmapWidth;
	h = toberestored->BitmapHeight;

	for (i=0; i < h; ++i)
		for (k=0; k < w; ++k)
		{
		data = toberestored->GetPixel(k,i);
		CopyPixel(k+x, i+y, data);
		}	
	}
}



void alBitmap::Line(int x1, int y1, int x2, int y2)
{
int dx = x2 - x1;
int dy = y2 - y1;

if ((dx == 0) && (dy == 0)) return;

int x = x1;
int y = y1;

int ry = 1;
int rx = 1;

if (dy < 0) {ry = -1; dy = -dy;}
if (dx < 0) {rx = -1; dx = -dx;}

if (dx >= dy)
	{
	int d = dx;
	do
	{
	SetPixel(x, y);
	x += rx;
	d -= dy;
	if (d <= 0)
			{
			y += ry;
			d += dx;
			}
	}
	while ((x != x2) || (y != y2));
	}

if (dy > dx)
	{
	int d = dy;
	do
	{
	SetPixel(x, y);
	y += ry;
	d -= dx;
	if (d <= 0)
			{
			x += rx;
			d += dy;
			}
	}
	while ((x != x2) || (y != y2));
	}

SetPixel(x2, y2);
}

void alBitmap::Line(BPoint A, BPoint B)
{
int x1 = (int) A.x;
int y1 = (int) A.y;
int x2 = (int) B.x;
int y2 = (int) B.y;

int dx = x2 - x1;
int dy = y2 - y1;

if ((dx == 0) && (dy == 0)) return;

int x = x1;
int y = y1;

int ry = 1;
int rx = 1;

if (dy < 0) {ry = -1; dy = -dy;}
if (dx < 0) {rx = -1; dx = -dx;}

if (dx >= dy)
	{
	int d = dx;
	do
	{
	SetPixel(x, y);
	x += rx;
	d -= dy;
	if (d <= 0)
			{
			y += ry;
			d += dx;
			}
	}
	while ((x != x2) || (y != y2));
	}

if (dy > dx)
	{
	int d = dy;
	do
	{
	SetPixel(x, y);
	y += ry;
	d -= dx;
	if (d <= 0)
			{
			x += rx;
			d += dy;
			}
	}
	while ((x != x2) || (y != y2));
	}

}

void alBitmap::Rectangle (int x1, int y1, int x2, int y2, bool fill)
{
if (fill)
	{
	int i;

	if (y1 > y2) 
		{
		i = y1;
		y1 = y2;
		y2 = i;
		}
	for (i = y1; i <= y2; ++i)
		{
		HLine(x1,i,x2);
		}
	}
else
	{
	Line(x1,y1,x2,y1);
	Line(x1,y2,x2,y2);
	Line(x1,y1,x1,y2);
	Line(x2,y1,x2,y2);
	}
}


void alBitmap::Rectangle(BPoint A, BPoint B, bool fill = false)
{
int x1,y1,x2,y2;
x1 = (int) A.x;
y1 = (int) A.y;
x2 = (int) B.x;
y2 = (int) B.y;
Rectangle(x1,y1,x2,y2,fill);
}


void alBitmap::SRectangle(int x1, int y1, int width, int height, float angle, bool fill = false)
{
float bogen = -((angle * 3.1416) / 180.0);
float sinus = sin(bogen);
float cosinus = cos(bogen);

int ax = (int) (width * cosinus);
int ay = (int) (-width * sinus);

int bx = (int) (height*sinus);
int by = (int) (height*cosinus);

if (fill)
	{	
	BPoint *ps[4];
	ps[0] = new BPoint((float)x1,(float)y1);;
	ps[1] = new BPoint((float)(x1+ax),(float)(y1+ay));;
	ps[3] = new BPoint((float)(x1+bx),(float)(y1+by));
	ps[2] = new BPoint((float)(x1+ax+bx),(float)(y1+ay+by));;
	
	FillSimplePoly(ps,4);
	
	delete ps[1];
	delete ps[0];
	delete ps[2];
	delete ps[3];
	}
else
	{
	Line(x1,y1,x1+bx,y1+by);
	Line(x1+bx,y1+by,x1+bx+ax,y1+by+ay);
	Line(x1+ax,y1+ay,x1+bx+ax,y1+by+ay);
	Line(x1,y1,x1+ax,y1+ay);
	}
}

void alBitmap::Triangle( BPoint A, BPoint B, BPoint C, bool fill)
{
if (fill)
	{
	BPoint *Punkte[3];

	Punkte[0] = &A;
	Punkte[1] = &B;
	Punkte[2] = &C;

	int i,n, smallest;
	smallest = 0;
	float x,y;

	for (i=0; i < 3; ++i)
		for (n=0; n < i; ++n)
			{
			if (Punkte[i]->y < Punkte[n]->y)
				{
				x = Punkte[i]->x;
				y = Punkte[i]->y;
				Punkte[i]->x = Punkte[n]->x;
				Punkte[i]->y = Punkte[n]->y;
				Punkte[n]->x = x;
				Punkte[n]->y = y;
				}	
			}
			
	int dx1,dy1,dx2,dy2;
	
	dx1 = (int) (Punkte[0]->x - Punkte[1]->x);
	dx2 = (int) (Punkte[2]->x - Punkte[0]->x);

	dy1 = (int) (Punkte[1]->y - Punkte[0]->y);
	dy2 = (int) (Punkte[2]->y - Punkte[0]->y);
	
	int x1,x2;
	int xz = (int) (Punkte[0]->x);
	int y1 = (int) (Punkte[0]->y);
	
	if ((dy1 != 0) && (dy2 != 0))
		{
		for (y=0; y <= dy1; ++y)
			{
			x1 = (int) ((y * dx1) / dy1);
			x2 = (int) ((y * dx2) / dy2);
			HLine(xz-x1,y1+y,xz+x2);
			} 
		}
		else HLine(xz-dx1,y1,xz+dx2);
		
	
	int a = dy1;
	dx1 = (int) (Punkte[2]->x - Punkte[1]->x);	
	dy1 = (int) (Punkte[2]->y - Punkte[1]->y);
	int xz2 = (int) (Punkte[1]->x);
	int y2 = (int) (Punkte[1]->y);
	int o = (int) (Punkte[2]->y - Punkte[1]->y);
	
	if (dy1 != 0)
		{
		for (y = 1; y <= o; ++y)
			{
			x1 = (int) ((y * dx1) / dy1);
			x2 = (int) (((y+a) * dx2) / dy2);
			HLine(xz2 + x1 , y2+y, xz + x2);
			}
		}
		else HLine(xz2 + dx1, y2, xz + dx2);
	} // fill?
	else
	{
	Line(A,B);
	Line(B,C);
	Line(C,A);
	}
}


void alBitmap::FillSimplePoly(BPoint *PunkteListe[], int numPoints)
{

if (numPoints < 3) return;

BPoint *Punkte [numPoints];

int biggest;
biggest = 0;
int i,a,b;

for (i=1; i < numPoints; ++i)
		{
		a = (int) PunkteListe[biggest]->y;
		b = (int) PunkteListe[i]->y;
		if (b < a) biggest = i;
		}
		
for (i=0; i < numPoints; ++i)
		{
		Punkte[i] = PunkteListe[biggest];
		++biggest;
		if (biggest >= numPoints) biggest = 0;
		}


int left_center, right_center;
int left_base_y, right_base_y;
int left_dx, left_dy, right_dx, right_dy;

left_center = (int) (Punkte[0]->x);
right_center = (int) (Punkte[0]->x);
left_base_y = 0;
right_base_y = 0;

right_dx = (int) (Punkte[1]->x) - (int) (Punkte[0]->x);
right_dy = (int) (Punkte[1]->y) - (int) (Punkte[0]->y);

int lpoint = numPoints-1 , rpoint = 1;

left_dx = (int) (Punkte[lpoint]->x) - (int) (Punkte[0]->x);
left_dy = (int) (Punkte[lpoint]->y) - (int) (Punkte[0]->y);

int left_x, right_x;
int absolute_y = (int) (Punkte[0]->y);
int origin;
int rightty = (int) Punkte[1]->y , leftty = (int) Punkte[lpoint]->y;

bool change_left = true, change_right = true;


bool loopit,init;

init = true;
do
{
loopit = true;
if (init) i = 0; else i = 1;
init = false;

	while (loopit)
	{
	change_left = false;
	change_right = false;

	
	if (left_dy != 0) left_x = ((i+left_base_y) * left_dx) / left_dy; else left_x = left_dx;
	if (right_dy != 0) right_x = ((i+right_base_y) * right_dx) / right_dy; else right_x = right_dx;
	
	HLine(left_center + left_x, absolute_y, right_center + right_x);

	if (absolute_y == leftty)//(int) Punkte[lpoint]->y) //(i+left_base_y == abs(left_dy))
		{
		change_right = true;
		left_base_y = 0;
		left_center = (int) Punkte[lpoint]->x; //left_x;
		
		origin = lpoint;
		do
		{
		if (lpoint > 0)
			{
			left_dx = (int) (Punkte[lpoint-1]->x) - (int) (Punkte[origin]->x);
			left_dy = (int) (Punkte[lpoint-1]->y) - (int) (Punkte[origin]->y);
			}
		--lpoint;
		} while ((left_dy <= 0) && (lpoint > rpoint));
		leftty = (int) Punkte[lpoint]->y;
		loopit = false;
		}
	
	if (absolute_y == rightty) //(int) Punkte[rpoint]->y)//(i+right_base_y == abs(right_dy))
		{
		if (!change_right) 
			{
			change_left = true; 
			}
		else 
			{
			change_right = false; 
			}
			
		right_base_y = 0;
		right_center = (int) Punkte[rpoint]->x; //right_x;
		
		origin = rpoint;
		do 
		{
		if (rpoint < numPoints-1)
			{
			right_dx = (int) (Punkte[rpoint+1]->x) - (int) (Punkte[origin]->x);
			right_dy = (int) (Punkte[rpoint+1]->y) - (int) (Punkte[origin]->y);
			}
		++rpoint;
		} while ((right_dy <= 0) && (lpoint > rpoint)); //Skip not drawable corners which would mess the whole algorithm up
		rightty = (int) Punkte[rpoint]->y;
		loopit = false;
		}
	
	if (change_left) left_base_y += i;
	if (change_right) right_base_y += i;		
	++absolute_y;
	++i;
	}
	
} while (lpoint >= rpoint);

}


void alBitmap::TranslatePoly(BPoint *Points[], int numPoint, float byX, float byY)
{
for (int i = 0; i < numPoint; ++i)
	{
	Points[i]->x += byX;
	Points[i]->y += byY;
	}
}

void alBitmap::RotatePoly(BPoint *Points[], int numPoint, float angle , float x, float y)
{
float dx,dy,bogen;
float rx,ry;
bogen = (angle * 3.1416) / 180;
float sinus, cosinus;
sinus = sin(bogen);
cosinus = cos(bogen);

for (int i = 0; i < numPoint; ++i)
	{
	dx = Points[i]->x - x;
	dy = Points[i]->y - y;
	
	rx = dx * cosinus - dy * sinus;
	ry = dx * sinus + dy * cosinus;
	
	Points[i]->x = x+rx;
	Points[i]->y = y+ry;
	}
}


void alBitmap::ScalePoly(BPoint *Points[], int numPoint, float scale)
{
int i;
for (i=0; i < numPoint; ++i)
	{
	Points[i]->x = Points[i]->x * scale;
	Points[i]->y = Points[i]->y * scale;
	}
}

void alBitmap::ScalePoly(BPoint *Points[], int numPoint, float x, float y, float scale)
{
int i;
for (i=0; i < numPoint; ++i)
	{
	Points[i]->x = x + (Points[i]->x - x) * scale;
	Points[i]->y = y + (Points[i]->y - y) * scale;
	}
}

void alBitmap::Polygon(BPoint *Points[], int numPoints, bool closed = true)
{
int i;

if (numPoints < 2) return;

for (i = 0; i < numPoints -1; ++i)
	{
	Line( *Points[i], *Points[i+1] );
	}
	
if (closed) Line( *Points[numPoints-1], *Points[0] );
}




// fill a complex polygon
void alBitmap::FillComplexPoly(BPoint *Points[], int numPoints)
{

//check numPoints making algorithm stable
switch (numPoints)
	{
	case 0: { return; 												break; }
	case 1: { SetPixel( (int) Points[0]->x, (int) Points[0]->y);	break; }
	case 2: { Line( *Points[0], *Points[1] );                   	break; }
	}


//Variables
bool 	check_list [numPoints];
int 	i, a, b, PointsUsed, numjunk = 0, poss, pose; //poss = first node, pose = last node in a valid node string
BPoint 	*junkPoints [numPoints]; //must be deleted
BPoint  *sortedPoints [numPoints];
int lowIndex, highIndex, cmp_modus;
float dy;

//Initialization
for (i=0; i < numPoints; ++i) 
	{
	check_list[i] = false;
	junkPoints[i] = new BPoint;
	}
	
//Sort nodes
int biggest = 0;
for (i=1; i < numPoints; ++i)
		{
		a = (int) Points[biggest]->y;
		b = (int) Points[i]->y;
		if (b < a) biggest = i;
		}
		
for (i=0; i < numPoints; ++i)
		{
		sortedPoints[i] = Points[biggest];
		++biggest;
		if (biggest >= numPoints) biggest = 0;
		}

//function core
do
	{
	//Finding a node string
	poss = 0;
	while ( check_list[poss] ) ++poss;
	if (poss > 0) --poss; //If point isnt the first point then it is not the first run
	
	pose = poss;
	while ((pose < numPoints) && (check_list[pose] == false)) ++pose;
	if (poss > 0) ++pose;
	if (pose >= numPoints) pose = numPoints - 1;

	//Finding the lowest and the highest node
	highIndex = poss;
	lowIndex = pose;
	
	//aus algorithmischen gründen muss high Index nicht unbedingt der höchste Punkt sein.
	//er muss vorallem an dem Vatterpolygonen anliegen:
	if (sortedPoints[poss+1]->y < sortedPoints[poss]->y) highIndex++;
	
	for (i = poss; i <= pose; ++i) // Den tiefsten Punkt suchen
		{
		if (sortedPoints[i]->y > sortedPoints[lowIndex]->y) lowIndex = i;
		}
	
	//sort out illegal nodes
	cmp_modus = -1;
	numjunk = 1;
	
	a = i = highIndex;
	check_list[highIndex] = true;
	junkPoints[0]->x = sortedPoints[i]->x;
	junkPoints[0]->y = sortedPoints[i]->y;
	
	do
		{
		
		do
			{
			++a;											//einen Punkt weiter gehen
			if (a == pose) a = poss;						//sicherstellen ,dass sich i entlang des valid node string bewegt
			dy = sortedPoints[a]->y - sortedPoints[i]->y; 	//Höhenunterschied berechnen
			}
		while (dy * cmp_modus > 0);
	
		junkPoints[numjunk]->x = sortedPoints[a]->x;
		junkPoints[numjunk]->y = sortedPoints[a]->y;
		check_list[a] = true;
				
		i = a;
		if (i > lowIndex) cmp_modus = 1;				//und dass der Vergleichsmodus sich passend ändert
		}
	while (i != highIndex);
	
	//draw partial polygon
	FillSimplePoly(junkPoints, numjunk);
	}
while (PointsUsed < numPoints);


for (i=0; i < numPoints; ++i) delete junkPoints[i];
} //End of FillComplexPoly


BRect *alBitmap::GetPolyLimits(BPoint *Points[], int numPoints)
{
if (numPoints < 1) return NULL;

float smallesty,greatesty, smallestx, greatestx;
smallestx = Points[0]->x;
greatestx = Points[0]->x;
smallesty = Points[0]->y;
greatesty = Points[0]->y;	


for (int i=0; i < numPoints; ++i)
		{
		if (Points[i]->x > greatestx) greatestx = Points[i]->x;
		if (Points[i]->x < smallestx) smallestx = Points[i]->x;

		if (Points[i]->y > greatesty) greatesty = Points[i]->y;
		if (Points[i]->y < smallesty) smallesty = Points[i]->y;		
		}

BRect *result = new BRect(smallestx, smallesty, greatestx, greatesty);
return result;
}


void alBitmap::Rotate(float angle)
{
	BPoint 			*PunkteListe[4];
	BPoint 			*Punkte[4];
	BRect 			*limit;
	alBitmap 		*res;
	int 			numPoints 			= 4;
	int 			laufv;
	unsigned char 	rd,gn,bl,ap;
	float 			bogen 				= (angle * 3.1416) / 180;
	int 			opt1,opt2; //various optimizers
	float 			opt3, opt4, opt5, opt6;
	
	PunkteListe[0] = new BPoint(0,0);
	PunkteListe[1] = new BPoint(BitmapWidth,0);
	PunkteListe[2] = new BPoint(BitmapWidth, BitmapHeight);
	PunkteListe[3] = new BPoint(0, BitmapHeight);
	
	RotatePoly(PunkteListe,4,angle);
	limit = GetPolyLimits(PunkteListe,4);
	
	//limit->OffsetBy( -limit->LeftTop().x, -limit->LeftTop().y);
	int deltax = (int) (-limit->LeftTop().x);
	int deltay = (int) (-limit->LeftTop().y);
	TranslatePoly(PunkteListe,4, deltax, deltay);
	
	res = new alBitmap(limit->Width(),limit->Height());
	res->ClearBitmap(0,0,0,0);

		int biggest;
		biggest = 0;
		int i,a,b;

		for (i=1; i < numPoints; ++i)
				{
				a = (int) PunkteListe[biggest]->y;
				b = (int) PunkteListe[i]->y;
				if (b < a) biggest = i;
				}
				
		for (i=0; i < numPoints; ++i)
				{
				Punkte[i] = PunkteListe[biggest];
				++biggest;
				if (biggest >= numPoints) biggest = 0;
				}
		
		
		int left_center, right_center;
		int left_base_y, right_base_y;
		int left_dx, left_dy, right_dx, right_dy;
		
		left_center = (int) (Punkte[0]->x);
		right_center = (int) (Punkte[0]->x);
		left_base_y = 0;
		right_base_y = 0;
		
		right_dx = (int) (Punkte[1]->x - Punkte[0]->x);
		right_dy = (int) (Punkte[1]->y - Punkte[0]->y);
		
		int lpoint = numPoints-1 , rpoint = 1;
		
		left_dx = (int) (Punkte[lpoint]->x - Punkte[0]->x);
		left_dy = (int) (Punkte[lpoint]->y - Punkte[0]->y);
		
		int left_x, right_x;
		int absolute_y = (int) (Punkte[0]->y);
		int origin;
		
		bool change_left = true, change_right = true;
		bool loopit,init;
		
		int p1x,p1y,p2x,p2y;
		int posx, pos2x, posy;
		int vecx,vecy;
		int lx,rx;
		int differenz;
		
		init = true;
		do
		{
		loopit = true;
		if (init) i = 0; else i = 1;
		init = false;
		
			while (loopit)
			{
			change_left = false;
			change_right = false;
		
			
			if (left_dy != 0) left_x = ((i+left_base_y) * left_dx) / left_dy; else left_x = left_dx;
			if (right_dy != 0) right_x = ((i+right_base_y) * right_dx) / right_dy; else right_x = right_dx;
			
			//res->HLine(left_center + left_x, absolute_y, right_center + right_x);
			//the line must now consist of colored pixels from the source Bitmap
			posx = left_center + left_x - deltax;
			posy = absolute_y - deltay;
			
			opt5 = cos(-bogen);
			opt6 = sin(-bogen);
			opt3 = - posy * opt6;
			opt4 =   posy * opt5;

			p1x = (int) (posx * opt5 + opt3 );
			p1y = (int) (posx * opt6 + opt4);

			pos2x = right_center + right_x - deltax;
			p2x = (int) (pos2x * opt5 + opt3);
			p2y = (int) (pos2x * opt6 + opt4);
			
			
			vecx = p2x - p1x;
			vecy = p2y - p1y;
			
			lx = posx + deltax;
			rx = pos2x + deltax;
			
			if (lx > rx)
				{
				differenz = lx;
				lx = rx;
				rx = differenz;
				}

			differenz = rx - lx;			
			if (differenz == 0) differenz = 1;
			
			opt1 = 0;
			opt2 = 0;
			for (laufv = lx; laufv <= rx; ++laufv)
				{
				p2x = p1x + (opt1 / differenz);
				p2y = p1y + (opt2 / differenz);
				opt1 += vecx;
				opt2 += vecy;
				GetPixel(p2x , p2y, &rd,&gn,&bl,&ap);
				res->SetPixel(laufv,absolute_y, rd,gn,bl,ap);
				}
			
			//end line
			
			if (i+left_base_y == abs(left_dy))
				{
				change_right = true;
				left_base_y = 0;
				left_center += left_x;
				
				origin = lpoint;
				do
				{
				if (lpoint > 0)
					{
					left_dx = (int) (Punkte[lpoint-1]->x - Punkte[origin]->x);
					left_dy = (int) (Punkte[lpoint-1]->y - Punkte[origin]->y);
					}
				--lpoint;
				} while ((left_dy <= 0) && (lpoint > rpoint));
				loopit = false;
				}
			
			if (i+right_base_y == abs(right_dy))
				{
				if (!change_right) change_left = true; else change_right = false;
					
				right_base_y = 0;
				right_center += right_x;
				
				origin = rpoint;
				do 
				{
				if (rpoint < numPoints-1)
					{
					right_dx = (int) (Punkte[rpoint+1]->x - Punkte[origin]->x);
					right_dy = (int) (Punkte[rpoint+1]->y - Punkte[origin]->y);
					}
				++rpoint;
				} while ((right_dy <= 0) && (lpoint > rpoint)); //Skip not drawable corners which would mess the whole algorithm up
				loopit = false;
				}
			
			if (change_left) left_base_y += i;
			if (change_right) right_base_y += i;		
			++absolute_y;			
			++i;
			}
	
		} while (lpoint >= rpoint);

delete [] BitmapData;
BitmapWidth = res->GetWidth();
BitmapHeight = res->GetHeight();
BitmapData = (unsigned char *) res->Bits();
res->BitmapData = NULL;
delete res;

delete Punkte[0];
delete Punkte[1];
delete Punkte[2];
delete Punkte[3];
}


void alBitmap::Spline(int x1, int y1, int x2, int y2, int chx1, int chy1, int chx2, int chy2, int steps = 20, bool drawhandles)
{
int vecx, vecy;
int addx, addy;
int px,py;
float factor;

vecx = x2 - x1;
vecy = y2 - y1;

if (steps < 5) steps = 5;

BPoint *ps[steps + 1];

int i,k = steps+1;
for (i = 0; i <= steps; ++i)
	{
	px = x1+((vecx * i) / steps);
	py = y1+((vecy * i) / steps);
	
	addx = ((chx1 * (steps-i)) / steps) + (( chx2 * i ) / steps );
	addy = ((chy1 * (steps-i)) / steps) + (( chy2 * i ) / steps );
	
	--k;
	factor = sin((3.14*i) / steps); 
	factor = sqrt(factor);
	if ((factor < 0) || (factor > 1)) factor = 1;
	
	addx = (int) (addx * factor);
	addy = (int) (addy * factor);
	
	ps[i] = new BPoint( (float) (px+addx), (float) (py+addy) );	
	}
	
Polygon(ps,steps+1,false);

if (drawhandles)
{
Line(x1 + chx1, y1 + chy1, x1 ,y1);
Line(x2, y2, x2 + chx2 ,y2 + chy2);
}

for (i=0; i <= steps; ++i) delete ps[i];
}

void alBitmap::Spline(BPoint *Points[], int numPoints, int steps)
{
if (numPoints <= 0) return;

SetPixel((int) Points[0]->x,(int) Points[0]->y);

if (numPoints <= 1) return;

int i;
int currentx, currenty;
int nextx, nexty;
int lookaheadx, lookaheady;
int lookbehindx, lookbehindy;
int vecx,vecy;
int v1x,v1y,v2x,v2y;
float factor , lenge1, lenge2;

for (i=0; i < numPoints-1; ++i)
	{
	currentx = (int) Points[i]->x;
	currenty = (int) Points[i]->y;
	
	if (i+1 < numPoints)
		{
		nextx = (int) Points[i+1]->x;
		nexty = (int) Points[i+1]->y;
		
		vecx = (nextx - currentx)/3;
		vecy = (nexty - currenty)/3;
		
		v1x = vecx;
		v1y = vecy;
		v2x = -vecx;
		v2y = -vecy;
		
		if (i-1 >= 0)
			{
			lookbehindx = (int) Points[i-1]->x;
			lookbehindy = (int) Points[i-1]->y;
			
			vecx = (int) (Points[i+1]->x - lookbehindx);
			vecy = (int) (Points[i+1]->y - lookbehindy);
			
			lenge1 = sqrt(v1x*v1x*1.0 + v1y*v1y*1.0);
			lenge2 = sqrt(vecx*vecx*1.0 + vecy*vecy*1.0);
			
			factor = lenge1 / lenge2;
			v1x = (int) (vecx * factor);
			v1y = (int) (vecy * factor);
			}
		else
			{
			v1x = 0;
			v1y = 0;
			}
					
		if (i+2 < numPoints)
			{
			lookaheadx = (int) Points[i+2]->x;
			lookaheady = (int) Points[i+2]->y;
			
			vecx = currentx - lookaheadx;
			vecy = currenty - lookaheady;
			
			lenge1 = sqrt(v2x*v2x*1.0 + v2y*v2y*1.0);
			lenge2 = sqrt(vecx*vecx*1.0 + vecy*vecy*1.0);
			
			factor = lenge1 / lenge2;
			v2x = (int) (vecx * factor);
			v2y = (int) (vecy * factor);
			}
		else
			{
			v2x = 0;
			v2y = 0;
			}
		
		Spline(currentx, currenty, nextx, nexty, v1x, v1y, v2x, v2y, steps);
		}
		
	}

}

void alBitmap::Truncate(BRect newsize)
{
alBitmap *buf= new alBitmap(newsize.Width() +1, newsize.Height() +1);
buf->ClearBitmap(0,0,0,0);

int xlow, xhigh, ylow, yhigh;
xlow = (int) newsize.LeftTop().x;
ylow = (int) newsize.LeftTop().y;
xhigh = (int) newsize.RightBottom().x;
yhigh = (int) newsize.RightBottom().y;

int x,y,poss,post;
int *source, *target;
int bufwidth = buf->GetWidth();

source = (int *) BitmapData;
target = (int *) buf->Bits();

for (y = ylow; y <= yhigh; ++y)
	{
	poss = y*BitmapWidth + xlow;
	post = (y-ylow)*bufwidth;
	for (x = xlow; x <= xhigh; ++x)
		{
		*(target+post) = *(source+poss);
		++poss;
		++post;
		}
	}

delete [] BitmapData;
BitmapData = (unsigned char *) buf->Bits();
BitmapWidth = buf->GetWidth();
BitmapHeight = buf->GetHeight();
buf->BitmapData = NULL;
delete buf;
}

void alBitmap::Resize(BRect newsize)
{
alBitmap *buf = new alBitmap(newsize.Width() +1, newsize.Height() +1);

int newwidth = buf->GetWidth();
int newheight = buf->GetHeight();

int x,y;
int getx, gety;
int *source = (int *) BitmapData;
int *target = (int *) buf->Bits();
int w,h;


h=0;
for (y = 0; y < newheight; ++y)
	{
	w=0;
	for (x = 0; x < newwidth; ++x)
		{
		getx = w / newwidth;
		gety = h / newheight;
		
		w += BitmapWidth;
		
		*(target + y*newwidth +x) = *(source + gety*BitmapWidth +getx);				
		}
	h += BitmapHeight;
	}

delete [] BitmapData;
BitmapData = (unsigned char *) buf->Bits();
BitmapWidth = buf->GetWidth();
BitmapHeight = buf->GetHeight();
buf->BitmapData = NULL;
delete buf;
}

void alBitmap::TileWithBitmap(int x, int y, alBitmap *source, unsigned char alph)
{
while ( x <= -source->GetWidth() ) 	x += source->GetWidth();
while ( y <= -source->GetHeight() )	y += source->GetHeight();
while ( x > 0) 						x -= source->GetWidth();
while ( y > 0) 						y -= source->GetHeight();

int repeatx = (BitmapWidth / source->GetWidth()) + 1;
int repeaty = (BitmapHeight / source->GetHeight()) + 1;

if (x + repeatx * source->GetWidth() < BitmapWidth)   ++repeatx;
if (y + repeaty * source->GetHeight() < BitmapHeight) ++repeaty;

int i,k,w,h;

w = source->GetWidth();
h = source->GetHeight();

for (i=0; i < repeaty; ++i)
	for (k=0; k < repeatx; ++k)
		{
		PaintBitmap(x+k*w, y+i*h, alph, source);
		}
}


void alBitmap::TileWithBitmap(int x, int y, BBitmap *source, unsigned char alph)
{
int i,k,w,h;

w = (int) source->Bounds().Width();
h = (int) source->Bounds().Height();


while ( x <= -w ) 	x += w;
while ( y <= -h )	y += h;
while ( x > 0)		x -= w;
while ( y > 0)		y -= h;

int repeatx = (BitmapWidth / w) + 1;
int repeaty = (BitmapHeight / h) + 1;

if (x + repeatx * w < BitmapWidth)   ++repeatx;
if (y + repeaty * h < BitmapHeight) ++repeaty;

for (i=0; i < repeaty; ++i)
	for (k=0; k < repeatx; ++k)
		{
		PaintBitmap(x+k*w, y+i*h, alph, source);
		}
}

void alBitmap::Circle( int x, int y, int r, bool fill = false)
{
r = abs(r);
int auslenkung,alteauslenkung = 0;
int sr = r*r;
int xr = -r;


if (fill)
	{
	for (int ay = y - r; ay <= y + r; ++ay)
		{
		auslenkung = (int) sqrt(sr - xr*xr);
		++xr;
	
		HLine(-auslenkung + x, ay, auslenkung + x);
		}
	}
else
	{
	for (int ay = y - r; ay <= y + r; ++ay)
		{
		auslenkung = (int) sqrt(sr - xr*xr);
		++xr;
	
		Line( alteauslenkung + x, ay-1,  auslenkung + x , ay);
		Line(-alteauslenkung + x, ay-1, -auslenkung + x, ay);
		alteauslenkung = auslenkung;
		}	
	}
}


void alBitmap::Ellipse(int x, int y, int rx, int ry, float angle, bool fill , int quality)
{
int greater;
int qrx = rx*rx;
int qry = ry*ry;
float bufx, bufy, bogen = (angle * 3.1416) / 180;
float sinus, cosinus;

sinus = sin(bogen);
cosinus = cos(bogen);

rx = abs(rx);
ry = abs(ry);

greater = rx;
if (ry > rx) greater = ry;

BPoint *P[greater*4];
int PointsUsed = 0;

int i;
for (i = - rx; i < 0; i += quality)
	{			
	P[PointsUsed] = new BPoint;
	
	bufy = sqrt (qry - (qry*i*i) / qrx );
	bufx = (float) i;
	P[PointsUsed]->y = bufy;
	P[PointsUsed]->x = bufx;
	++PointsUsed;
	}

for (i = PointsUsed - 1; i >= 0; --i)
	{			
	P[PointsUsed] = new BPoint;
	P[PointsUsed]->y = P[i]->y;
	P[PointsUsed]->x = -P[i]->x;	
	++PointsUsed;
	}

for (i = PointsUsed - 1; i >= 0; --i)
	{			
	P[PointsUsed] = new BPoint;
	P[PointsUsed]->y = -P[i]->y;
	P[PointsUsed]->x = P[i]->x;	
	++PointsUsed;
	}


RotatePoly(P, PointsUsed, angle);
TranslatePoly(P,PointsUsed, (float) x, (float) y);

if (fill) FillSimplePoly(P,PointsUsed);
else Polygon(P,PointsUsed);


for (i=0; i < PointsUsed; ++i) delete P[i];
}

void alBitmap::SetColor(unsigned int color_code)
{
unsigned int colorem, transp;

colorem = color_code & 0xFF;
transp	= color_code & 0xFF00;

if (transp == 0) transp = OPAQUE;

switch (colorem)
	{
	case TRANSPARENCY:	{ SetColor(0,0,0,0); break; }
	
	case WHITE:			{ SetColor(255,255,255); break; }
	case BLACK:			{ SetColor(0,0,0); break; }
	case GREY:			{ SetColor(128,128,128); break; }
	case BRIGHT_GREY:	{ SetColor(200,200,200); break; }
	case DARK_GREY:		{ SetColor(75,75,75); break; }
	
	case BLUE:			{ SetColor(0,0,200); break; }
	case BRIGHT_BLUE:	{ SetColor(0,0,255); break; }
	case DARK_BLUE:		{ SetColor(0,0,128); break; }
	
	case RED:			{ SetColor(200,0,0); break; }
	case BRIGHT_RED:	{ SetColor(255,0,0); break; }
	case DARK_RED:		{ SetColor(128,0,0); break; }
	
	case GREEN:			{ SetColor(0,200,0); break; }
	case BRIGHT_GREEN:	{ SetColor(0,255,0); break; }
	case DARK_GREEN:	{ SetColor(0,128,0); break; }

	case YELLOW:		{ SetColor(200,200,0); break; }
	case BRIGHT_YELLOW:	{ SetColor(255,255,0); break; }
	case DARK_YELLOW:	{ SetColor(128,128,0); break; }
	
	case CYAN:			{ SetColor(0,200,200); break; }
	case BRIGHT_CYAN:	{ SetColor(0,255,255); break; }
	case DARK_CYAN:		{ SetColor(0,128,128); break; }
	
	case MAGENTA:		{ SetColor(200,0,200); break; }
	case BRIGHT_MAGENTA:{ SetColor(255,0,255); break; }
	case DARK_MAGENTA:	{ SetColor(128,0,128); break; }

	case PINK:			{ SetColor(200,128,200); break; }
	case BRIGHT_PINK:	{ SetColor(255,200,255); break; }
	case DARK_PINK:		{ SetColor(180,100,180); break; }
	
	case BROWN:			{ SetColor(170,128,0); break; }
	case BRIGHT_BROWN:	{ SetColor(200,150,0); break; }
	case DARK_BROWN:	{ SetColor(128,90,0); break; }
	
	case VIOLETE:		{ SetColor(180,0,235); break; }
	case BRIGHT_VIOLETE:{ SetColor(200,0,255); break; }
	case DARK_VIOLETE:	{ SetColor(150,0,200); break; }
	
	case ORANGE:		{ SetColor(255,180,0); break; }
	case BRIGHT_ORANGE:	{ SetColor(255,200,0); break; }
	case DARK_ORANGE:	{ SetColor(220,150,0); break; }
	}
	
switch (transp)
	{
	case TRANSPARENT:	{ color_alpha = 0; break; }
	case GLASS:			{ color_alpha = 45; break; }
	case HALFTONE:		{ color_alpha = 128; break; }
	case HARDTOSEE:		{ color_alpha = 210; break; }
	case OPAQUE:		{ color_alpha = 255; break; }
	}
	
color_data = Unify(color_red, color_green, color_blue, color_alpha);
}
