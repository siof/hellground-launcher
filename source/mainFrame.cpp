#include "mainFrame.h"

bool MainFrame::win_process_scan()
{
    wxString cheat("wowemuhack");
    wxString cmd("tasklist");
    wxArrayString output, error;
    wxExecute(cmd, output, error);

    for (wxArrayString::iterator iter = output.begin(); iter != output.end(); ++iter)
    {
        (*iter).LowerCase();
        if ((*iter).Contains(cheat))
            return true;
    }
    return false;
}


MainFrame::MainFrame(const wxString& title)
: wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxSize(MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT),
          wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxFRAME_SHAPED)
{
    Center();
    m_panel = new wxPanel(this, wxID_ANY);

    int y = 50;
    for (int i = 0; i < MAIN_FRAME_BUTTONS; i++)
        m_button[i] = new wxButton(m_panel, wxID_ANY, (wxChar *)"sadas", wxPoint(70, y +(y*i)));

    //wxProcess *test = wxProcess::Open("start wow.exe");
    //wxMenu *menuFile = new wxMenu;

    //menuFile->Append( ID_About, (wxChar *)"&About..." );
    //menuFile->AppendSeparator();
    //menuFile->Append( ID_Quit, (wxChar *)"&Exit" );
 
    //wxMenuBar *menuBar = new wxMenuBar;
    //menuBar->Append( menuFile, (wxChar *)"&File" );
 
    //SetMenuBar( menuBar );

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
 
void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

