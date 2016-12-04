#include "RowHeading.h"
#include "wx/wx.h"
#include "wx/brush.h"
#include "../xLightsMain.h"
#include "EffectDropTarget.h"
#include "../BitmapCache.h"
#include <wx/numdlg.h> 

BEGIN_EVENT_TABLE(RowHeading, wxWindow)
EVT_LEFT_DOWN(RowHeading::mouseLeftDown)
EVT_RIGHT_DOWN(RowHeading::rightClick)
EVT_LEFT_DCLICK(RowHeading::leftDoubleClick)
EVT_PAINT(RowHeading::render)
END_EVENT_TABLE()

// Menu constants
const long RowHeading::ID_ROW_MNU_INSERT_LAYER_ABOVE = wxNewId();
const long RowHeading::ID_ROW_MNU_INSERT_LAYER_BELOW = wxNewId();
const long RowHeading::ID_ROW_MNU_INSERT_LAYERS_BELOW = wxNewId();
const long RowHeading::ID_ROW_MNU_DELETE_LAYER = wxNewId();
const long RowHeading::ID_ROW_MNU_DELETE_ALL_LAYERS = wxNewId();
const long RowHeading::ID_ROW_MNU_LAYER = wxNewId();
const long RowHeading::ID_ROW_MNU_PLAY_MODEL = wxNewId();
const long RowHeading::ID_ROW_MNU_EXPORT_MODEL = wxNewId();
const long RowHeading::ID_ROW_MNU_EXPORT_RENDERED_MODEL = wxNewId();
const long RowHeading::ID_ROW_MNU_EDIT_DISPLAY_ELEMENTS = wxNewId();
const long RowHeading::ID_ROW_MNU_TOGGLE_STRANDS = wxNewId();
const long RowHeading::ID_ROW_MNU_TOGGLE_NODES = wxNewId();
const long RowHeading::ID_ROW_MNU_CONVERT_TO_EFFECTS = wxNewId();
const long RowHeading::ID_ROW_MNU_PROMOTE_EFFECTS = wxNewId();
const long RowHeading::ID_ROW_MNU_COPY_ROW = wxNewId();
const long RowHeading::ID_ROW_MNU_PASTE_ROW = wxNewId();

// Timing Track popup menu
const long RowHeading::ID_ROW_MNU_ADD_TIMING_TRACK = wxNewId();
const long RowHeading::ID_ROW_MNU_RENAME_TIMING_TRACK = wxNewId();
const long RowHeading::ID_ROW_MNU_DELETE_TIMING_TRACK = wxNewId();
const long RowHeading::ID_ROW_MNU_IMPORT_TIMING_TRACK = wxNewId();
const long RowHeading::ID_ROW_MNU_EXPORT_TIMING_TRACK = wxNewId();
const long RowHeading::ID_ROW_MNU_IMPORT_NOTES = wxNewId();
const long RowHeading::ID_ROW_MNU_IMPORT_LYRICS = wxNewId();
const long RowHeading::ID_ROW_MNU_BREAKDOWN_TIMING_PHRASES = wxNewId();
const long RowHeading::ID_ROW_MNU_BREAKDOWN_TIMING_WORDS = wxNewId();


int DEFAULT_ROW_HEADING_HEIGHT = 22;

RowHeading::RowHeading(MainSequencer* parent, wxWindowID id, const wxPoint &pos, const wxSize &size,
                       long style, const wxString &name):
                       wxWindow((wxWindow*)parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    DOUBLE_BUFFER(this);
    mHeaderColorModel = new xlColor(212,208,200);
    mHeaderColorTiming = new xlColor(130,178,207);
    mHeaderSelectedColor = new xlColor(130,178,207);
    wxString tooltip;
    papagayo_icon = BitmapCache::GetPapgayoIcon(tooltip, 16, false);
    model_group_icon = BitmapCache::GetModelGroupIcon(tooltip, 16, false);
    mCanPaste = false;
}

RowHeading::~RowHeading()
{
    if (mHeaderColorModel != NULL) delete mHeaderColorModel;
    if (mHeaderColorTiming != NULL) delete mHeaderColorTiming;
    if (mHeaderSelectedColor != NULL) delete mHeaderSelectedColor;
}

void RowHeading::mouseLeftDown( wxMouseEvent& event)
{
    mSelectedRow = event.GetY()/DEFAULT_ROW_HEADING_HEIGHT;
    if(mSelectedRow < mSequenceElements->GetVisibleRowInformationSize())
    {
        bool result;
        Element* e = mSequenceElements->GetVisibleRowInformation(mSelectedRow)->element;
        if(e->GetType() == ELEMENT_TYPE_MODEL)
        {
            mSequenceElements->UnSelectAllElements();
            ModelElement *me = dynamic_cast<ModelElement *>(e);
            me->SetSelected(true);
            wxCommandEvent playEvent(EVT_MODEL_SELECTED);  // send model selection in case we need to switch playback to this model
            playEvent.SetString(e->GetModelName());
            wxPostEvent(GetParent(), playEvent);
            Refresh(false);
        }
        if(HitTestCollapseExpand(mSelectedRow,event.GetX(),&result))
        {
            e->SetCollapsed(!result);
            wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
            wxPostEvent(GetParent(), eventRowHeaderChanged);
        }
        else if(HitTestTimingActive(mSelectedRow,event.GetX(),&result))
        {
            mSequenceElements->DeactivateAllTimingElements();
            TimingElement *te = dynamic_cast<TimingElement *>(e);
            te->SetActive(!result);
            // Set the selected timing row.
            int selectedTimingRow = result?mSelectedRow:-1;
            mSequenceElements->SetSelectedTimingRow(selectedTimingRow);
            wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
            wxPostEvent(GetParent(), eventRowHeaderChanged);
        }
    }
}
void RowHeading::leftDoubleClick(wxMouseEvent& event)
{
    mSelectedRow = event.GetY()/DEFAULT_ROW_HEADING_HEIGHT;
    if (mSelectedRow >= mSequenceElements->GetVisibleRowInformationSize()) {
        return;
    }
    Row_Information_Struct *ri =  mSequenceElements->GetVisibleRowInformation(mSelectedRow);
    Element* element = ri->element;

    if (element->GetType() == ELEMENT_TYPE_MODEL) {
        ModelElement *me = dynamic_cast<ModelElement *>(element);
        me->ShowStrands(!me->ShowStrands());
        wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
        eventRowHeaderChanged.SetString(element->GetModelName());
        wxPostEvent(GetParent(), eventRowHeaderChanged);
    } else if (element->GetType() == ELEMENT_TYPE_SUBMODEL) {

    } else if (element->GetType() == ELEMENT_TYPE_STRAND) {
        StrandElement *se = dynamic_cast<StrandElement *>(element);
        se->ShowNodes(!se->ShowNodes());

        wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
        eventRowHeaderChanged.SetString(element->GetName());
        wxPostEvent(GetParent(), eventRowHeaderChanged);
    } else if (element->GetType() == ELEMENT_TYPE_TIMING) {
        if (element->GetEffectLayerCount() > 1) {
            if(element->GetCollapsed()) {
                element->SetCollapsed(false);
            } else {
                element->SetCollapsed(true);
            }
            wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
            eventRowHeaderChanged.SetString(element->GetName());
            wxPostEvent(GetParent(), eventRowHeaderChanged);
        }
    }
}
void RowHeading::rightClick( wxMouseEvent& event)
{
    wxMenu mnuLayer;
    mSelectedRow = event.GetY()/DEFAULT_ROW_HEADING_HEIGHT;
    if (mSelectedRow >= mSequenceElements->GetVisibleRowInformationSize()) {
        return;
    }

    Row_Information_Struct *ri =  mSequenceElements->GetVisibleRowInformation(mSelectedRow);
    Element* element = ri->element;
    if (element->GetType()==ELEMENT_TYPE_MODEL
        || element->GetType()==ELEMENT_TYPE_SUBMODEL
        || element->GetType()==ELEMENT_TYPE_STRAND)
    {
        if (ri->nodeIndex < 0) {
            mnuLayer.Append(ID_ROW_MNU_INSERT_LAYER_ABOVE,"Insert Layer Above");
            mnuLayer.Append(ID_ROW_MNU_INSERT_LAYER_BELOW,"Insert Layer Below");
            mnuLayer.Append(ID_ROW_MNU_INSERT_LAYERS_BELOW, "Insert Multiple Layers Below");

            if(element->GetEffectLayerCount() > 1)
            {
                mnuLayer.Append(ID_ROW_MNU_DELETE_LAYER,"Delete Layer");

				mnuLayer.Append(ID_ROW_MNU_DELETE_ALL_LAYERS, "Delete All Layers");
            }
            mnuLayer.AppendSeparator();
        }
        mnuLayer.Append(ID_ROW_MNU_PLAY_MODEL,"Play Model");
        mnuLayer.Append(ID_ROW_MNU_EXPORT_MODEL,"Export Model");
        mnuLayer.Append(ID_ROW_MNU_EXPORT_RENDERED_MODEL, "Render and Export Model");
        mnuLayer.AppendSeparator();
        bool canPromote = false;
        ModelElement *me = dynamic_cast<ModelElement *>(element);
        if (element->GetType()==ELEMENT_TYPE_STRAND || element->GetType()==ELEMENT_TYPE_SUBMODEL) {
            me = dynamic_cast<SubModelElement *>(element)->GetModelElement();
        }
        if (me->GetSubModelCount() > 0) {
            if (element->GetType() != ELEMENT_TYPE_SUBMODEL) {
                canPromote = true;
            }
            mnuLayer.Append(ID_ROW_MNU_TOGGLE_STRANDS,"Toggle Strands");
            if (ri->strandIndex >= 0) {
                mnuLayer.Append(ID_ROW_MNU_TOGGLE_NODES,"Toggle Nodes");
            }
        } else {
            mnuLayer.Append(ID_ROW_MNU_TOGGLE_STRANDS,"Toggle Models");
        }
        if (ri->nodeIndex > -1) {
            StrandElement *se = dynamic_cast<StrandElement*>(element);
            if (se && se->GetNodeLayer(ri->nodeIndex)->GetEffectCount() == 0) {
                mnuLayer.Append(ID_ROW_MNU_CONVERT_TO_EFFECTS, "Convert To Effect");
            }
        }
        if (canPromote) {
            mnuLayer.Append(ID_ROW_MNU_PROMOTE_EFFECTS, "Promote Node Effects");
        }
        mnuLayer.AppendSeparator();
        mnuLayer.Append(ID_ROW_MNU_COPY_ROW,"Copy Row");
        wxMenuItem* menu_paste = mnuLayer.Append(ID_ROW_MNU_PASTE_ROW,"Paste Row");
        if (!mCanPaste) {
            menu_paste->Enable(false);
        }
    }
    else
    {
        mnuLayer.Append(ID_ROW_MNU_ADD_TIMING_TRACK,"Add Timing Track");
        mnuLayer.Append(ID_ROW_MNU_RENAME_TIMING_TRACK, "Rename Timing Track");
        mnuLayer.Append(ID_ROW_MNU_DELETE_TIMING_TRACK,"Delete Timing Track");
        mnuLayer.Append(ID_ROW_MNU_IMPORT_TIMING_TRACK, "Import Timing Track");
        mnuLayer.Append(ID_ROW_MNU_EXPORT_TIMING_TRACK, "Export Timing Track");
        mnuLayer.Append(ID_ROW_MNU_IMPORT_NOTES, "Import Notes");
        mnuLayer.AppendSeparator();
        mnuLayer.Append(ID_ROW_MNU_IMPORT_LYRICS,"Import Lyrics");
        mnuLayer.Append(ID_ROW_MNU_BREAKDOWN_TIMING_PHRASES,"Breakdown Phrases");
        if( element->GetEffectLayerCount() > 1 )
        {
            mnuLayer.Append(ID_ROW_MNU_BREAKDOWN_TIMING_WORDS,"Breakdown Words");
        }
        mnuLayer.AppendSeparator();
        mnuLayer.Append(ID_ROW_MNU_COPY_ROW,"Copy Row");
        wxMenuItem* menu_paste = mnuLayer.Append(ID_ROW_MNU_PASTE_ROW,"Paste Row");
        if( !mCanPaste ) {
            menu_paste->Enable(false);
        }
    }

    mnuLayer.AppendSeparator();
    mnuLayer.Append(ID_ROW_MNU_EDIT_DISPLAY_ELEMENTS,"Edit Display Elements");
    mnuLayer.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RowHeading::OnLayerPopup, NULL, this);
    Draw();
    PopupMenu(&mnuLayer);
}

void RowHeading::OnLayerPopup(wxCommandEvent& event)
{
    Row_Information_Struct *ri = mSequenceElements->GetVisibleRowInformation(mSelectedRow);
    Element* element = ri->element;
    int layer_index = ri->layerIndex;
    int id = event.GetId();
    if(id == ID_ROW_MNU_INSERT_LAYER_ABOVE)
    {
        element->InsertEffectLayer(layer_index);
        wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
        wxPostEvent(GetParent(), eventRowHeaderChanged);
    }
    else if(id == ID_ROW_MNU_INSERT_LAYER_BELOW)
    {
        if( layer_index < element->GetEffectLayerCount()-1)
        {
            element->InsertEffectLayer(layer_index+1);
        }
        else
        {
            element->AddEffectLayer();
        }
        wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
        wxPostEvent(GetParent(), eventRowHeaderChanged);
    }
    else if (id == ID_ROW_MNU_INSERT_LAYERS_BELOW)
    {
        int numtoinsert = wxGetNumberFromUser("Enter number of layers to insert", "Layers", "Insert multiple layers", 2, 1, 20, this);

        if (numtoinsert > 0)
        {
            if (layer_index < element->GetEffectLayerCount() - 1)
            {
                for (int i = 0; i < numtoinsert; i++)
                {
                    element->InsertEffectLayer(layer_index + 1);
                }
            }
            else
            {
                for (int i = 0; i < numtoinsert; i++)
                {
                    element->AddEffectLayer();
                }
            }
            wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
            wxPostEvent(GetParent(), eventRowHeaderChanged);
        }
    }
    else if(id == ID_ROW_MNU_DELETE_LAYER)
    {
        int layerIndex = mSequenceElements->GetVisibleRowInformation(mSelectedRow)->layerIndex;
        wxString prompt = wxString::Format("Delete 'Layer %d' of '%s'?",
                                      layerIndex+1,element->GetModelName());
        wxString caption = "Confirm Layer Deletion";

        int answer = wxMessageBox(prompt,caption,wxYES_NO);
        if(answer == wxYES)
        {
            element->RemoveEffectLayer(layerIndex);
            wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
            wxPostEvent(GetParent(), eventRowHeaderChanged);
        }
    }
	else if (id == ID_ROW_MNU_DELETE_ALL_LAYERS)
	{
		int delLayer = element->GetEffectLayerCount();
		int layerIndex = mSequenceElements->GetVisibleRowInformation(mSelectedRow)->layerIndex;
		wxString prompt = wxString::Format("Delete %d Layers?",
			delLayer);
		wxString caption = "Confirm All Layer Deletion";

		int answer = wxMessageBox(prompt, caption, wxYES_NO);
		if (answer == wxYES)
		{
			for (delLayer--; delLayer >= 0; delLayer--)
			{
				element->RemoveEffectLayer(delLayer);
			}

			element->AddEffectLayer();
			
			wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
			wxPostEvent(GetParent(), eventRowHeaderChanged);
		}
	}
    else if(id == ID_ROW_MNU_ADD_TIMING_TRACK)
    {
        std::string name = wxGetTextFromUser("What is name of new timing track?", "Timing Track Name").ToStdString();
        if( mSequenceElements->ElementExists(name) )
        {
            wxMessageBox("Timing name already exists in sequence as a model or another timing.", "ERROR");
        }
        else if(name.size()>0)
        {
            // Deactivate active timing mark so new one is selected;
            mSequenceElements->DeactivateAllTimingElements();
            int timingCount = mSequenceElements->GetNumberOfTimingElements();
            Element* e = mSequenceElements->AddElement(timingCount,name,
                                                       "timing",true,false,true,false);
            e->AddEffectLayer();
            mSequenceElements->AddTimingToAllViews(name);
            wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
            wxPostEvent(GetParent(), eventRowHeaderChanged);
        }
    }
    else if (id == ID_ROW_MNU_RENAME_TIMING_TRACK)
    {
        std::string name = wxGetTextFromUser("What is the new name of the timing track?", "Timing Track Name").ToStdString();
        if (mSequenceElements->ElementExists(name))
        {
            wxMessageBox("Timing name already exists in sequence as a model or another timing.", "ERROR");
        }
        else if (name.size()>0)
        {
            std::string oldname = element->GetName();
            mSequenceElements->GetXLightsFrame()->RenameTimingElement(oldname, name);
        }
    }
    else if(id == ID_ROW_MNU_DELETE_TIMING_TRACK)
    {
        wxString prompt = wxString::Format("Delete 'Timing Track '%s'?",element->GetName());
        wxString caption = "Confirm Timing Track Deletion";

        int answer = wxMessageBox(prompt,caption,wxYES_NO);
        if(answer == wxYES)
        {
            mSequenceElements->DeleteElement(element->GetModelName());
            wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
            wxPostEvent(GetParent(), eventRowHeaderChanged);
        }
    }
    else if (id == ID_ROW_MNU_EXPORT_TIMING_TRACK) {
        wxLogNull logNo; //kludge: avoid "error 0" message from wxWidgets after new file is written
        wxString filetypes;
        if (mSequenceElements->GetElement(element->GetName())->GetEffectLayerCount() == 3)
        {
            filetypes = "Timing files (*.xtiming)|*.xtiming|Papagayo files (*.pgo)|*.pgo";
        }
        else
        {
            filetypes = "Timing files (*.xtiming)|*.xtiming";
        }
        wxString filename = wxFileSelector(_("Choose output file"), wxEmptyString, element->GetModelName(), wxEmptyString, filetypes, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (filename.IsEmpty()) return;
        wxFileName fn(filename);
        static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
        TimingElement *te = dynamic_cast<TimingElement *>(element);
        if (fn.GetExt().Lower() == "xtiming")
        {
            wxFile f(filename);
            logger_base.info("Saving to xtiming file %s.", (const char *)filename.c_str());
            if (!f.Create(filename, true) || !f.IsOpened())
            {
                logger_base.info("Unable to create file %s. Error %d\n", (const char *)filename.c_str(), f.GetLastError());
                wxMessageBox(wxString::Format("Unable to create file %s. Error %d\n", filename, f.GetLastError()));
                return;
            }
            wxString name = wxFileName(filename).GetName();
            wxString td = wxString(te->GetExport().c_str());
            wxString v = xlights_version_string;
            f.Write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<timing ");
            f.Write(wxString::Format("name=\"%s\" ", name));
            f.Write(wxString::Format("SourceVersion=\"%s\">\n", v));
            f.Write(td);
            f.Write("</timing>\n");
            f.Close();
        }
        else if (fn.GetExt().Lower() == "pgo")
        {
            wxFile f(filename);
            logger_base.info("Saving to papagayo file %s.", (const char *)filename.c_str());
            if (!f.Create(filename, true) || !f.IsOpened())
            {
                logger_base.info("Unable to create file %s. Error %d\n", (const char *)filename.c_str(), f.GetLastError());
                wxMessageBox(wxString::Format("Unable to create file %s. Error %d\n", filename, f.GetLastError()));
                return;
            }
            wxString td = wxString(te->GetPapagayoExport(mSequenceElements->GetFrequency()).c_str());
            f.Write(td);
            f.Close();
        }
    } else if(id == ID_ROW_MNU_IMPORT_TIMING_TRACK) {
        wxCommandEvent playEvent(EVT_IMPORT_TIMING);
        wxPostEvent(GetParent(), playEvent);
    }
    else if (id == ID_ROW_MNU_IMPORT_NOTES) {
        wxCommandEvent importNotesEvent(EVT_IMPORT_NOTES);
        wxPostEvent(GetParent(), importNotesEvent);
    } else if(id == ID_ROW_MNU_IMPORT_LYRICS) {
        mSequenceElements->ImportLyrics(dynamic_cast<TimingElement *>(element), GetParent());
    } else if(id == ID_ROW_MNU_BREAKDOWN_TIMING_PHRASES) {
        int result = wxMessageBox("Breakdown phrases? Any existing words and phonemes will be deleted.", "Confirm Action", wxOK | wxCANCEL | wxCENTER);
        if (result == wxOK) {
            BreakdownTimingPhrases(dynamic_cast<TimingElement *>(element));
        }
    } else if(id == ID_ROW_MNU_BREAKDOWN_TIMING_WORDS) {
        int result = wxMessageBox("Breakdown words? Any existing phonemes will be deleted.", "Confirm Action", wxOK | wxCANCEL | wxCENTER);
        if (result == wxOK) {
            BreakdownTimingWords(dynamic_cast<TimingElement *>(element));
        }
    } else if (id == ID_ROW_MNU_EXPORT_MODEL) {
        wxCommandEvent playEvent(EVT_EXPORT_MODEL);
        playEvent.SetInt(0);
        playEvent.SetString(element->GetModelName());
        wxPostEvent(GetParent(), playEvent);
    } else if (id == ID_ROW_MNU_EXPORT_RENDERED_MODEL) {
        wxCommandEvent playEvent(EVT_EXPORT_MODEL);
        playEvent.SetInt(1);
        playEvent.SetString(element->GetModelName());
        wxPostEvent(GetParent(), playEvent);
    } else if (id == ID_ROW_MNU_PLAY_MODEL) {
        wxCommandEvent playEvent(EVT_PLAY_MODEL);
        playEvent.SetString(element->GetModelName());
        wxPostEvent(GetParent(), playEvent);
    } else if (id == ID_ROW_MNU_COPY_ROW) {
        wxCommandEvent copyRowEvent(EVT_COPY_MODEL_EFFECTS);
        copyRowEvent.SetInt(mSelectedRow);
        wxPostEvent(GetParent(), copyRowEvent);
        mCanPaste = true;
    } else if (id == ID_ROW_MNU_PASTE_ROW) {
        wxCommandEvent pasteRowEvent(EVT_PASTE_MODEL_EFFECTS);
        pasteRowEvent.SetInt(mSelectedRow);
        wxPostEvent(GetParent(), pasteRowEvent);
    } else if(id==ID_ROW_MNU_EDIT_DISPLAY_ELEMENTS) {
        wxCommandEvent displayElementEvent(EVT_SHOW_DISPLAY_ELEMENTS);
        wxPostEvent(GetParent(), displayElementEvent);
    } else if (id == ID_ROW_MNU_TOGGLE_STRANDS) {
        ModelElement *me = dynamic_cast<ModelElement *>(element);
        if (me == nullptr) {
            StrandElement *se = dynamic_cast<StrandElement *>(element);
            me = se->GetModelElement();
        }
        me->ShowStrands(!me->ShowStrands());
        wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
        eventRowHeaderChanged.SetString(element->GetModelName());
        wxPostEvent(GetParent(), eventRowHeaderChanged);
    } else if (id == ID_ROW_MNU_TOGGLE_NODES) {
        StrandElement *se = dynamic_cast<StrandElement *>(element);
        se->ShowNodes(!se->ShowNodes());
        wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
        wxPostEvent(GetParent(), eventRowHeaderChanged);
    } else if (id == ID_ROW_MNU_CONVERT_TO_EFFECTS) {
        wxCommandEvent evt(EVT_CONVERT_DATA_TO_EFFECTS);
        evt.SetClientData(element);
        int i = ((ri->strandIndex << 16) & 0xFFFF0000) + ri->nodeIndex;
        evt.SetInt(i);
        wxPostEvent(GetParent(), evt);
    } else if (id == ID_ROW_MNU_PROMOTE_EFFECTS) {
        wxCommandEvent evt(EVT_PROMOTE_EFFECTS);
        SubModelElement *se = dynamic_cast<SubModelElement *>(element);
        if (se != nullptr) {
            evt.SetClientData(se->GetModelElement());
        } else {
            evt.SetClientData(element);
        }
        wxPostEvent(GetParent(), evt);
    }

    // Make sure message box is painted over by grid.
    wxCommandEvent eventForceRefresh(EVT_FORCE_SEQUENCER_REFRESH);
    wxPostEvent(GetParent(), eventForceRefresh);
}

void RowHeading::BreakdownTimingPhrases(TimingElement* element)
{
    element->SetFixedTiming(0);
    EffectLayer* layer = element->GetEffectLayer(0);
    if( element->GetEffectLayerCount() > 1 )
    {
        for( int k = element->GetEffectLayerCount()-1; k > 0; k--)
        {
            element->RemoveEffectLayer(k);
        }
    }
    EffectLayer* word_layer = element->AddEffectLayer();
    for( int i = 0; i < layer->GetEffectCount(); i++ )
    {
        Effect* effect = layer->GetEffect(i);
        std::string phrase = effect->GetEffectName();
        mSequenceElements->BreakdownPhrase(word_layer, effect->GetStartTimeMS(), effect->GetEndTimeMS(), phrase);
    }
    wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
    wxPostEvent(GetParent(), eventRowHeaderChanged);
}

void RowHeading::BreakdownTimingWords(TimingElement* element)
{
    if( element->GetEffectLayerCount() > 2 )
    {
        element->RemoveEffectLayer(2);
    }
    EffectLayer* word_layer = element->GetEffectLayer(1);
    EffectLayer* phoneme_layer = element->AddEffectLayer();
    for( int i = 0; i < word_layer->GetEffectCount(); i++ )
    {
        Effect* effect = word_layer->GetEffect(i);
        std::string word = effect->GetEffectName();
        mSequenceElements->BreakdownWord(phoneme_layer, effect->GetStartTimeMS(), effect->GetEndTimeMS(), word);
    }
    wxCommandEvent eventRowHeaderChanged(EVT_ROW_HEADINGS_CHANGED);
    wxPostEvent(GetParent(), eventRowHeaderChanged);
}

bool RowHeading::HitTestCollapseExpand(int row,int x, bool* IsCollapsed)
{
    if(mSequenceElements->GetVisibleRowInformation(row)->element->GetType() != ELEMENT_TYPE_TIMING &&
       x<DEFAULT_ROW_HEADING_MARGIN)
    {
        *IsCollapsed = mSequenceElements->GetVisibleRowInformation(row)->Collapsed;
        return true;
    }
    else
    {
        return false;
    }
}

bool RowHeading::HitTestTimingActive(int row,int x, bool* IsActive)
{
    if (mSequenceElements->GetVisibleRowInformation(row)->element->GetType() == ELEMENT_TYPE_TIMING
        && x < DEFAULT_ROW_HEADING_MARGIN) {

        *IsActive = dynamic_cast<TimingElement*>(mSequenceElements->GetVisibleRowInformation(row)->element)->GetActive();
        return true;
    }
    return false;
}

void RowHeading::SetSelectedRow(int row_number)
{
    mSequenceElements->UnSelectAllElements();
    mSelectedRow = row_number;
    Refresh(false);
}

int RowHeading::GetMaxRows()
{
    int max = (int)(getHeight()/DEFAULT_ROW_HEADING_HEIGHT);
    return max;
}

void RowHeading::render( wxPaintEvent& event )
{
    if(!IsShownOnScreen()) return;
    wxPaintDC dc(this);
    Draw();
}

void RowHeading::Draw()
{
    wxClientDC dc(this);
    wxCoord w,h;
    wxPen penOutline(wxColor(32,32,32), .1);
    dc.GetSize(&w,&h);
    wxBrush brush(mHeaderColorModel->asWxColor(),wxBRUSHSTYLE_SOLID);
    dc.SetBrush(brush);
    dc.SetPen(penOutline);
    int row=0;
    int startY = 0,endY = 0;
    for(int i =0;i< mSequenceElements->GetVisibleRowInformationSize();i++)
    {
        wxString prefix;
        if (mSequenceElements->GetVisibleRowInformation(i)->submodel) {
            prefix = "  ";
        }
        wxBrush brush2(GetHeaderColor(mSequenceElements->GetVisibleRowInformation(i))->asWxColor(),wxBRUSHSTYLE_SOLID);
        dc.SetBrush(brush2);
        startY = DEFAULT_ROW_HEADING_HEIGHT*row;
        endY = DEFAULT_ROW_HEADING_HEIGHT*(row+1);
        dc.SetBackgroundMode(wxTRANSPARENT);
        dc.DrawRectangle(0,startY,w,DEFAULT_ROW_HEADING_HEIGHT);
        if(mSequenceElements->GetVisibleRowInformation(i)->layerIndex>0
           || mSequenceElements->GetVisibleRowInformation(i)->strandIndex >= 0)   // If effect layer = 0
        {
            dc.SetPen(*wxLIGHT_GREY_PEN);
            dc.DrawLine(1,startY,w-1,startY);
            dc.DrawLine(1,startY-1,w-1,startY-1);
            dc.SetPen(*wxBLACK_PEN);
            if (mSequenceElements->GetVisibleRowInformation(i)->strandIndex >= 0) {
                wxRect r(DEFAULT_ROW_HEADING_MARGIN,startY,w-DEFAULT_ROW_HEADING_MARGIN,DEFAULT_ROW_HEADING_HEIGHT);
                wxString name = mSequenceElements->GetVisibleRowInformation(i)->displayName;
                if (name == "") {
                    if (mSequenceElements->GetVisibleRowInformation(i)->nodeIndex >= 0) {
                        name = wxString::Format("Node %d", mSequenceElements->GetVisibleRowInformation(i)->nodeIndex + 1);
                    } else {
                        name = wxString::Format("Strand %d", mSequenceElements->GetVisibleRowInformation(i)->strandIndex + 1);
                    }

                }
                if (mSequenceElements->GetVisibleRowInformation(i)->nodeIndex >= 0) {
                    dc.DrawLabel(prefix + "     " + name,r,wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT);
                } else if (mSequenceElements->GetVisibleRowInformation(i)->layerIndex == 0) {
                    dc.DrawLabel(prefix + "  " + name,r,wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT);
                }
            }
            dc.SetPen(penOutline);
            dc.SetBrush(brush2);
        }
        else        // Draw label
        {
            if (mSequenceElements->GetVisibleRowInformation(i)->element->GetType() == ELEMENT_TYPE_SUBMODEL) {
                prefix += "  ";
            }
            wxRect r(DEFAULT_ROW_HEADING_MARGIN,startY,w-DEFAULT_ROW_HEADING_MARGIN,DEFAULT_ROW_HEADING_HEIGHT);
            dc.DrawLabel(prefix + mSequenceElements->GetVisibleRowInformation(i)->element->GetName(),r,wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT);
        }

        if (mSequenceElements->GetVisibleRowInformation(i)->element->GetType() != ELEMENT_TYPE_TIMING)
        {
            if(mSequenceElements->GetVisibleRowInformation(i)->element->GetEffectLayerCount() > 1
               && mSequenceElements->GetVisibleRowInformation(i)->layerIndex == 0
               && mSequenceElements->GetVisibleRowInformation(i)->nodeIndex == -1)
            {
                dc.SetBrush(*wxWHITE_BRUSH);
                dc.SetPen(*wxBLACK_PEN);
                dc.DrawRectangle(2,startY + DEFAULT_ROW_HEADING_HEIGHT/2 - 4,9,9);
                dc.DrawLine(2,startY + DEFAULT_ROW_HEADING_HEIGHT/2,9,startY + DEFAULT_ROW_HEADING_HEIGHT/2);
                if(mSequenceElements->GetVisibleRowInformation(i)->Collapsed)
                {
                    dc.DrawLine(6,startY + DEFAULT_ROW_HEADING_HEIGHT/2 + 4,6,startY + DEFAULT_ROW_HEADING_HEIGHT/2 - 4);
                }
                dc.SetPen(penOutline);
                dc.SetBrush(brush2);
            }
            // draw Model Group icon if necessary
            Model *m = mSequenceElements->GetXLightsFrame()->AllModels[mSequenceElements->GetVisibleRowInformation(i)->element->GetModelName()];
            if (m != nullptr)
            {
                if (m->GetDisplayAs() == "ModelGroup")
                {
                    dc.DrawBitmap(model_group_icon, getWidth() - 25, startY + 3, true);
                }
                else if (m->GetStringType().find("Single Color") == 0)
                {
                    if (m->GetNodeCount() > 0)
                    {
                        m->SetNodeColor(0, xlWHITE);
                        xlColor color = m->GetNodeColor(0);
                        dc.SetPen(*wxBLACK_PEN);
                        dc.SetBrush(wxBrush(color.asWxColor()));
                        dc.DrawRectangle(getWidth() - 21, startY + 5, 12, 12);
                        dc.SetPen(penOutline);
                        dc.SetBrush(brush2);
                    }
                }
            }
        }
        else if(mSequenceElements->GetVisibleRowInformation(i)->element->GetType()==ELEMENT_TYPE_TIMING)
        {
            if( mSequenceElements->GetVisibleRowInformation(i)->layerIndex == 0 )
            {
                dc.SetPen(*wxBLACK_PEN);
                if(dynamic_cast<TimingElement*>(mSequenceElements->GetVisibleRowInformation(i)->element)->GetActive())
                {
                    dc.SetBrush(*wxWHITE_BRUSH);
                    dc.DrawCircle(7,startY + DEFAULT_ROW_HEADING_HEIGHT/2,5);

                    dc.SetBrush(*wxGREY_BRUSH);
                    dc.DrawCircle(7,startY + DEFAULT_ROW_HEADING_HEIGHT/2,2);
                }
                else
                {
                    dc.SetBrush(*wxWHITE_BRUSH);
                    dc.DrawCircle(7,startY + DEFAULT_ROW_HEADING_HEIGHT/2,5);
                }
                dc.SetPen(penOutline);
                dc.SetBrush(brush2);
                if(mSequenceElements->GetVisibleRowInformation(i)->element->GetEffectLayerCount() > 1)
                {
                    dc.DrawBitmap(papagayo_icon, getWidth()-25, startY+3, true);
                }
            }
        }
        row++;
    }
    wxBrush b(mHeaderColorModel->asWxColor(),wxBRUSHSTYLE_SOLID);
    dc.SetBrush(b);
    dc.DrawRectangle(0,endY,w,h);
}

const xlColor* RowHeading::GetHeaderColor(Row_Information_Struct* info)
{
    if (info->element->GetType() == ELEMENT_TYPE_TIMING)
    {
        return GetTimingColor(info->colorIndex);
    }

    if (info->RowNumber == mSelectedRow )
    //if (info->element->GetSelected())
    {
        return  mHeaderSelectedColor;
    }
    else
    {
        return mHeaderColorModel;
    }
}

void RowHeading::SetSequenceElements(SequenceElements* elements)
{
    mSequenceElements = elements;
}

void RowHeading::DrawHeading(wxPaintDC* dc, wxXmlNode* model,int width,int row)
{
}

int RowHeading::getWidth()
{
    return GetSize().x;
}

int RowHeading::getHeight()
{
    return GetSize().y;
}

const xlColor* RowHeading::GetTimingColor(int colorIndex)
{
    const xlColor* value;
    switch(colorIndex%5)
    {
        case 0:
            //
            value = &xlCYAN;
            break;
        case 1:
            value = &xlRED;
            break;
        case 2:
            value = &xlGREEN;
            break;
        case 3:
            value = &xlBLUE;
            break;
        default:
            value = &xlYELLOW;
            break;
    }
    return value;
}







