
#include "wx/wx.h"
#include "mainFrame.h"

class LauncherApp: public wxApp
{
    virtual bool OnInit();
};


enum
{
    ID_Quit = 1,
};
 
/*BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
END_EVENT_TABLE()
*/

IMPLEMENT_APP(LauncherApp)

bool LauncherApp::OnInit()
{
    MainFrame *frame = new MainFrame(wxString("Hellground Launcher"));
    frame->Show();
    SetTopWindow(frame);
    return true;
}

/*
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox((wxChar *)"This is a wxWindows Hello world sample",
        (wxChar *)"About Hello World", wxOK | wxICON_INFORMATION, this);
}*/
