

#include "wx/wx.h"


#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

enum
{
    MAIN_FRAME_WIDTH   = 600,
    MAIN_FRAME_HEIGHT  = 480,
    MAIN_FRAME_BUTTONS = 6,
};

enum
{
    ID_HOME = 1,
    ID_FORUM,
    ID_PANEL,
    ID_ARMORY,
    ID_WIKI,
    ID_PLAY,
};

class MainFrame : public wxFrame
{
    public:
        MainFrame(const wxString& title);
        ~MainFrame();

        DECLARE_EVENT_TABLE();

        void OnHome(wxCommandEvent &)  {wxLaunchDefaultBrowser(wxString("http://wow.gamefreedom.pl/"));};
        void OnForum(wxCommandEvent &) {wxLaunchDefaultBrowser(wxString("http://gamefreedom.pl/"));};
        void OnPanel(wxCommandEvent &) {wxLaunchDefaultBrowser(wxString("http://panel.gamefreedom.pl/"));};
        void OnArmory(wxCommandEvent &){wxLaunchDefaultBrowser(wxString("http://gogle.pl"));}; // unused bo nie dziala ;S
        void OnWiki(wxCommandEvent &)  {wxLaunchDefaultBrowser(wxString("http://hgwiki.gamefreedom.pl/"));};
        void OnPlay(wxCommandEvent &)  {};

    private:
        wxPanel  *m_panel;
        wxButton *m_button[MAIN_FRAME_BUTTONS];

        bool win_process_scan();

        //DECLARE_EVENT_TABLE()
};
#endif
