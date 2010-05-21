#include "mainFrame.h"
#include "grafiki.h"       //plik z includami grafik

#include <wx/process.h>
#include <wx/socket.h>
#include <wx/html/htmlwin.h>
#include <wx/file.h>
#include <wx/bitmap.h>
#include <wx/protocol/http.h>
#include <wx/sstream.h>

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
    cheat_list.Add(wxString("wowporty"));
    cheat_list.Add(wxString("wmove"));
    cheat_list.Add(wxString("mmowned"));
    cheat_list.Add(wxString("gatherbuddy"));
    cheat_list.Add(wxString("auktionator"));
    cheat_list.Add(wxString("wow tracker"));
    cheat_list.Add(wxString("hack"));
    cheat_list.Add(wxString("cheat engine"));
    cheat_list.Add(wxString("cheat"));
    cheat_list.Add(wxString("bh-tool"));
}

bool scan(wxString &string_to_scan)
{
    string_to_scan.LowerCase();
    for (wxArrayString::const_iterator cheat = cheat_list.begin(); cheat != cheat_list.end(); ++cheat)
        if (string_to_scan.Contains(*cheat))
        #ifdef LINUX
            if (!string_to_scan.Contains("ps -A f"))
        #endif
                return true;

    return false;
}

bool process_scan()
{
#ifdef WIN32
    wxString command = "tasklist /V";
#else
    wxString command = "ps -A f";
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

    //m_html = new wxHtmlWindow(this, ID_HTML, wxPoint(0,100), wxSize(MAIN_FRAME_WIDTH, 348), wxHW_SCROLLBAR_NEVER);

    m_info[INFO_MAIN] = new wxStaticText(this, ID_INFO_MAIN, "", wxPoint(50, 100), wxSize(400, 175), wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
    m_info[INFO_CHANGELOG] = new wxStaticText(this, ID_INFO_CHANGELOG, "", wxPoint(50, 325), wxSize(400, 125), wxALIGN_LEFT|wxST_NO_AUTORESIZE);
    m_info[INFO_OTHER] = new wxStaticText(this, ID_INFO_OTHER, "", wxPoint(500, 100), wxSize(250, 350), wxALIGN_CENTRE|wxST_NO_AUTORESIZE);


    // TODO: fix it for windows
    //m_html->LoadPage("66.102.13.147");

    for (int i = 0; i < INFO_COUNT; i++)
    {
        wxHTTP get;
        get.SetHeader(_T("Content-Type"), _T("text/html; charset=utf-8"));
        get.SetTimeout(10); //10 sekund zamiast 10 minut

        wxInputStream * httpInput;

        if (get.Connect(_T("www.fumetsu.cba.pl")))
        {
            httpInput = get.GetInputStream(*InfoFileName(i));

            if (get.GetError() == wxPROTO_NOERR)
            {               
                wxString out;

                wxStringOutputStream outStream(&out);
                httpInput->Read(outStream);

                m_info[i]->SetOwnForegroundColour(wxColour("WHITE"));
                m_info[i]->SetLabel(out);
            }
            else
            {
                m_info[i]->SetLabel("B³¹d po³¹czenia !!");
            }
        }
        else
        {
            m_info[i]->SetLabel("B³¹d ³¹czenia z serwerem http !!");
        }
    }

    s_ip.Hostname(ADRES);
    s_ip.Service(5600);

    m_sock = new wxSocketClient();

    m_thread = new ACThread(m_sock);
    m_thread->Create();
    m_thread->Run();

    #ifdef WIN32
    m_background = new wxBitmapButton(m_panel, ID_BACKGROUND, wxBitmap(tloXpm), wxPoint(0, -20), wxSize(MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT-30));
    m_background->Disable();
    #else
    m_background = new wxStaticBitmap(m_panel, ID_BACKGROUND, wxBitmap(tloXpm), wxPoint(0, -20), wxSize(MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT-30));
    #endif

    m_button[BUTTON_HOME] = new wxBitmapButton(m_panel, ID_HOME, wxBitmap(btnHomeXpm),
                               wxPoint(HOME_POS_X, HOME_POS_Y), wxSize(BUTTON_WIDTH, BUTTON_HEIGHT), wxBORDER_NONE);

    m_button[BUTTON_FORUM] = new wxBitmapButton(m_panel, ID_FORUM, wxBitmap(btnForumXpm),
                               wxPoint(FORUM_POS_X, FORUM_POS_Y), wxSize(BUTTON_WIDTH, BUTTON_HEIGHT), wxBORDER_NONE);

    m_button[BUTTON_ARMORY] = new wxBitmapButton(m_panel, ID_ARMORY, wxBitmap(btnArmoryXpm),
                               wxPoint(ARMORY_POS_X, ARMORY_POS_Y), wxSize(BUTTON_WIDTH, BUTTON_HEIGHT), wxBORDER_NONE);

    m_button[BUTTON_PANEL] = new wxBitmapButton(m_panel, ID_PANEL, wxBitmap(btnPanelXpm),
                               wxPoint(PANEL_POS_X, PANEL_POS_Y), wxSize(BUTTON_WIDTH, BUTTON_HEIGHT), wxBORDER_NONE);

    m_button[BUTTON_PLAY] = new wxBitmapButton(m_panel, ID_PLAY, wxBitmap(btnPlayXpm),
                               wxPoint(PLAY_POS_X, PLAY_POS_Y), wxSize(BUTTON_WIDTH, BUTTON_HEIGHT-15), wxBORDER_NONE);

    m_checkbox = new wxCheckBox(m_panel, ID_CHECK_BOX, wxString(/*"-opengl"*/""), wxPoint(646, 465));
    //ustaw checkboxa zaznaczonego domyslnie
#ifdef WIN32
    m_checkbox->SetValue(false);
#else
    m_checkbox->SetValue(true);
#endif

    m_taskbar = new TaskBar();

#ifdef LINUX
    if (m_taskbar->IsAvailable())
#endif
    {
        m_taskbar->SetIcon(wxIcon(wow_xpm), wxString("HG Launcher"));
    }

    init_cheat_list();

    // nie wiem jaka to flaga 65536 w define, ale usuwamy domyslna 16 | 65536 bo brzydko wygladala
    CreateStatusBar(1, 65536);
    SetStatusText("HG Launcher v"
                  LAUNCHER_VERSION);

    SetIcon(wxIcon(wow_xpm));
    //m_html->SetBackgroundImage(wxBitmap(htmlBoxXpm));

    m_button[BUTTON_HOME]->SetBitmapHover(wxBitmap(btnHomeHXpm));
    m_button[BUTTON_FORUM]->SetBitmapHover(wxBitmap(btnForumHXpm));
    m_button[BUTTON_ARMORY]->SetBitmapHover(wxBitmap(btnArmoryHXpm));
    m_button[BUTTON_PANEL]->SetBitmapHover(wxBitmap(btnPanelHXpm));
    m_button[BUTTON_PLAY]->SetBitmapHover(wxBitmap(btnPlayHXpm));
}

MainFrame::~MainFrame()
{
    //m_thread->Wait();
    for (int i = 0; i < MAIN_FRAME_BUTTONS; i++)
        delete m_button[i];
    //delete m_html;
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
    realmlist.Write("set realmlist " + ADRES);
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
    while (loop)
    {
        if (!m_sock->IsConnected())
            m_sock->Connect(s_ip, true);

        if (m_sock->IsConnected())
        {
            // found cheat - send cheat kick message
            if (process_scan())
            {
                m_sock->Write("Kc", 2);
                Sleep(THREAD_SLEEP_INTERVAL);
                continue;
            }
            //m_sock->Write("P_", 2);
        }
        else
            m_sock->Close();
        Sleep(THREAD_SLEEP_INTERVAL);
    }
    return 0;
}

wxString * MainFrame::InfoFileName(int info)
{
    wxString * tmp;
    switch(info)
    {
    case 0:
        tmp = new wxString("/test1.txt");
        break;
    case 1:
        tmp = new wxString("/test2.txt");
        break;
    case 2:
        tmp = new wxString("/test3.txt");
        break;
    }

    return tmp;
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
