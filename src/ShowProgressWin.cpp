#include "MatrixFunctionality.h"

extern ShowProgressWin *theone;

void HandleProgress(BMessage *message, alHandler *thisone)
{
switch (message->what)
	{
	case 67:
		{
		theone->stat_bar->Update(1,"","");
		break;
		}
	}
}

ShowProgressWin::ShowProgressWin(float width, float height, const char *text, float range):alNormalWindow(NULL,width,height,text)
{
stat_bar = new BStatusBar (BRect (20,30,380,60),"Name","0%","100%");

alHandler *x = new alHandler();
x->MessageReceived = &HandleProgress;
AnyMessageHandler = x;

AddChild(stat_bar);
}

ShowProgressWin::~ShowProgressWin()
{
tonotify->AnyMessageHandler->Parent = (char *) tonotify;
tonotify->PostMessage(10);
toshutLooper->PostMessage(2);
stat_bar->RemoveSelf();




for ( int i=0; i < threads; ++i)
	{
	thread_id tid;
	tid = threadid[i];
	kill_thread (tid);
	}

delete stat_bar;
delete [] threadid;
}

void ShowProgressWin::SetKnobTo(float value)
{
}