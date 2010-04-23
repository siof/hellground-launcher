
#include <wx/wx.h>
#include "mainFrame.h"

class LauncherApp: public wxApp
{
    virtual bool OnInit();
};

IMPLEMENT_APP(LauncherApp)

bool LauncherApp::OnInit()
{
    MainFrame *frame = new MainFrame(wxString("Hellground Launcher"));
    frame->Show();
    SetTopWindow(frame);
    return true;
}
