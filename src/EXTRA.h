#include <Message.h>
#include <List.h>
#include <String.h>
#include <Window.h>
#include <Looper.h>
#include <Rect.h>
#include <Screen.h>
#include <Handler.h>
#include <stdio.h>
#include <Application.h>
#include <Button.h>
#include <OS.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Alert.h>
#include <TextView.h>
#include <TranslatorRoster.h>
#include <TranslationDefs.h>
#include <TranslatorFormats.h>
#include <Bitmap.h>
#include <Picture.h>
#include <File.h>
#include <BitmapStream.h>
#include <Region.h>
#include <DataIO.h>
#include <Path.h>
#include <Roster.h>
#include <Entry.h>
#include <Cursor.h>
#include <Control.h>
#include <ScrollBar.h>
#include <Directory.h>
#include <NodeInfo.h>

/*
Class list:

1. namespace extra (all kinds of functions)
2. class alRect	- BRect (useless: best if removed)
3. class alHandler - derived from none (dont like complicated BHandlers)
4. class alLooper - derived from BLooper (Now Threads are really easy)
5. class alNormalWindow - from BWindow (uuf! Helps you concentrate on the real thing)
6. class alQAWin - from alNormalWindow (a special window for a quick setup of a window app)
7. class alQuickApp - from BApplication (the fundament of all applications written by Aleksander Lodwich)
8. class alLeftButton - from BButton (an ordinary button)
9. class alRightButton - from BButton (follows right edge)
10.class alDynButton - from BButton (a BButton folowing left and right side)
11.class alHButtonBar - from BView (alignment support:manages BButton array horizontally)
12.class alVButtonBar - from BView (alignment support:manages BButton arrays vertically)
13.class alBitmapButton - from BControl (a one state image Button)
14.class alSwitchButton - from BControl (a two state image Button)
15.class alHPictureBar - from none (ImageBar horizontal)
16.class alVPictureBar - from none (ImageBar Vertical)
17.class alResourceFile - from BPositionIO (for EXTRA.cpp internal purposes; use GetResourceBitmap() )
18.class alTreeViewOBJ - from BView (custom treeview)
19.class alQuickView - from BView (for Quick tests or if you don´t want to write an extra class)
20.class alControl - from BControl (Scrollable and ToolTip Support)
20.class alScrollControl - from none (Provides scrolls for alControl)
21.class alToolTip -from alNormalWindow (provides ToolTip Windows)
22.class alBitmap - from nonw (provides Bitmap the way I like)
*/


const int CONST_AppWantsToQuit= 1000; // called once before app quits. is called from ~alQuickApp. Here you delete your Handlers.
const int CONST_AppReadyToRun=  1001; // Here you load your resource files, create windows and that like.
const int CONST_MainFrameQuits= 1002; // one of your mainframe type windows quits. for organizational purposes. called from ~alNormalWindow
const int CONST_QuitThisFrame=  1003; // with this message you can make anything you want ...
const int CONST_PULSE=  		1004; // pendant to B_PULSE

const int CONST_TTPopStyle = 2000; // ToolTip appeares immediatelly
const int CONST_TTZoomStyle = 2001; // ToolTip builds up 
const int CONST_TTNotShow = 2002; //ToolTip will not be displayed!

const unsigned int TRANSPARENCY 	= 0;
const unsigned int WHITE 			= 1;
const unsigned int BRIGHT_GREY 		= 2;
const unsigned int GREY 			= 3;
const unsigned int DARK_GREY 		= 4;
const unsigned int BLACK 			= 5;
const unsigned int BRIGHT_BLUE 		= 6;
const unsigned int BLUE 			= 7;
const unsigned int DARK_BLUE 		= 8;
const unsigned int BRIGHT_RED 		= 9;
const unsigned int RED 				= 10;
const unsigned int DARK_RED 		= 11;
const unsigned int BRIGHT_GREEN 	= 12;
const unsigned int GREEN 			= 13;
const unsigned int DARK_GREEN 		= 14;
const unsigned int BRIGHT_YELLOW 	= 15;
const unsigned int YELLOW 			= 16;
const unsigned int DARK_YELLOW 		= 17;
const unsigned int BRIGHT_CYAN 		= 18;
const unsigned int CYAN 			= 19;
const unsigned int DARK_CYAN 		= 20;
const unsigned int BRIGHT_MAGENTA 	= 21;
const unsigned int MAGENTA 			= 22;
const unsigned int DARK_MAGENTA 	= 23;
const unsigned int BRIGHT_PINK	 	= 24;
const unsigned int PINK 			= 25;
const unsigned int DARK_PINK 		= 26;
const unsigned int BRIGHT_BROWN 	= 27;
const unsigned int BROWN 			= 28;
const unsigned int DARK_BROWN 		= 29;
const unsigned int BRIGHT_VIOLETE 	= 30;
const unsigned int VIOLETE 			= 31;
const unsigned int DARK_VIOLETE 	= 32;
const unsigned int BRIGHT_ORANGE 	= 33;
const unsigned int ORANGE 			= 34;
const unsigned int DARK_ORANGE 		= 35;

const unsigned int TRANSPARENT 	= 0x100;
const unsigned int GLASS 		= 0x300;
const unsigned int HALFTONE 	= 0x500;
const unsigned int HARDTOSEE 	= 0x700;
const unsigned int OPAQUE 	 	= 0x900;


namespace extra
{
BString StringOutOf(int what);
BString fStringOutOf(float what, int dez);
int32 OKBox (const char *Output);
int32 YesNoBox (const char *Output);
int32 YesNoCancelBox (const char *Output);
int32 ErrorBox (const char *Output);
int32 CBox2 ( const char *sel1, const char *sel2 );
int32 CBox3 ( const char *sel1, const char *sel2 , const char *sel3);
BBitmap *FetchBitmap(const char *filename);
BBitmap *GetResourceBitmap(const char *file, int32 type, int32 ID );
void WaitMilisecs( int32 wait );
void ShowFileError( status_t *result);
}



class alRect : public BRect
{
public:
alRect();
alRect ( BRect rxc );
alRect (float x1, float y1, float x2, float y2);
void ResizeTo (float width, float height);
void SetWidth ( float width );
void SetHeight (float height );
};



class alHandler
{
public:
sem_id HandlerSemaphore;
bool initialized;
bool isprotected;
char *Parent;
const char *HandlerName;

typedef void (*MessageReceiver) ( BMessage *message , alHandler *thisone);
alHandler();
alHandler(MessageReceiver mr, const char *name);
alHandler(const char *name);
void HandInMessage(BMessage *message );
void (*MessageReceived) ( BMessage *message , alHandler *thisone);
virtual ~alHandler();
virtual void Open();
virtual void Protect();
};



class alLooper : public BLooper
{
public:
typedef void (*func) (BMessage *m, alHandler *h);

alLooper(func Function,const char *thread_name, int32 priority);
~alLooper();

virtual void MessageReceived(BMessage *message);
virtual void SetPriority(int32 p);
alHandler *myHandler;
char *Parent;
};


class alThread
{
public:
volatile bool stop;
thread_id ThisThread;
bool successful;
bool syncsign;

typedef int32 (*thr) (void *s);
void *DataSource;

alThread(thr thread_function, const char* thread_name, bool autoresume = false, int32 priority = B_LOW_PRIORITY );
~alThread();

void Suspend();
void Resume();
void WaitFor();
bool Sync();
};


class alNormalWindow : public BWindow
{
public:
alNormalWindow (alHandler *TH, float x1, float y1, float x2, float y2, const char *Name );
alNormalWindow (alHandler *TH, float width, float height, const char *Name );
alNormalWindow (alHandler *TH, const char *Name);
~alNormalWindow();
virtual void CloseWin();
virtual void DispatchMessage(BMessage *message, BHandler *target);
virtual void MessageReceived(BMessage *message);
virtual	void KeyDown(const char *bytes, int32 numBytes);
virtual void FrameMoved(BPoint origin);
virtual void MouseDown( BPoint p);
virtual void OpenHandler();
virtual void ProtectHandler();
virtual void FixFrame();
virtual void FreeFrame();
virtual void Pulse();
virtual void FrameResized(float width, float height);
virtual void Delete( BView *any);
virtual void Modus1();
virtual void Modus2();
virtual void DeleteMyHandler(bool yesno);

alHandler *AnyMessageHandler;
bool killhandler;
int op_modus;
float orx,ory;
float oldw, oldh;
bool skipanyfurther;
};







class alQAWin : public alNormalWindow
{
public:
alQAWin (alHandler *TH, float x1, float y1, float x2, float y2, const char *Name );
alQAWin (alHandler *TH, float width, float height, const char *Name );
alQAWin (alHandler *TH, const char *Name);
~alQAWin ();

BMenu *AddMenuToFrame(const char *title);
BMenu *AddMenuToMenu(const char *title, BMenu *parent_menu);
BMenuItem *AddItemToMenu(const char *title, BMessage *action_mes, BMenu *parent_menu);
void ShowMenu();

BMenuBar *MainFrameMenu;
};






class alQuickApp : public BApplication
{
public:
alQAWin *AppMainFrame;
alHandler *AnyMessageHandler;
bool initialized;
BPath *MyFilePosition;
unsigned char *current_cursor;

alQuickApp(alHandler *TH, const char *name, const char *wintitle, float x1 , float y1, float x2, float y2);
alQuickApp(alHandler *TH, const char *name, const char *wintitle, float width , float height);
alQuickApp(alHandler *TH, const char *name, const char *wintitle);
alQuickApp(alHandler *TH, const char *name);
~alQuickApp();
virtual void MessageReceived(BMessage *message);
virtual void ReadyToRun();
virtual void *GetActiveWindow();
virtual void RefsReceived(BMessage *message);
virtual void CursorArrow();
virtual void CursorBusy();
virtual void CursorCross();
virtual void CursorPen();
virtual void CursorHelp();
virtual void CheckCursor();
};










class alLeftButton : public BButton
{
public:
BMessage *action_message;

alLeftButton(float x1, float y1, float width, float height, const char *title, BMessage *action);
alLeftButton(float x1, float y1, const char *title, BMessage *action);
~alLeftButton();

void ReplaceMessage( BMessage *action );
virtual	void KeyDown(const char *bytes, int32 numBytes);
};







class alRightButton : public BButton
{
public:
BMessage *action_message;

alRightButton(float x1, float y1, float width, float height, const char *title, BMessage *action);
alRightButton(float x1, float y1, const char *title, BMessage *action);
~alRightButton();

void ReplaceMessage( BMessage *action );
virtual	void KeyDown(const char *bytes, int32 numBytes);
};










class alDynButton : public BButton
{
public:
BMessage *action_message;

alDynButton(float x1, float y1, float x2, float height, const char *title, BMessage *action);
~alDynButton();

void ReplaceMessage( BMessage *action );
virtual	void KeyDown(const char *bytes, int32 numBytes);
};











class alHButtonBar : public BView
{
public:
int maxx;
alHButtonBar ( float x1, float y1, float width );
virtual void AddButton ( const char *title, BMessage *message );
virtual void AddChild( BView *none_accepted, BView *before = NULL);
~alHButtonBar();
};

















class alVButtonBar : public BView
{
public:
int maxy;
alVButtonBar ( float x1, float y1, float width, float height );
virtual void AddButton ( const char *title, BMessage *message );
virtual void AddChild( BView *none_accepted, BView *before = NULL);
~alVButtonBar();
};





class alBitmapButton: public BControl
{
public:
BBitmap *B1, *B2;
BMessage *Nachricht;
rgb_color backcolor;

alBitmapButton( float x, float y, const char *f1, const char *f2, BMessage *activated);
alBitmapButton( float x, float y, BBitmap *f1, BBitmap *f2, BMessage *activated);
virtual void Draw(BRect restriction);
virtual void MouseDown( BPoint p);
virtual void MouseUp( BPoint p);
~alBitmapButton();
};






class alSwitchButton: public BControl
{
public:
BBitmap *B1, *B2;
BMessage *N1, *N2;
rgb_color backcolor;

alSwitchButton( float x, float y, const char *f1, const char *f2, BMessage *ON, BMessage *OFF);
alSwitchButton( float x, float y, BBitmap *f1, BBitmap *f2, BMessage *ON, BMessage *OFF);
virtual void Draw(BRect restriction);
virtual void MouseDown( BPoint p);
virtual void MouseUp( BPoint p);
~alSwitchButton();
};





class alHPictureBar
{
public:

float maximum;
float x,y;
alNormalWindow *Window;

alHPictureBar(float xi, float yi, alNormalWindow *anywin);
virtual ~alHPictureBar();
virtual alSwitchButton *AddSwitch(const char *f1, const char *f2, BMessage *on, BMessage *off);
virtual alBitmapButton *AddButton(const char *f1, const char *f2, BMessage *activated);
virtual alSwitchButton *AddSwitch(BBitmap *f1, BBitmap *f2, BMessage *on, BMessage *off);
virtual alBitmapButton *AddButton(BBitmap *f1, BBitmap *f2, BMessage *activated);

};


class alVPictureBar
{
public:

float maximum;
float x,y;
alNormalWindow *Window;

alVPictureBar(float xi, float yi, alNormalWindow *anywin);
virtual ~alVPictureBar();
virtual alSwitchButton *AddSwitch(const char *f1, const char *f2, BMessage *on, BMessage *off);
virtual alBitmapButton *AddButton(const char *f1, const char *f2, BMessage *activated);
virtual alSwitchButton *AddSwitch(BBitmap *f1, BBitmap *f2, BMessage *on, BMessage *off);
virtual alBitmapButton *AddButton(BBitmap *f1, BBitmap *f2, BMessage *activated);
};







class alResourceFile : public BPositionIO
{
public:

alResourceFile(const char *resource_filename, int32 type, int32 ID);
~alResourceFile();

virtual ssize_t Read(void *buffer, size_t numBytes);
virtual ssize_t ReadAt(off_t posx, void *buffer, size_t numBytes);
virtual off_t Seek(off_t posx, uint32 mode);
virtual off_t Position(void) const;
virtual ssize_t WriteAt(off_t posx, const void *buffer, size_t numBytes);

uint32 position;
size_t size;
void *zeiger;
area_id ma;
uint32 memspace;
};










class alTreeViewOBJ : public BView
{
public:
alTreeViewOBJ(BLooper *TargetLooper);
alTreeViewOBJ(BLooper *TargetLooper, float x1, float y1, float x2, float y2);
~alTreeViewOBJ();
virtual void Draw(BRect updateRect);
virtual void DrawNeccessary();
void Refresh();
virtual void MouseDown(BPoint point);
virtual void MouseMoved(BPoint point, uint32 transit, const BMessage *message);
virtual void AttachedToWindow(void);

BList *TreeData;
BLooper *LooperForMessages;
BString *TreeName;
BScrollBar *HorizontalScroll;
BScrollBar *VerticalScroll;
int Sel;
int oldSel;
int overfloat, oldoverfloat;
long width, height;
};











class alQuickView : public BView
{
public:
void *DataServer;

alQuickView(float x1, float y1, float x2, float y2);

virtual void Draw( BRect restriced);
virtual void Refresh();
virtual void MouseDown( BPoint p );
virtual void MouseUp( BPoint p );
virtual void KeyDown ( const char *what, int32 code );
virtual void KeyUp ( const char *what, int32 code );

bool DoDraw;
bool DoMouseDown;
bool DoMouseUp;
bool DoKeyDown;
bool DoKeyUp;

void (*userDraw) (alQuickView *me);
void (*userMouseDown) (alQuickView *me,BPoint p);
void (*userMouseUp) (alQuickView *me,BPoint p);
void (*userKeyDown) (alQuickView *me,const char *what, int32 code);
void (*userKeyUp) (alQuickView *me,const char *what, int32 code);
};








class alControl;

class alToolTip
{
public:
alToolTip(BString *tip , int style, BPoint p);
~alToolTip();

alControl *Invokator;
alNormalWindow *TTWindow;
BString *tipx;
alQuickView *view;
int timeleft;
int buildup;
float offset;
alQuickView *qView;
};











class alScrollControl;

class alControl : public BControl
{
public:

//Datas you normally don´t touch; things in here that do no concern anybody, but it´s up to you!
BString *ToolTipText;
int tooltipstyle;
bool showagain;
bool tooltipinited;
bool isattached;
alToolTip *floater;


float x_minRange, x_maxRange;
float y_minRange, y_maxRange;
alScrollControl *Parent;
bool HasScroller;
uint32 oldtransit;
BCursor *custom_cursor;

//Methods you normally use
alControl(float x1,float y1, float x2,float y2, const char *name, const char *label, BMessage *message, uint32 resMode, uint32 flags);
~alControl(void);

virtual float GetScrollX(void);
virtual float GetScrollY(void);
virtual void NewHScrollValue ( float value);
virtual void NewVScrollValue ( float value);
virtual void MouseMoved(BPoint p, uint32 transit, const BMessage *message);
bool IsAttached() {return isattached;}
void MakeAttached();
void CheckToolTip(int transit, BPoint p); // You call this function from MouseMoved to control standard ToolTips
void ActivateToolTip(const char *tip);
void DeactivateToolTip();
void SetToolTipStyle(int style);
void CheckBars(void);
void SetCursor(void *buf);
};











class CustomScroll : public BScrollBar
{
public:
alScrollControl *Mutter;
CustomScroll (alScrollControl *mom, BRect r,const char *name, BView *any, float min, float max, orientation direction);
~CustomScroll();
virtual void ValueChanged(float value);
};

class alScrollControl
{
public:
alScrollControl(alControl *target, BView *ANY, bool horizontal, bool vertical);
alScrollControl(alControl *target, bool horizontal, bool vertical);
virtual ~alScrollControl();
virtual void UpdateScrolls();

BWindow *anywindow;
BView *anyparentview;
CustomScroll *hBar;
CustomScroll *vBar;
bool HasHorizontal;
bool HasVertical;
alControl *anyview;
};


















class alBitmap
{
public:
//You never touch those directly unless you know what you do!
int BitmapWidth, BitmapHeight;
unsigned char *BitmapData;
unsigned char color_red,color_green, color_blue, color_alpha;
unsigned int color_data;
int originx, originy;
bool Aliasing;

bool save_changes;
int max_saveable_changes;
BList *saved_changes;

//a couple of useful constructors
alBitmap(int width, int height);
alBitmap(const char *filename);
alBitmap(BFile source);
alBitmap(BBitmap *source);
virtual ~alBitmap();


//useful stuff
virtual void WriteToFile(entry_ref *path, const char *filename);
void SaveChanges(bool save, bool clear_cache, int max_changes_cacheable);
bool IsSavingChanges() {return save_changes;}
void ClearCache();
void Undo();
void SubmitChange(alBitmap *savedRegion);


//all inline Pixelworks
virtual unsigned int Unify(unsigned char red,unsigned  char green, unsigned char blue, unsigned char alpha = 255)
		{
		unsigned int a,b,c,d;
		a = alpha;
		b = red;
		c = green;
		d = blue;
		a <<= 24;
		b <<= 16;
		c <<= 8;
		unsigned int data = a | b | c | d ;
		return data;
		}

virtual void CopyPixel(int x, int y, unsigned char red,unsigned  char green, unsigned char blue, unsigned char alpha)
{
if ((x >= 0) && (x < BitmapWidth))
	if ((y >= 0) && (y < BitmapHeight))
		{
		int pos = y*4*BitmapWidth + x*4;
		*(BitmapData+pos) = blue;
		*(BitmapData+pos+1) = green;
		*(BitmapData+pos+2) = red;
		*(BitmapData+pos+3) = alpha;		
		}
}

virtual void CopyPixel(int x, int y, unsigned int data)
{
int *s;
s = (int *) BitmapData;

if ((x >= 0) && (x < BitmapWidth))
	if ((y >= 0) && (y < BitmapHeight))
		{
		int pos = y*BitmapWidth + x;
		*(s+pos) = data;
		}
}


virtual void CopyPixel(int x, int y)
{
int *s;
s = (int *) BitmapData;

if ((x >= 0) && (x < BitmapWidth))
	if ((y >= 0) && (y < BitmapHeight))
		{
		int pos = y*BitmapWidth + x;
		*(s+pos) = color_data;
		}
}

virtual void SetPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
int pos;
signed int r2,g2,b2,a2;
signed int r,g,b,a;

r = (signed int) red;
g = (signed int) green;
b = (signed int) blue;
a = (signed int) alpha;

if ((x >= 0) && (x < BitmapWidth))
	if ((y >= 0) && (y < BitmapHeight))
		{
		pos = (y*BitmapWidth + x) << 2;

		b2 = *(BitmapData+pos);
		g2 = *(BitmapData+pos+1);
		r2 = *(BitmapData+pos+2);
		a2 = *(BitmapData+pos+3);

		b2 = b2+(((b - b2)*a) >> 8 );
		g2 = g2+(((g - g2)*a) >> 8 );
		r2 = r2+(((r - r2)*a) >> 8 );
		a2 += a;
		if (a2 > 255) a2 = 255;
			
		*(BitmapData+pos) = b2;
		*(BitmapData+pos+1) = g2;
		*(BitmapData+pos+2) = r2;
		*(BitmapData+pos+3) = a2;		
		
		if (Aliasing)
			{//C
			int i,k;
	
			r = r2;
			g = g2;
			b = b2;
			a = a2 / 8;
			int j = a2 / 10;
	
			for (i = -1; i < 2; ++i)
				{
				if ((y+i >= 0) && (y+i < BitmapHeight)) 
				for (k = -1; k < 2; ++k)
					{
					if ((x+k >= 0) && (x+k < BitmapWidth))
						{
						j = -j;
						a2 = a + j;
						pos = ((y+i)*BitmapWidth + (x+k)) << 2;
		
						b2 = *(BitmapData+pos);
						g2 = *(BitmapData+pos+1);
						r2 = *(BitmapData+pos+2);
						a2 = *(BitmapData+pos+3);

						b2 = b2+(((b - b2)*a)>>8);
						g2 = g2+(((g - g2)*a)>>8);
						r2 = r2+(((r - r2)*a)>>8);
						a2 += a;
						if (a2 > 255) a2 = 255;
			
						*(BitmapData+pos) = b2;
						*(BitmapData+pos+1) = g2;
						*(BitmapData+pos+2) = r2;
						*(BitmapData+pos+3) = a2;				
						}
					}
				}
			} //Aliasing

		}
}


virtual void SetPixel(int x, int y)
{ //A
int pos;
signed int r2,g2,b2,a2;
signed int r,g,b,a;

r = (signed int) color_red;
g = (signed int) color_green;
b = (signed int) color_blue;
a = (signed int) color_alpha;

if ((x >= 0) && (x < BitmapWidth))
	if ((y >= 0) && (y < BitmapHeight))
		{//B
		pos = (y*BitmapWidth + x) << 2;

		b2 = *(BitmapData+pos);
		g2 = *(BitmapData+pos+1);
		r2 = *(BitmapData+pos+2);
		a2 = *(BitmapData+pos+3);

		b2 = b2+(((b - b2)*a) >> 8);
		g2 = g2+(((g - g2)*a) >> 8);
		r2 = r2+(((r - r2)*a) >> 8);
		a2 += a;
		if (a2 > 255) a2 = 255;
			
		*(BitmapData+pos) = b2;
		*(BitmapData+pos+1) = g2;
		*(BitmapData+pos+2) = r2;
		*(BitmapData+pos+3) = a2;		
		
		if (Aliasing)
			{//C
			int i,k;
	
			r = r2;
			g = g2;
			b = b2;
			a = a2 / 2;
			int j = a2 / 4;
	
			for (i = -1; i < 2; ++i)
				{
				if ((y+i >= 0) && (y+i < BitmapHeight)) 
				for (k = -1; k < 2; ++k)
					{
					if ((x+k >= 0) && (x+k < BitmapWidth))
						{
						j = -j;
						a2 = a + j;
						pos = ((y+i)*BitmapWidth + (x+k)) << 2;
		
						b2 = *(BitmapData+pos);
						g2 = *(BitmapData+pos+1);
						r2 = *(BitmapData+pos+2);
						a2 = *(BitmapData+pos+3);

						b2 = b2+(((b - b2)*a) >> 8);
						g2 = g2+(((g - g2)*a) >> 8);
						r2 = r2+(((r - r2)*a) >> 8);
						a2 += a;
						if (a2 > 255) a2 = 255;
			
						*(BitmapData+pos) = b2;
						*(BitmapData+pos+1) = g2;
						*(BitmapData+pos+2) = r2;
						*(BitmapData+pos+3) = a2;				
						}
					}
				}
			} //Aliasing
		} //if x and y are visible
}//A


virtual void GetPixel(int x, int y, unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *alpha)
{
if ((x >= 0) && (x < BitmapWidth))
	{
	if ((y >= 0) && (y < BitmapHeight))
		{
		int pos = y*4*BitmapWidth + x*4;
		*blue   = *(BitmapData+pos);
		*green = *(BitmapData+pos+1);
		*red  = *(BitmapData+pos+2);
		*alpha = *(BitmapData+pos+3);		
		}
	else 
		{
		red = 0;
		green = 0;
		blue = 0;
		alpha = 0;
		}
	}
else 
	{
	red = 0;
	green = 0;
	blue = 0;
	alpha = 0;	
	}
}

virtual unsigned int GetPixel(int x, int y)
{
unsigned int data;
unsigned int *a = (unsigned int *) BitmapData;
data = 0;

if ((x >= 0) && (x < BitmapWidth))
	if ((y >= 0) && (y < BitmapHeight))
		{
		int pos = y*BitmapWidth + x;
		data = *(a+pos);
		}
return data;
}


virtual void GetAlpha(int x, int y, unsigned char *alpha)
{
if ((x >= 0) && (x < BitmapWidth))
	if ((y >= 0) && (y < BitmapHeight))
		{
		int pos = y*4*BitmapWidth + x*4;
		*alpha = *(BitmapData+pos+3);		
		}
}

virtual void CopyAlpha(int x, int y, unsigned char alpha)
{
if ((x >= 0) && (x < BitmapWidth))
	if ((y >= 0) && (y < BitmapHeight))
		{
		int pos = y*4*BitmapWidth + x*4;
		*(BitmapData+pos+3) = alpha;		
		}
}

virtual void SetAlpha(int x, int y, unsigned char alpha)
{
if ((x >= 0) && (x < BitmapWidth))
	if ((y >= 0) && (y < BitmapHeight))
		{
		int pos = y*4*BitmapWidth + x*4;
		unsigned char a = *(BitmapData+pos+3);		
		alpha += a;
		if (alpha > 255) alpha = 255;
		*(BitmapData+pos+3) = alpha;		
		}
}


//A couple of general Object settings methods
virtual bool IsAntiAliasing() {return Aliasing;}
virtual void SetAntiAliasing(bool alias) {Aliasing = alias;}

virtual int GetWidth() {return BitmapWidth;}
virtual int GetHeight() {return BitmapHeight;}
virtual void *Bits() {return (void *) BitmapData;}
virtual void SetBits( void *RGB32Data, int width, int height) 
			 {
			 delete [] BitmapData;
			 BitmapData = (unsigned char *) RGB32Data; 
			 BitmapWidth = width; 
			 BitmapHeight = height; 
			 }


//a couple of Bitmap painting routines
virtual void PaintBitmap(int x, int y, alBitmap *source);
virtual void PaintBitmap(int x, int y, unsigned char alph, alBitmap *source);
virtual void PaintBitmap(int x, int y, alBitmap *source, BRect constrain);
virtual void PaintBitmap(int x, int y, unsigned char alph, alBitmap *source, BRect constrain);
virtual void TileWithBitmap(int x, int y, alBitmap *source, unsigned char alph = 255);


virtual void PaintBitmap(int x, int y, BBitmap *source);
virtual void PaintBitmap(int x, int y, unsigned char alph, BBitmap *source);
virtual void PaintBitmap(int x, int y, BBitmap *source, BRect constrain);
virtual void PaintBitmap(int x, int y, unsigned char alph, BBitmap *source, BRect constrain);
virtual void TileWithBitmap(int x, int y, BBitmap *source, unsigned char alph = 255);


//With these functions you can retrieve new BBitmap Objects which then belong to you alone.
virtual BBitmap *GetBBitmap ();
virtual BBitmap *GetBBitmap (BRect segment);


void SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
{
color_red = red;
color_green = green;
color_blue = blue;
color_alpha = alpha;
color_data = Unify(red,green,blue,alpha);
}

void SetColor(unsigned int color_code = 0);

void GetColor( unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *alpha)
{
*red = color_red;
*green = color_green;
*blue = color_blue;
*alpha = color_alpha;
}


// a special Line version that is accelerated and can only draw horizontally, for filling purposes
virtual void HLine(int x1, int y, int x2)
{
int x;
if (x1 > x2)
	{
	x = x1;
	x1 = x2;
	x2 = x;
	}
	
for (x = x1; x <= x2; ++x) SetPixel(x,y);
}

//A couple routines that make Live easier when toying around with plygons
virtual void TranslatePoly(BPoint *Points[], int numPoint, float byX, float byY);
virtual void RotatePoly(BPoint *Points[], int numPoint, float angle, float x = 0.0 , float y = 0.0 );
virtual void ScalePoly(BPoint *Points[], int numPoint, float scale);
virtual void ScalePoly(BPoint *Points[], int numPoint, float x, float y, float scale);
virtual BRect *GetPolyLimits(BPoint *Points[], int numPoint);


virtual void Line(int x1, int y1, int x2, int y2);
virtual void Line(BPoint A, BPoint B); 
virtual void Rectangle(int x1, int y1, int x2, int y2, bool fill = false); //ordinary rectangle
virtual void Rectangle(BPoint A, BPoint B, bool fill = false); //ordinary rectangle
virtual void SRectangle(int x1, int y1, int width, int height, float angle, bool fill = false); //a Rectangle with the abilty to be rotated
virtual void Triangle(BPoint A, BPoint B, BPoint C, bool fill = false); //draws a triangle
virtual void FillSimplePoly(BPoint *Points[], int numPoints); //This polygon routine relies on the fact that no internal angle is greater than 180°
virtual void FillComplexPoly(BPoint *Points[], int numPoints); //breaks down complex Polygons into simple ones and draws them then
virtual void Polygon(BPoint *Points[], int numPoints, bool closed = true); //draws the outline of a polygon
virtual void Spline(int x1, int y1, int x2, int y2, int chx1, int chy1, int chx2, int chy2, int steps = 20, bool drawhandles = false);
virtual void Spline(BPoint *Points[], int numPoints, int steps = 20);
virtual void Circle( int x, int y, int r, bool fill = false);
virtual void Ellipse(int x, int y, int rx, int ry, float angle = 0.0, bool fill = false, int quality = 4);
//virtual void Segment(int x, int y, int rx, int ry, int angle, int stangle, int endangle, bool fill = false);
//virtual BPoint DrawString(BString string , BPoint P);

//virtual void aLine(int x1, int y1, int x2, int y2, bool pattern = false);
//virtual void aLine(BPoint A, BPoint B);
//virtual void aRectangle(int x1, int y1, int x2, int y2, bool fill = false,bool pattern = false);
//virtual void aSRectangle(int x1, int y1, int width, int height, int angle, bool fill = false,bool pattern = false);
//virtual void aTriangle(BPoint A, BPoint B, BPoint C, bool fill = false);
//virtual void aFillSimplePoly(BPoint *FirstPoint[], int numPoints);
//virtual void aCircle( int x, int y, int r, bool fill = false,bool pattern = false);
//virtual void aEllipse(int x, int y, int rx, int ry, int angle, bool fill = false,bool pattern = false);
//virtual void aSegment(int x, int y, int rx, int ry, int angle, int stangle, int endangle, bool fill = false, bool pattern = false);
//virtual BPoint aDrawString(BString string , BPoint P);

//virtual void pLine(int x1, int y1, int x2, int y2);
//virtual void pRectangle(int x1, int y1, int x2, int y2, bool fill = false);
//virtual void pSRectangle(int x1, int y1, int width, int height, int angle, bool fill = false);
//virtual void pTriangle(BPoint A, BPoint B, BPoint C, bool fill = false);
//virtual void pFillSimplePoly(BPoint *FirstPoint[], int numPoints);
//virtual void pCircle( int x, int y, int r, bool fill = false);
//virtual void pEllipse(int x, int y, int rx, int ry, int angle, bool fill = false);
//virtual void pSegment(int x, int y, int rx, int ry, int angle, int stangle, int endangle, bool fill = false);
//virtual BPoint pDrawString(BString string , BPoint P);


virtual void ClearBitmap();
virtual void ClearBitmap(unsigned int data);
virtual void ClearBitmap(unsigned char red, unsigned char green, unsigned char blue,unsigned char alpha = 255);
virtual void MakeBitmapAlpha(unsigned int alpha);

/*
virtual void ExchangeColors(unsigned char r1, unsigned char g1, unsigned char b1,
							unsigned char r2, unsigned char g2, unsigned char b2);
virtual void ChangeColorRange(unsigned char r1, unsigned char g1, unsigned char b1,
							unsigned char r2, unsigned char g2, unsigned char b2);
virtual void ExchangeColors(unsigned char r1, unsigned char g1, unsigned char b1,
							unsigned char r2, unsigned char g2, unsigned char b2, BRegion *constrain);
virtual void ChangeColorRange(unsigned char r1, unsigned char g1, unsigned char b1,
							unsigned char r2, unsigned char g2, unsigned char b2, BRegion *constrain);
*/
//virtual void MakeTransparentColor(unsigned char r1,unsigned char g1, unsigned char b1, unsigned char transparency);
//virtual void MakeColorRangeTransparent(unsigned char r1,unsigned char g1, unsigned char b1, unsigned char transparency);
//virtual void MakeTransparentColor(unsigned char r1,unsigned char g1, unsigned char b1, unsigned char transparency, BRegion *constrain);
//virtual void MakeColorRangeTransparent(unsigned char r1,unsigned char g1, unsigned char b1, unsigned char transparency, BRegion *constrain);
//virtual void MakeRegionTransparent(BRegion *op_region, unsigned char transparency);

//virtual void ChangeContrast( int grade );
//virtual void ChangeContrast( int grade, BRegion *constrain);
//virtual void ChangeBrightness ( int grade );
//virtual void ChangeBrightness ( int grade, BRegion *constrain);
//virtual void ChangeColorDynamincs (int grade);
//virtual void ChangeColorDynamincs (int grade, BRegion *constrain);
//virtual void ChangeRedDynamincs (int grade);
//virtual void ChangeRedDynamincs (int grade, BRegion *constrain);
//virtual void ChangeGreenDynamincs (int grade);
//virtual void ChangeGreenDynamincs (int grade, BRegion *constrain);
//virtual void ChangeBlueDynamincs (int grade);
//virtual void ChangeBlueDynamincs (int grade, BRegion *constrain);
//virtual void ChangeAlphaDynamincs (int grade);
//virtual void ChangeAlphaDynamincs (int grade, BRegion *constrain);
//virtual void Sharpness(int grade);

virtual void Rotate(float angle);
//virtual void Rotate(int angle, BRegion to_be_rotated, bool cut, int region_transparency);
virtual void Resize(BRect newsize);
virtual void Truncate(BRect newsize);

//virtual void EffektInvert();
//virtual void EffektInvert(Bregion *region);
//virtual void EffektMove(int vecx, int vecy);
//virtual void EffektMove(int vecx, int vecy, BRegion *region);
//virtual void EffektWater(int grade);
//virtual void EffektWater(int grade, BRegion *region);
//virtual void EffektPaint(int size, int speed );
//virtual void EffektPaint(int size, int speed, Bregion *region);
//virtual void EffektGreyScales(int number);
//virtual void EffektGreyScales(int number, BRegion *region);
//virtual void EffektBlackWhite(int grade);
//virtual void EffektBlackWhite(int grade, BRegion *region);


//virtual void Fill(int x, int y);
//virtual void Fill(int x, int y, int tolerance_red, int tolerance_green, int tolerance_blue);

/*
virtual void CreatePattern(int x, int y, int width, int height, int style = 0 ...);
virtual void SetPattern(int x, int y, alBitmap *bitmap);
virtual alBitmap *GetPatternBitmap();
*/

virtual alBitmap *Copy(BRegion *tobecopied);
virtual alBitmap *Cut(BRegion *tobecopied);
};