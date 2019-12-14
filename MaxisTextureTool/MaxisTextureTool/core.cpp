#include "core.h"
#include "Helpers.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	if (!wxApp::OnInit()) {
		return false;
	}

	//wxImage::AddHandler(new wxPNGHandler);

	Frame *frame = new Frame("Test");
	//frame->SetIcon(wxICON(AAAAA));
	frame->Show(true);

	wxImage::AddHandler(new wxPNGHandler);

	return true;
}

int App::OnExit()
{
	return wxApp::OnExit();
}

Frame::Frame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & (~wxRESIZE_BORDER) & (~wxMAXIMIZE_BOX))//, fieldSize(wxSize(48, -1))), currentThread(nullptr)
{
	Bind(wxEVT_CLOSE_WINDOW, &Frame::OnClose, this);

	panel1 = new wxPanel(this, wxID_ANY);

	imagePanel = new wxImagePanel(this, wxID_ANY);

	loadPalette = new wxButton(panel1, wxID_ANY, "Load");
	loadPalette->Bind(wxEVT_BUTTON, &Frame::OnLoadPalette, this);
	loadPalette->SetToolTip("Load a palette from a Maxis mesh geometry file.");

	exportPalette = new wxButton(panel1, wxID_ANY, "Export");
	exportPalette->Bind(wxEVT_BUTTON, &Frame::OnExportPalette, this);
	exportPalette->SetToolTip("Export the current palette as a PNG or a GIMP palette.");

	loadTextures = new wxButton(panel1, wxID_ANY, "Load");
	loadTextures->Bind(wxEVT_BUTTON, &Frame::OnLoadTextures, this);
	loadTextures->SetToolTip("Load a Maxis composite bitmap file.");

	textCurNumber = new wxStaticText(panel1, wxID_ANY, "-", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL | wxST_NO_AUTORESIZE);
	textCurNumber->SetBackgroundColour(wxColour(255, 0, 0));

	// Unicode values from: https://en.wikipedia.org/wiki/Geometric_Shapes

	prev = new wxButton(panel1, wxID_ANY, L"\u25C0");
	prev->Bind(wxEVT_BUTTON, &Frame::OnPrev, this);

	next = new wxButton(panel1, wxID_ANY, L"\u25B6");
	next->Bind(wxEVT_BUTTON, &Frame::OnNext, this);

	goTo = new wxButton(panel1, wxID_ANY, "Go to");
	goTo->Bind(wxEVT_BUTTON, &Frame::OnGoTo, this);

	wxIntegerValidator<unsigned int> validatorCurNumber;
	goToNumber = new wxTextCtrl(panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, validatorCurNumber);

	exportCurrentTexture = new wxButton(panel1, wxID_ANY, "Export Current");
	exportCurrentTexture->Bind(wxEVT_BUTTON, &Frame::OnExportCurrentTexture, this);
	exportCurrentTexture->SetToolTip("Export the current texture as a PNG.");

	exportAllTextures = new wxButton(panel1, wxID_ANY, "Export All");
	exportAllTextures->Bind(wxEVT_BUTTON, &Frame::OnExportAllTextures, this);
	exportAllTextures->SetToolTip("Export all textures as PNGs.");

	replaceCurrent = new wxButton(panel1, wxID_ANY, "Replace Current");
	replaceCurrent->Bind(wxEVT_BUTTON, &Frame::OnReplaceCurrent, this);
	replaceCurrent->SetToolTip("Replace the current texture. Colours will be mapped to the current palette.");

	saveTextureFile = new wxButton(panel1, wxID_ANY, "Save");
	saveTextureFile->Bind(wxEVT_BUTTON, &Frame::OnSaveTextureFile, this);

	wxBoxSizer *baseSizer = new wxBoxSizer(wxVERTICAL); // Contains panel
	wxBoxSizer *panelSizer = new wxBoxSizer(wxHORIZONTAL); // Contains everything else

	wxBoxSizer *bSizer1 = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer *sBSizerPalette = new wxStaticBoxSizer(wxVERTICAL, panel1, "Palette");
	wxStaticBoxSizer *sBSizerTexture = new wxStaticBoxSizer(wxVERTICAL, panel1, "Textures");
	wxBoxSizer *bSizer2a = new wxBoxSizer(wxHORIZONTAL); // For prev/next buttons.
	wxBoxSizer *bSizer2b = new wxBoxSizer(wxHORIZONTAL); // For go to button and field.
	
	baseSizer->Add(panel1, 1, wxEXPAND | wxALL, 0);

	panelSizer->Add(imagePanel);
	panelSizer->Add(bSizer1);

	int margin = wxSizerFlags::GetDefaultBorder();

	sBSizerPalette->Add(loadPalette, 1, wxALIGN_CENTER | wxALL, margin);
	sBSizerPalette->Add(exportPalette, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	bSizer1->Add(sBSizerPalette, 0, wxEXPAND | wxALL, margin);

	sBSizerTexture->Add(loadTextures, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	sBSizerTexture->Add(textCurNumber, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	sBSizerTexture->Add(bSizer2a, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	sBSizerTexture->Add(bSizer2b, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	sBSizerTexture->Add(exportCurrentTexture, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	sBSizerTexture->Add(exportAllTextures, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	sBSizerTexture->Add(replaceCurrent, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	sBSizerTexture->Add(saveTextureFile, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	bSizer1->Add(sBSizerTexture, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, margin);
	
	bSizer2a->Add(prev, 0, wxALIGN_CENTER, margin);
	bSizer2a->Add(next, 0, wxALIGN_CENTER, margin);

	bSizer2b->Add(goTo, 0, wxALIGN_CENTER, margin);
	bSizer2b->Add(goToNumber, 0, wxALIGN_CENTER, margin);

	panel1->SetSizer(panelSizer);

	SetSizerAndFit(baseSizer);

	Centre();

	SetControlState(0);
}

void Frame::OnClose(wxCloseEvent& event)
{
	//wxMessageBox("Message", "Caption", wxOK | wxICON_INFORMATION);
	
	
	//Close(true); // From hello world guide: not what I've done in the past and gets into a loop calling OnClose.
	Destroy();
}

void Frame::OnLoadPalette(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog openFileDialog(this, "Open file", wxEmptyString, wxEmptyString, "Maxis mesh files (*.max)|*.max", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	int result = openFileDialog.ShowModal();

	if (result == wxID_CANCEL) {
		return;
	}

	// GetPath returns const, so wxFileInputStream will open the file in read-only mode.
	wxFileInputStream inStream(openFileDialog.GetPath());

	if (!inStream.IsOk()) {
		wxMessageBox("Couldn't open file.", "Error", wxOK | wxICON_ERROR);
		return;
	}

	wxFile *file = inStream.GetFile();

	unsigned char bytesCmapOffset[4];

	file->Seek(16);
	file->Read(&bytesCmapOffset, 4);

	int cmapStart = Helpers::BytesToInt32(bytesCmapOffset);

	int cmapDataStart = cmapStart + 33;

	unsigned char bytesColour[3];

	int offset = cmapDataStart;
	palette.clear();
	for (int i = 0; i < PaletteColourCount; ++i) {
		file->Seek(offset);
		file->Read(&bytesColour, 3);

		palette.push_back(std::make_tuple(bytesColour[0], bytesColour[1], bytesColour[2]));

		offset += 3;
	}

	if (!IsTextureFileLoaded()) { // No texture file loaded, show palette.
		imagePanel->SetImage(GeneratePaletteImage(1));
		SetControlState(1);
	}
	else {
		UpdateImage();
	}
}

void Frame::OnExportPalette(wxCommandEvent& WXUNUSED(event))
{
	if (!IsPaletteLoaded()) {
		wxMessageBox("Please load a palette.", "Information", wxOK | wxICON_INFORMATION);
		return;
	}

	wxFileDialog fileDialog(this, "Save file", wxEmptyString, wxEmptyString, "PNG files (*.png)|*.png|GIMP palette files (*.gpl)|*.gpl", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	int result = fileDialog.ShowModal();

	if (result == wxID_CANCEL) {
		return;
	}

	wxFileName fn = wxFileName(fileDialog.GetPath());
	wxString ext = fn.GetExt();

	if (ext == "png") {
		wxImage paletteImage = GeneratePaletteImage(1);
		paletteImage.SaveFile(fileDialog.GetPath());
	}
	else if (ext == "gpl") {
		wxFile paletteFile;
		paletteFile.Create(fileDialog.GetPath(), true);
		wxString data;
		data << "GIMP Palette\n"
			<< "Name: Maxis\n"
			<< "# Exported by the Maxis Texture Tool.\n";

		for (int i = 0; i < PaletteColourCount; ++i)
		{
			int r, g, b;
			std::tie(r, g, b) = palette[i];
			data << wxString::Format("%3d %3d %3d\n", r, g, b);
		}

		paletteFile.Write(data);
		paletteFile.Close();
	}
	else {
		wxString message;
		message << "Unanticipated extension (" << ext << ").";
		wxMessageBox(message, "Error", wxOK | wxICON_ERROR);
		return;
	}
}

void Frame::OnLoadTextures(wxCommandEvent& WXUNUSED(event))
{
	if (!IsPaletteLoaded()) {
		wxMessageBox("Please load a palette.", "Information", wxOK | wxICON_INFORMATION);
		return;
	}

	wxFileDialog openFileDialog(this, "Open file", wxEmptyString, wxEmptyString, "Maxis composite bitmap files (*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	int result = openFileDialog.ShowModal();

	if (result == wxID_CANCEL) {
		return;
	}

	// GetPath returns const, so wxFileInputStream will open the file in read-only mode.
	wxFileInputStream inStream(openFileDialog.GetPath());

	if (!inStream.IsOk()) {
		wxMessageBox("Couldn't open file.", "Error", wxOK | wxICON_ERROR);
		return;
	}

	wxFile *file = inStream.GetFile();

	int fileSize = ReadInt32(file, 0);

	unsigned char *raw = new unsigned char[fileSize];
	file->Seek(0);
	file->Read(raw, fileSize);
	fileBytes = std::vector<unsigned char>(raw, raw + fileSize);
	delete[] raw;

	imageCount = ReadInt32(file, 8);
	curImage = 0;

	int resolutionCount = ReadInt32(file, 12);

	int texStart = 16 + resolutionCount * 3 * 4;
	
	starts = std::vector<int>(imageCount);

	int current = texStart;
	for (int i = 0; i < imageCount; ++i) {
		starts[i] = current;

		int colCount = ReadInt32(file, current); // X
		int rowCount = ReadInt32(file, current + 4); // Y
		int dataStart = current + 3 * 4 + rowCount * 4;

		current = current + 3 * 4 + rowCount * 4 + colCount * rowCount;
	}

	UpdateImage();

	SetControlState(2);
}

void Frame::OnPrev(wxCommandEvent& WXUNUSED(event))
{
	if (!IsTextureFileLoaded()) {
		wxMessageBox("Please load a file.", "Information", wxOK | wxICON_INFORMATION);
		return;
	}

	curImage = (curImage - 1) % imageCount;
	if (curImage < 0) curImage += imageCount;

	UpdateImage();
}

void Frame::OnNext(wxCommandEvent& WXUNUSED(event))
{
	if (!IsTextureFileLoaded()) {
		wxMessageBox("Please load a file.", "Information", wxOK | wxICON_INFORMATION);
		return;
	}

	curImage = (curImage + 1) % imageCount;

	UpdateImage();
}

void Frame::OnGoTo(wxCommandEvent& WXUNUSED(event))
{
	if (!IsTextureFileLoaded()) {
		wxMessageBox("Please load a file.", "Information", wxOK | wxICON_INFORMATION);
		return;
	}

	// TODO text validator is unsigned, need a better way to convert this (could use ToULong but it's kind of clunky).
	int value = wxAtoi(goToNumber->GetLineText(0));

	if (value < 1) value = 1;
	if (value > imageCount) value = imageCount;

	wxString updatedLabel;
	updatedLabel << value;
	goToNumber->SetLabel(updatedLabel);

	curImage = value - 1;

	UpdateImage();
}

void Frame::OnExportCurrentTexture(wxCommandEvent& WXUNUSED(event))
{
	if (!IsTextureFileLoaded()) {
		wxMessageBox("Please load a file.", "Information", wxOK | wxICON_INFORMATION);
		return;
	}

	wxFileDialog fileDialog(this, "Save file", wxEmptyString, wxEmptyString, "PNG files (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	int result = fileDialog.ShowModal();

	if (result == wxID_CANCEL) {
		return;
	}

	wxImage image = ImageFromBytes(palette, fileBytes, starts[curImage]);
	image.SaveFile(fileDialog.GetPath());
}

void Frame::OnExportAllTextures(wxCommandEvent& WXUNUSED(event))
{
	if (!IsTextureFileLoaded()) {
		wxMessageBox("Please load a file.", "Information", wxOK | wxICON_INFORMATION);
		return;
	}

	wxDirDialog dirDialog(this, "Specify output folder", wxEmptyString);
	int result = dirDialog.ShowModal();

	if (result == wxID_CANCEL) {
		return;
	}

	for (int i = 0; i < imageCount; ++i) {
		wxImage image = ImageFromBytes(palette, fileBytes, starts[i]);
		wxFileName fileName = wxFileName::DirName(dirDialog.GetPath());
		wxString name;
		name << "tex" << i;
		fileName.SetName(name);
		fileName.SetExt("png");
		image.SaveFile(fileName.GetFullPath());
	}
}

void Frame::OnReplaceCurrent(wxCommandEvent& WXUNUSED(event))
{
	if (!IsTextureFileLoaded()) {
		wxMessageBox("Please load a file.", "Information", wxOK | wxICON_INFORMATION);
		return;
	}

	int start = starts[curImage];

	int colCount = Helpers::BytesToInt32(fileBytes.data() + start); // X
	int rowCount = Helpers::BytesToInt32(fileBytes.data() + start + 4); // Y

	int dataStart = start + 3 * 4 + rowCount * 4;

	wxString openMessage;
	openMessage << "Select an image (" << colCount << "x" << rowCount << ")";
	wxFileDialog openFileDialog(this, openMessage, wxEmptyString, wxEmptyString, "PNG files (*.png)|*.png", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	int result = openFileDialog.ShowModal();

	if (result == wxID_CANCEL) {
		return;
	}

	wxImage replacement;
	replacement.LoadFile(openFileDialog.GetPath());

	if (replacement.GetWidth() != colCount || replacement.GetHeight() != rowCount) {
		// There's no reason why a texture can't be replaced with one of a different size.
		// This is just laziness on my part to avoid having to update anything besides the pixel data of the current texture.
		wxString message;
		message << "Selected image is " << replacement.GetWidth() << "x" << replacement.GetHeight() << ".\n"
			<< "Existing texture is " << colCount << "x" << rowCount << ".\n"
			<< "Size must match.";
		wxMessageBox(message, "Error", wxOK | wxICON_ERROR);
		return;
	}

	wxProgressDialog progressDialog("Mapping colours to palette...", "...", colCount*rowCount, this, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	int pixelsReplacedCount = 0;
	unsigned char *repData = replacement.GetData();

	// For colours that aren't in the palette.
	std::unordered_map<int, int> nearestMatches;

	for (int curRow = 0; curRow < rowCount; ++curRow)
	{
		for (int curCol = 0; curCol < colCount; ++curCol)
		{
			int newR = repData[3 * curRow*colCount + 3 * curCol];
			int newG = repData[3 * curRow*colCount + 3 * curCol + 1];
			int newB = repData[3 * curRow*colCount + 3 * curCol + 2];

			int address = dataStart + curCol + curRow * colCount;

			// Find exact match if possible.
			std::vector<std::tuple<int, int, int>>::iterator it;
			it = std::find(palette.begin(), palette.end(), std::make_tuple(newR, newG, newB));
			if (it != palette.end())
			{
				fileBytes[address] = std::distance(palette.begin(), it);
				continue;
			}
			
			// No exact match; new colour will need to be replaced with nearest in palette.
			++pixelsReplacedCount;

			// Check if a nearest match was already found for this colour.
			int key = newR + 1000 * newG + 1000000 * newB;
			auto existingNearest = nearestMatches.find(key);
			if (existingNearest != nearestMatches.end())
			{
				fileBytes[address] = existingNearest->second;
				continue;
			}

			// Find palette colour closest to new one.
			int indexBest;
			int minError;
			for (int curPal = 0; curPal < palette.size(); ++curPal)
			{
				int r, g, b;
				std::tie(r, g, b) = palette[curPal];

				// Something perceptually-weighted would be better but this is cheap.
				int curError = std::abs(newR - r) + std::abs(newG - g) + std::abs(newB - b);
				
				if (curPal == 0 || curError < minError) {
					indexBest = curPal;
					minError = curError;
				}
			}

			/*
			// This isn't any faster than using a raw loop.
			it = std::min_element(palette.begin(), palette.end(), [=](const std::tuple<int,int,int> &a, const std::tuple<int, int, int> &b) {
				int rA, gA, bA;
				std::tie(rA, gA, bA) = a;
				int rB, gB, bB;
				std::tie(rB, gB, bB) = b;

				int errorA = std::abs(newR - rA) + std::abs(newG - gA) + std::abs(newB - bA);
				int errorB = std::abs(newR - rB) + std::abs(newG - gB) + std::abs(newB - bB);

				return errorA < errorB;
			});
			int indexBest = std::distance(palette.begin(), it);
			*/

			nearestMatches[key] = indexBest;
			fileBytes[address] = indexBest;
		}

		// Only update progress once per row (each update adds delay).
		wxString progressMessage;
		progressMessage << "Pixel " << curRow * colCount + colCount << " of " << rowCount * colCount;
		progressDialog.Update(curRow*colCount + colCount, progressMessage);
	}

	if (pixelsReplacedCount != 0) {
		wxString message;
		message << pixelsReplacedCount << " pixels were replaced with the nearest palette colour.";
		wxMessageBox(message, "Information", wxOK | wxICON_INFORMATION);
	}

	UpdateImage();
}

void Frame::OnSaveTextureFile(wxCommandEvent& WXUNUSED(event)) {
	if (!IsTextureFileLoaded()) {
		wxMessageBox("Please load a file.", "Information", wxOK | wxICON_INFORMATION);
		return;
	}

	wxFileDialog fileDialog(this, "Save file", wxEmptyString, wxEmptyString, "Maxis composite bitmap files (*.bmp)|*.bmp", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	int result = fileDialog.ShowModal();

	if (result == wxID_CANCEL) {
		return;
	}

	wxFile file;
	file.Create(fileDialog.GetPath(), true);
	file.Write(fileBytes.data(), fileBytes.size());
	file.Close();
}







void Frame::UpdateImage()
{
	int start = starts[curImage];

	int colCount = Helpers::BytesToInt32(fileBytes.data() + start); // X
	int rowCount = Helpers::BytesToInt32(fileBytes.data() + start + 4); // Y

	wxString strCurNumber;
	strCurNumber << curImage+1 << " of " << imageCount << " (" << colCount << "x" << rowCount << ")";
	textCurNumber->SetLabel(strCurNumber);

	wxImage image = ImageFromBytes(palette, fileBytes, start);
	imagePanel->SetImage(image);
}

int Frame::ReadInt32(wxFile* file, int address)
{
	unsigned char fourBytes[4];
	file->Seek(address);
	file->Read(&fourBytes, 4);
	return Helpers::BytesToInt32(fourBytes);
}

int Frame::ReadByte(wxFile* file, int address)
{
	unsigned char fourBytes[4] = { 0, 0, 0, 0 }; // Initialized to zero by default, but this makes intent explicit.
	file->Seek(address);
	file->Read(&fourBytes, 1);
	return Helpers::BytesToInt32(fourBytes);
}

wxImage Frame::GeneratePaletteImage(int scaleFactor)
{
	if (!IsPaletteLoaded()) return wxImage(16,16); // TODO should probably throw

	if (scaleFactor < 1) scaleFactor = 1;

	const int Dim = 16;
	wxImage paletteImage(Dim, Dim);

	for (int i = 0; i < PaletteColourCount; ++i)
	{
		int r, g, b;
		std::tie(r, g, b) = palette[i];
		paletteImage.SetRGB(i % Dim, i / Dim, r, g, b);
	}

	if (scaleFactor != 1)
	{
		paletteImage.Rescale(scaleFactor * Dim, scaleFactor * Dim, wxIMAGE_QUALITY_NEAREST);
	}

	return paletteImage;
}

void Frame::SaveImage(const wxImage &image)
{
	wxFileDialog fileDialog(this, "Save file", wxEmptyString, wxEmptyString, "PNG files (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	int result = fileDialog.ShowModal();

	if (result == wxID_CANCEL) {
		return;
	}

	image.SaveFile(fileDialog.GetPath());
}

wxImage Frame::ImageFromBytes(const std::vector<std::tuple<int, int, int>> palette, const std::vector<unsigned char> &bytes, int start)
{
	int colCount = Helpers::BytesToInt32(bytes.data() + start); // X
	int rowCount = Helpers::BytesToInt32(bytes.data() + start + 4); // Y

	int dataStart = start + 3 * 4 + rowCount * 4;

	wxImage out(colCount, rowCount, true);

	for (int curRow = 0; curRow < rowCount; ++curRow)
	{
		for (int curCol = 0; curCol < colCount; ++curCol)
		{
			int address = dataStart + curCol + curRow * colCount;
			int index = bytes[address];
			int r, g, b;
			std::tie(r, g, b) = palette.at(index);
			out.SetRGB(curCol, curRow, r, g, b);
		}
	}

	return out;
}

bool Frame::IsPaletteLoaded() {
	return palette.size() == PaletteColourCount;
}

bool Frame::IsTextureFileLoaded() {
	return imageCount > 0;
}

void Frame::SetControlState(int state)
{
	switch (state)
	{
		case 0: // No palette, no texture file.
			loadPalette->Enable();
			exportPalette->Disable();
			loadTextures->Disable();
			prev->Disable();
			next->Disable();
			goTo->Disable();
			goToNumber->Disable();
			exportCurrentTexture->Disable();
			exportAllTextures->Disable();
			replaceCurrent->Disable();
			saveTextureFile->Disable();
			break;
		case 1: // Palette, no texture file.
			loadPalette->Enable();
			exportPalette->Enable();
			loadTextures->Enable();
			prev->Disable();
			next->Disable();
			goTo->Disable();
			goToNumber->Disable();
			exportCurrentTexture->Disable();
			exportAllTextures->Disable();
			replaceCurrent->Disable();
			saveTextureFile->Disable();
			break;
		case 2: // Palette and texture file.
			loadPalette->Enable();
			exportPalette->Enable();
			loadTextures->Enable();
			prev->Enable();
			next->Enable();
			goTo->Enable();
			goToNumber->Enable();
			exportCurrentTexture->Enable();
			exportAllTextures->Enable();
			replaceCurrent->Enable();
			saveTextureFile->Enable();
			break;
	}
}