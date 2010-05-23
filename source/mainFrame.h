
#include <wx/wx.h>
#include <wx/process.h>
#include <wx/socket.h>
#include <wx/html/htmlwin.h>
#include <wx/taskbar.h>

#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#define LAUNCHER_VERSION    "0.0.1"
                        //logonhg.gamefreedom.pl
#define ADRES           wxString("logonhg.gamefreedom.pl")
#define ADRES_PARSER    wxString("www.logonhg.gamefreedom.pl")
#define PLIK_PARSER     wxString("/parser.irc")
#define REFRESH_TIMER   60000;

#ifdef WIN32
    #define wxEXEC_NOEVENTS 0
#endif

enum
{
    MAIN_FRAME_WIDTH    = 800,
#ifdef WIN32
    MAIN_FRAME_HEIGHT   = 615,
    BUTTON_HEIGHT       = 63,
    BUTTON_WIDTH        = 127,
    HOME_POS_X          = 20,
    HOME_POS_Y          = 474,
    FORUM_POS_X         = 159,
    FORUM_POS_Y         = 474,
    ARMORY_POS_X        = 298,
    ARMORY_POS_Y        = 474,
    PANEL_POS_X         = 437,
    PANEL_POS_Y         = 474,
    PLAY_POS_X          = 628,
    PLAY_POS_Y          = 487,
    INFO_MAIN_POS_X     = 50,
    INFO_MAIN_POS_Y     = 100,
    INFO_CHANGE_POS_X   = 50,
    INFO_CHANGE_POS_Y   = 325,
    INFO_OTHER_POS_X    = 500,
    INFO_OTHER_POS_Y    = 100,
    INFO_ONLINE_POS_X   = 50,
    INFO_ONLINE_POS_Y   = 290,
#else
    MAIN_FRAME_HEIGHT   = 600,
    BUTTON_HEIGHT       = 73,
    BUTTON_WIDTH        = 137,
    HOME_POS_X          = 15,
    HOME_POS_Y          = 469,
    FORUM_POS_X         = 154,
    FORUM_POS_Y         = 469,
    ARMORY_POS_X        = 293,
    ARMORY_POS_Y        = 469,
    PANEL_POS_X         = 432,
    PANEL_POS_Y         = 469,
    PLAY_POS_X          = 623,
    PLAY_POS_Y          = 482,
                                //DO POPRAWY
    INFO_MAIN_POS_X     = 50,
    INFO_MAIN_POS_Y     = 100,
    INFO_CHANGE_POS_X   = 50,
    INFO_CHANGE_POS_Y   = 325,
    INFO_OTHER_POS_X    = 500,
    INFO_OTHER_POS_Y    = 100,
    INFO_ONLINE_POS_X   = 50,
    INFO_ONLINE_POS_Y   = 290,
#endif

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

enum Infos
{
    INFO_MAIN = 0,
    INFO_CHANGELOG,
    INFO_OTHER,
    INFO_COUNT = 3,
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
    ID_INFO_MAIN,
    ID_INFO_CHANGELOG,
    ID_INFO_OTHER,
    ID_INFO_ONLINE,
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

        wxString * InfoFileName(int info);
        void RefreshOnline();

        DECLARE_EVENT_TABLE();

        void OnHome(wxCommandEvent &)  {wxLaunchDefaultBrowser(wxString("http://wow.gamefreedom.pl/"));};
        void OnForum(wxCommandEvent &) {wxLaunchDefaultBrowser(wxString("http://gamefreedom.pl/"));};
        void OnPanel(wxCommandEvent &) {wxLaunchDefaultBrowser(wxString("http://panel.gamefreedom.pl/"));};
        void OnArmory(wxCommandEvent &){wxLaunchDefaultBrowser(wxString("http://armoryhg.dof.nazwa.pl/"));}; // unused bo nie dziala ;S
        void OnWiki(wxCommandEvent &)  {wxLaunchDefaultBrowser(wxString("http://hgwiki.gamefreedom.pl/"));};
        void OnPlay(wxCommandEvent &);
        void OnTaskBarLeftClick(wxTaskBarIconEvent &);
        void MinimazeToTray(wxIconizeEvent &){this->Hide();}

        void OnWoWClose(wxProcessEvent &)
        {
            //m_button[BUTTON_PLAY]->Enable();
            //this->Show();
            this->Close();
        }

    private:
        wxPanel * m_panel;
        wxBitmapButton * m_button[MAIN_FRAME_BUTTONS];
        //wxHtmlWindow *m_html;
        wxCheckBox * m_checkbox;
        wxSocketClient * m_sock;
        wxStaticText * m_info[INFO_COUNT];
        wxStaticText * m_online;
        #ifdef WIN32
        wxBitmapButton * m_background;
        #else
        wxStaticBitmap * m_background;
        #endif
        TaskBar * m_taskbar;

        ACThread * m_thread;

        wxIPV4address m_ip;

        //DECLARE_EVENT_TABLE()
};
#endif
