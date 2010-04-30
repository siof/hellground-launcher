
#include <wx/wx.h>
#include <wx/process.h>
#include <wx/socket.h>
#include <wx/html/htmlwin.h>
#include <wx/taskbar.h>

#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#define LAUNCHER_VERSION    "0.0.1"
                        //logonhg.gamefreedom.pl
#define ADRES wxString("logonhg.gamefreedom.pl")

#ifdef WIN32
    #define wxEXEC_NOEVENTS 0
#endif

enum
{
    MAIN_FRAME_WIDTH   = 800,
    MAIN_FRAME_HEIGHT  = 615,

    THREAD_SLEEP_INTERVAL = 10000,
};

enum Buttons
{
    BUTTON_HOME = 0,
    BUTTON_FORUM,
    BUTTON_ARMORY,
    BUTTON_PANEL,
    //BUTTON_WIKI,
    BUTTON_PLAY,
    MAIN_FRAME_BUTTONS = 5,//6,
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
    ID_CHECK_BOX,
    ID_TASKBAR,
    ID_BACKGROUND,
};

class TaskBar : public wxTaskBarIcon
{
    public:
        TaskBar(){}
        void OnLeftButtonDClick(wxTaskBarIconEvent&);
        void OnMenuShowHide(wxCommandEvent&);
        void OnMenuExit(wxCommandEvent&);
    private:
        virtual wxMenu *CreatePopupMenu();

DECLARE_EVENT_TABLE()
};

class ACThread : public wxThread
{
    public:
        ACThread(wxSocketClient *sock) : wxThread(), m_sock(sock) {loop = true;}

        void Wait()
        {
            loop = false;
            wxThread::Wait();
        }

    private:
        bool loop;
        wxSocketClient *m_sock;
        void *Entry();
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
        void OnArmory(wxCommandEvent &){wxLaunchDefaultBrowser(wxString("http://armoryhg.dof.nazwa.pl/"));}; // unused bo nie dziala ;S
        void OnWiki(wxCommandEvent &)  {wxLaunchDefaultBrowser(wxString("http://hgwiki.gamefreedom.pl/"));};
        void OnPlay(wxCommandEvent &);
        void OnTaskBarLeftClick(wxTaskBarIconEvent &);

        void OnWoWClose(wxProcessEvent &)
        {
            m_button[BUTTON_PLAY]->Enable();
            this->Show();
        }

    private:
        wxPanel  *m_panel;
        wxBitmapButton *m_button[MAIN_FRAME_BUTTONS];
        wxHtmlWindow *m_html;
        wxCheckBox * m_checkbox;
        wxSocketClient *m_sock;
        wxBitmapButton *m_background;
        TaskBar *m_taskbar;

        ACThread *m_thread;

        wxIPV4address m_ip;

        //DECLARE_EVENT_TABLE()
};
#endif
