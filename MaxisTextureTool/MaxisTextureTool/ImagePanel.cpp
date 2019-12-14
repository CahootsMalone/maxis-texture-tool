#include "ImagePanel.h"

wxImagePanel::wxImagePanel(wxFrame* parent, wxWindowID id) : wxPanel(parent, id, wxDefaultPosition, wxSize(Size, Size))
{
	Bind(wxEVT_PAINT, &wxImagePanel::OnPaint, this);
	image = wxImage(Size, Size, true); // Cleared to black.
}

void wxImagePanel::SetImage(const wxImage& newImage)
{
	if (newImage.GetWidth() < Size && newImage.GetHeight() < Size) {
		int base = std::max(newImage.GetWidth(), newImage.GetHeight());
		int scaleFactor = Size / base;
		if (scaleFactor == 1) {
			image = newImage;
		}
		else {
			wxImage scaledImage(newImage);
			scaledImage.Rescale(scaleFactor * newImage.GetWidth(), scaleFactor * newImage.GetHeight(), wxIMAGE_QUALITY_NEAREST);
			image = scaledImage;
		}
	}
	else {
		image = newImage;
	}
	
	Paint();
}

void wxImagePanel::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this); // TODO use wxBufferedPaintDC instead? https://docs.wxwidgets.org/3.0/classwx_buffered_paint_d_c.html
	Render(dc);
}

void wxImagePanel::Paint()
{
	wxClientDC dc(this);
	Render(dc);
}

void wxImagePanel::Render(wxDC& dc)
{
	dc.SetBackground(wxBrush(wxColour(0, 0, 0)));
	dc.Clear();
	dc.DrawBitmap(wxBitmap(image), 0, 0);
	dc.SetBackground(wxNullBrush);
}