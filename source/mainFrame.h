

#include "wx/wx.h"


#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

enum
{
    MAIN_FRAME_WIDTH   = 600,
    MAIN_FRAME_HEIGHT  = 480,
    MAIN_FRAME_BUTTONS = 5,
};

class MainFrame : public wxFrame
{
    public:
        MainFrame(const wxString& title);
        ~MainFrame();

        void OnQuit(wxCommandEvent& event);

    private:
        wxPanel  *m_panel;
        wxButton *m_button[MAIN_FRAME_BUTTONS];

        bool win_process_scan();

        //DECLARE_EVENT_TABLE()
};
#endif
