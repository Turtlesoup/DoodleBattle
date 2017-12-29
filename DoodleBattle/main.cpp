#include "main.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    BaseApplication *app = new BaseApplication();
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("Doodle Battle");
    //eglView->setFrameSize(320, 480);
	eglView->setFrameSize(1536, 2048);
    // The resolution of ipad3 is very large. In general, PC's resolution is smaller than it.
    // So we need to invoke 'setFrameZoomFactor'(only valid on desktop(win32, mac, linux)) to make the window smaller.
    eglView->setFrameZoomFactor(0.35f);
    return CCApplication::sharedApplication()->run();
}