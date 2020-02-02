#pragma once

#include "wx/wx.h"
#include <wx/wfstream.h>
#include <wx/valnum.h>
#include <wx/filename.h>
#include <wx/progdlg.h>

#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>

#include "ImagePanel.h"

class App : public wxApp {
public:
	virtual bool OnInit();
	virtual int OnExit();
};

class Frame : public wxFrame {
	public:
		Frame(const wxString& title);

		void OnClose(wxCloseEvent&);

	private: // Various wxWidgets classes are allocated on the heap; see https://wiki.wxwidgets.org/Avoiding_Memory_Leaks.
		wxPanel *panel1;

		wxImagePanel *imagePanel;

		wxButton *loadPalette;
		wxButton *exportPalette;
		wxCheckBox *transparencyToggle;
		wxButton *loadTextures;
		wxStaticText *textCurNumber;
		wxButton *prev;
		wxButton *next;
		wxButton *goTo;
		wxTextCtrl *goToNumber;
		wxButton *exportCurrentTexture;
		wxButton *exportAllTextures;
		wxButton *replaceCurrent;
		wxButton *saveTextureFile;
		wxButton* addTexture;
		wxButton *help;
		wxButton *about;

		void OnLoadPalette(wxCommandEvent& WXUNUSED(event));
		void OnExportPalette(wxCommandEvent& WXUNUSED(event));
		void OnTransparencyToggle(wxCommandEvent& WXUNUSED(event));
		void OnLoadTextures(wxCommandEvent& WXUNUSED(event));
		void OnPrev(wxCommandEvent& WXUNUSED(event));
		void OnNext(wxCommandEvent& WXUNUSED(event));
		void OnGoTo(wxCommandEvent& WXUNUSED(event));
		void OnExportCurrentTexture(wxCommandEvent& WXUNUSED(event));
		void OnExportAllTextures(wxCommandEvent& WXUNUSED(event));
		void OnReplaceCurrent(wxCommandEvent& WXUNUSED(event));
		void OnSaveTextureFile(wxCommandEvent& WXUNUSED(event));
		void OnAddTexture(wxCommandEvent& WXUNUSED(event));
		void OnHelp(wxCommandEvent& WXUNUSED(event));
		void OnAbout(wxCommandEvent& WXUNUSED(event));

		std::vector <std::tuple<int, int, int>> palette;

		const int PaletteColourCount = 256;

		wxImage GeneratePaletteImage(int scaleFactor);

		void SaveImage(const wxImage &image);

		std::vector<unsigned char> fileBytes;

		std::vector<int> starts;

		int imageCount = 0;
		int curImage = 0;

		int ReadInt32(wxFile* file, int address);

		int ReadByte(wxFile* file, int address);

		void UpdateImage();

		static wxImage ImageFromBytes(const std::vector<std::tuple<int, int, int>> palette, const std::vector<unsigned char> &bytes, int start, bool isFirstPaletteColourTransparent = false);

		bool IsPaletteLoaded();
		bool IsTextureFileLoaded();

		void SetControlState(int state);

		bool firstColourTransparent = true;
};