
#include <wx/wx.h>
#include <wx/process.h>
#include <wx/html/htmlwin.h>


#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#define LAUNCHER_VERSION    "0.0.1"

enum
{
    MAIN_FRAME_WIDTH   = 600,
    MAIN_FRAME_HEIGHT  = 480,
};

enum Buttons
{
    BUTTON_HOME = 0,
    BUTTON_FORUM,
    BUTTON_PANEL,
    BUTTON_ARMORY,
    BUTTON_WIKI,
    BUTTON_PLAY,
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
    ID_WOW_PROCESS,
    ID_HTML,
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
        void OnPlay(wxCommandEvent &);

        void OnWoWClose(wxProcessEvent &) {m_button[BUTTON_PLAY]->Enable();}

    private:
        wxPanel  *m_panel;
        wxButton *m_button[MAIN_FRAME_BUTTONS];
        wxHtmlWindow *m_html;

        bool win_process_scan();

        //DECLARE_EVENT_TABLE()
};
#endif
