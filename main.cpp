#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/textfile.h>
#include <wx/string.h>
#include <iostream>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void OnExit(wxCommandEvent &event);
    void OnOpen(wxCommandEvent &event);
    void OnEnter(wxCommandEvent &event);
    void OnButtonClick(wxCommandEvent &event);
    wxDECLARE_EVENT_TABLE();
    wxTextCtrl *m_textCtrl;
    wxTextFile tfile;
};
enum
{
    ID_TextCtrl = 1,
    ID_Button
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_TEXT_ENTER(ID_TextCtrl, MyFrame::OnEnter)
        EVT_BUTTON(ID_Button, MyFrame::OnButtonClick)
            EVT_MENU(wxID_EXIT, MyFrame::OnExit)
                EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
                    wxEND_EVENT_TABLE()
                        wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));

    frame->Show(true);
    return true;
}
MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);

    wxMenuBar *menuBar = new wxMenuBar;
    wxMenu *file = new wxMenu;
    file->Append(wxID_OPEN, wxT("&Open"));
    menuBar->Append(file, "&Open");
    menuBar->Append(menuFile, "&Exit");
    SetMenuBar(menuBar);

    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    m_textCtrl = new wxTextCtrl(panel, ID_TextCtrl, "Enter text and press Enter", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxButton *button = new wxButton(panel, ID_Button, "Get Value");
    hbox->Add(m_textCtrl, 1, 20);
    hbox->Add(button, 1, wxALL, 5);
    panel->SetSizer(hbox);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
}
void MyFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}
void MyFrame::OnOpen(wxCommandEvent &event)
{
    wxFileDialog *openFileDialog = new wxFileDialog(this);

    if (openFileDialog->ShowModal() == wxID_OK)
    {
        wxString filename = openFileDialog->GetPath();

        if (!tfile.Open(filename))
        {
            // Handle error (e.g., file not found or read error)
            wxLogError("Cannot open file %s", filename);
            return;
        }
    }
}

void MyFrame::OnEnter(wxCommandEvent &event)
{
    wxString value = m_textCtrl->GetValue(); // Get the input value
    wxMessageBox(wxString::Format("You entered via ENTER: %s", value), "Input Received", wxOK | wxICON_INFORMATION, this);
    SetStatusText(wxString::Format("Text entered: %s", value));
}

void MyFrame::OnButtonClick(wxCommandEvent &event)
{
    wxString value = m_textCtrl->GetValue(); // Get the input value

    // Read and process the file line by line
    for (size_t i = 0; i < tfile.GetLineCount(); ++i)
    {
        wxString line = tfile.GetLine(i);
        size_t position = line.Find(value);

        if (position != wxString::npos)
        {
            wxLogMessage("'%s' found in the string at index: %zu", line, position);
            break;
            // Output will be: 'fox' found in the string at index: 16
        }
        else
        {
            continue;
        }
    }
    // wxMessageBox(wxString::Format("You entered via Button: %s", value), "Input Received", wxOK | wxICON_INFORMATION, this);
}