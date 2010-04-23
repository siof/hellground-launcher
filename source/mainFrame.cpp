#include "mainFrame.h"
#include <wx/socket.h>

static wxArrayString cheat_list;
static wxArrayString cmd;

enum
{
    CMD_WINDOWS = 0,
    CMD_LINUX   = 1,
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_BUTTON( ID_HOME,   MainFrame::OnHome)
  EVT_BUTTON( ID_FORUM,  MainFrame::OnForum)
  EVT_BUTTON( ID_PANEL,  MainFrame::OnPanel)
  EVT_BUTTON( ID_WIKI,   MainFrame::OnWiki)
  EVT_BUTTON( ID_ARMORY, MainFrame::OnArmory)
  EVT_BUTTON( ID_PLAY,   MainFrame::OnPlay)
END_EVENT_TABLE()


void init_cheat_list()
{
    // tu komendy pod rozne systemy
    cmd.Add(wxString("tasklist"));
    cmd.Add(wxString("ps -A"));
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


bool MainFrame::win_process_scan()
{
#ifdef WIN32
    wxString command = cmd[CMD_WINDOWS];
#else
    wxString command = cmd[CMD_LINUX];
#endif

    wxArrayString output, error;
    wxExecute(command, output, error);

    for (wxArrayString::iterator iter = output.begin(); iter != output.end(); ++iter)
        if (scan(*iter))
            return true;
    return false;
}

MainFrame::MainFrame(const wxString& title)
: wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxSize(MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT),
          wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxFRAME_SHAPED)
{
    Center();
    m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT));

    m_button[0] = new wxButton(m_panel, ID_HOME, wxString("HG Home"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 10), wxSize(120, 60));

    m_button[1] = new wxButton(m_panel, ID_FORUM, wxString("HG Forum"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 80), wxSize(120, 60));

    m_button[2] = new wxButton(m_panel, ID_PANEL, wxString("HG Panel Gracza"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 150), wxSize(120, 60));

    m_button[3] = new wxButton(m_panel, ID_ARMORY, wxString("HG Armory"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 220), wxSize(120, 60));

    m_button[4] = new wxButton(m_panel, ID_WIKI, wxString("HG WoWWiki"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 290), wxSize(120, 60));

    m_button[5] = new wxButton(m_panel, ID_PLAY, wxString("Play WoW!"),
                               wxPoint(MAIN_FRAME_WIDTH-130, 360), wxSize(120, 60));

    //wxProcess *test = wxProcess::Open("start wow.exe");
    //wxMenu *menuFile = new wxMenu;

    //menuFile->Append( ID_About, (wxChar *)"&About..." );
    //menuFile->AppendSeparator();
    //menuFile->Append( ID_Quit, (wxChar *)"&Exit" );
 
    //wxMenuBar *menuBar = new wxMenuBar;
    //menuBar->Append( menuFile, (wxChar *)"&File" );
 
    //SetMenuBar( menuBar );

    init_cheat_list();

    bool noob = win_process_scan();
    // nie wiem jaka to flaga 65536 w define, ale usuwamy domyslna 16 | 65536 bo brzydko wygldala
    CreateStatusBar(1, 65536);
    if(noob)
        SetStatusText( (wxChar *)"CHEAT FOUND" );
    else
        SetStatusText( (wxChar *)"CHEAT NOT FOUND" );
}

MainFrame::~MainFrame()
{
    for (int i = 0; i < MAIN_FRAME_BUTTONS; i++)
        delete m_button[i];
    delete m_panel;
}

/*void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}*/
