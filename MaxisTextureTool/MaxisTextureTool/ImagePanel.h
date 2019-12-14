#pragma once

#include <wx/wx.h>

class wxImagePanel : public wxPanel
{
public:
	wxImagePanel(wxFrame* parent, wxWindowID id);

	void SetImage(const wxImage& newImage);

	void OnPaint(wxPaintEvent& evt);
	
	void Paint();

	void Render(wxDC& dc);
private:
	wxImage image;

	static const int Size = 512;
};