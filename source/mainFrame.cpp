
#include "mainFrame.h"

#include <wx/process.h>
#include <wx/socket.h>
#include <wx/html/htmlwin.h>
#include <wx/file.h>

MainFrame * window;

static wxArrayString cheat_list;

static wxIPV4address s_ip;

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_BUTTON( ID_HOME,   MainFrame::OnHome)
  EVT_BUTTON( ID_FORUM,  MainFrame::OnForum)
  EVT_BUTTON( ID_PANEL,  MainFrame::OnPanel)
  EVT_BUTTON( ID_WIKI,   MainFrame::OnWiki)
  EVT_BUTTON( ID_ARMORY, MainFrame::OnArmory)
  EVT_BUTTON( ID_PLAY,   MainFrame::OnPlay)
  EVT_END_PROCESS( ID_WOW_PROCESS, MainFrame::OnWoWClose)
END_EVENT_TABLE()


void init_cheat_list()
{
    // tu dodajemy wpisy ktore ma sprawdzic
    cheat_list.Add(wxString("wowemuhack"));
    cheat_list.Add(wxString("wpe"));
}

bool scan(wxString &string_to_scan)
{
    string_to_scan.LowerCase();
    for (wxArrayString::const_iterator cheat = cheat_list.begin(); cheat != cheat_list.end(); ++cheat)
        if (string_to_scan.Contains(*cheat))
            return true;
    return false;
}

bool process_scan()
{
#ifdef WIN32
    wxString command = "tasklist /V";
#else
    wxString command = "ps -A";
#endif

    wxArrayString output, error;

    wxExecute(command, output, error, wxEXEC_NOEVENTS);

    for (wxArrayString::iterator iter = output.begin(); iter != output.end(); ++iter)
        if (scan(*iter))
            return true;
    return false;
}

MainFrame::MainFrame(const wxString& title)
: wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxSize(MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT),
          wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxFRAME_SHAPED)
{
    window = this;
    Center();
    wxInitAllImageHandlers();
    m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT));

    m_html = new wxHtmlWindow(this, ID_HTML, wxPoint(5,5), wxSize(MAIN_FRAME_WIDTH-140, MAIN_FRAME_HEIGHT-65), wxHW_SCROLLBAR_NEVER);
    // TODO: fix it for windows
    //m_html->LoadPage("http://wow.gamefreedom.pl");

    s_ip.Hostname("logonhg.gamefreedom.pl");
    s_ip.Service(5600);

    m_sock = new wxSocketClient();

    m_thread = new ACThread(m_sock);
    m_thread->Create();
    m_thread->Run();

    m_button[BUTTON_HOME] = new wxButton(m_panel, ID_HOME, wxString("HG Home"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 10), wxSize(120, 60));

    m_button[BUTTON_FORUM] = new wxButton(m_panel, ID_FORUM, wxString("HG Forum"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 80), wxSize(120, 60));

    m_button[BUTTON_PANEL] = new wxButton(m_panel, ID_PANEL, wxString("HG Panel Gracza"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 150), wxSize(120, 60));

    m_button[BUTTON_ARMORY] = new wxButton(m_panel, ID_ARMORY, wxString("HG Armory"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 220), wxSize(120, 60));

    m_button[BUTTON_WIKI] = new wxButton(m_panel, ID_WIKI, wxString("HG WoWWiki"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 290), wxSize(120, 60));

    m_button[BUTTON_PLAY] = new wxButton(m_panel, ID_PLAY, wxString("Play WoW!"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 360), wxSize(120, 60));

#ifdef LINUX
    m_checkbox = new wxCheckBox(m_panel, ID_CHECK_BOX, wxString("-opengl"), wxPoint(MAIN_FRAME_WIDTH-100, 425));
    //ustaw checkboxa zaznaczonego domyslnie
    m_checkbox->SetValue(true);
#endif

    m_button[BUTTON_ARMORY]->Disable(); //tymczasowo do czasu wlaczenia armory

    m_taskbar = new TaskBar();

#ifdef LINUX
    if (m_taskbar->IsAvailable())
#endif
    {
        m_taskbar->SetIcon(wxIcon(wxString("wow.png"), wxBitmapType(wxBITMAP_TYPE_PNG)), wxString("HG Launcher"));
    }

    init_cheat_list();

    // nie wiem jaka to flaga 65536 w define, ale usuwamy domyslna 16 | 65536 bo brzydko wygladala
    CreateStatusBar(1, 65536);
    SetStatusText("HG Launcher v"
                  LAUNCHER_VERSION);
}

MainFrame::~MainFrame()
{
    for (int i = 0; i < MAIN_FRAME_BUTTONS; i++)
        delete m_button[i];
    delete m_html;
    delete m_sock;
    delete m_thread;
    delete m_taskbar;
#ifdef LINUX
    delete m_checkbox;
#endif
    delete m_panel;

    window = NULL;
}

void MainFrame::OnPlay(wxCommandEvent &)
{
    wxFile realmlist("realmlist.wtf", wxFile::write);
    realmlist.Write("set realmlist logonhg.gamefreedom.pl");
    realmlist.Close();

    wxString cmd;
#ifdef WIN32
    cmd = "Wow.exe";
#else
#ifdef LINUX
    if (m_checkbox->IsChecked())
        cmd = "wine WoW.exe -opengl";
    else
        cmd = "wine WoW.exe";
#else
    //cmd = ; //kod dla maca
#endif
#endif

    SetStatusText(wxString("Launching World of Warcraft. Please wait..."));
    wxProcess *process = new wxProcess(this, ID_WOW_PROCESS);
    long int pid = wxExecute(cmd, wxEXEC_ASYNC, process);

    if (!process->Exists(pid))
    {
        SetStatusText(wxString("ERROR! Couldn't find WoW.exe! "
                               "Launcher must be in World of Warcraft installation directory!"));
        delete process;
        return;
    }
    m_button[BUTTON_PLAY]->Disable();
    window->Hide();
}

void *ACThread::Entry()
{
    while (1)
    {
        if (!m_sock->IsConnected())
            m_sock->Connect(s_ip, true);

        if (m_sock->IsConnected())
        {
            // found cheat - send cheat kick message
            if (process_scan())
                m_sock->Write("Kc", 2);
        }
        else
            m_sock->Close();
        Sleep(THREAD_SLEEP_INTERVAL);
    }
    return 0;
}

//---------------------------------------------------------TaskBar

enum
{
    SHOW_HIDE = 10001,
    EXIT,
};

BEGIN_EVENT_TABLE(TaskBar, wxTaskBarIcon)
    EVT_MENU(SHOW_HIDE, TaskBar::OnMenuShowHide)
    EVT_MENU(EXIT,    TaskBar::OnMenuExit)
    EVT_TASKBAR_LEFT_DCLICK(TaskBar::OnLeftButtonDClick)
END_EVENT_TABLE()

void TaskBar::OnMenuShowHide(wxCommandEvent& )
{
    if (window->IsShown())
        window->Hide();
    else
        window->Show();
}

void TaskBar::OnMenuExit(wxCommandEvent& )
{
    window->Close(true);
}

wxMenu *TaskBar::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;
    menu->Append(SHOW_HIDE, _T("Show/Hide Launcher"));
    menu->AppendSeparator();
    menu->Append(EXIT, _T("Exit"));
    return menu;
}

void TaskBar::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
    if (window->IsShown())
        window->Hide();
    else
        window->Show();
}

/*void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}*/
