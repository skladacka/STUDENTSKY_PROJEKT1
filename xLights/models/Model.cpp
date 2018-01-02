#include <wx/wx.h>

#include "Model.h"

#include <wx/xml/xml.h>
#include <wx/tokenzr.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#include "../xLightsMain.h" //for Preview and Other model collections
#include "../xLightsXmlFile.h"
#include "../Color.h"
#include "../DrawGLUtils.h"
#include "../DimmingCurve.h"

#include "../StrandNodeNamesDialog.h"
#include "../ModelFaceDialog.h"
#include "../ModelStateDialog.h"
#include "../ModelDimmingCurveDialog.h"
#include "../StartChannelDialog.h"
#include "../SubModelsDialog.h"
#include "../ControllerConnectionDialog.h"
#include "../outputs/Output.h"
#include "../outputs/IPOutput.h"

#include "ModelScreenLocation.h"
#include <wx/sstream.h>

#include "SubModel.h"
#include "../UtilFunctions.h"
#include "xLightsVersion.h"


static const std::string DEFAULT("Default");
static const std::string PER_PREVIEW("Per Preview");
static const std::string SINGLE_LINE("Single Line");
static const std::string AS_PIXEL("As Pixel");
static const std::string VERT_PER_STRAND("Vertical Per Strand");
static const std::string HORIZ_PER_STRAND("Horizontal Per Strand");

static const std::string PER_PREVIEW_NO_OFFSET("Per Preview No Offset");

#define retmsg(msg)  \
{ \
wxMessageBox(msg, _("Export Error")); \
return; \
}

const std::vector<std::string> Model::DEFAULT_BUFFER_STYLES {DEFAULT, PER_PREVIEW, SINGLE_LINE, AS_PIXEL};

Model::Model(const ModelManager &manager) : modelDimmingCurve(nullptr), ModelXml(nullptr),
    parm1(0), parm2(0), parm3(0), pixelStyle(1), pixelSize(2), transparency(0), blackTransparency(0),
    StrobeRate(0), changeCount(0), modelManager(manager), CouldComputeStartChannel(false), maxVertexCount(0),
    splitRGB(false)
{
}

Model::~Model() {
    if (modelDimmingCurve != nullptr) {
        delete modelDimmingCurve;
    }
    for (auto it = subModels.begin(); it != subModels.end(); ++it) {
        Model *m = *it;
        delete m;
    }
}

static const std::string CLICK_TO_EDIT("--Click To Edit--");
class StrandNodeNamesDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    StrandNodeNamesDialogAdapter(const Model *model)
    : wxPGEditorDialogAdapter(), m_model(model) {
    }
    virtual bool DoShowDialog(wxPropertyGrid* propGrid,
                              wxPGProperty* WXUNUSED(property) ) override {
        StrandNodeNamesDialog dlg(propGrid);
        dlg.Setup(m_model,
                  m_model->GetModelXml()->GetAttribute("NodeNames").ToStdString(),
                  m_model->GetModelXml()->GetAttribute("StrandNames").ToStdString());
        if (dlg.ShowModal() == wxID_OK) {
            m_model->GetModelXml()->DeleteAttribute("NodeNames");
            m_model->GetModelXml()->DeleteAttribute("StrandNames");
            m_model->GetModelXml()->AddAttribute("NodeNames", dlg.GetNodeNames());
            m_model->GetModelXml()->AddAttribute("StrandNames", dlg.GetStrandNames());
            wxVariant v(CLICK_TO_EDIT);
            SetValue(v);
            return true;
        }
        return false;
    }
protected:
    const Model *m_model;
};

class FacesDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    FacesDialogAdapter(Model *model)
    : wxPGEditorDialogAdapter(), m_model(model) {
    }
    virtual bool DoShowDialog(wxPropertyGrid* propGrid,
                              wxPGProperty* WXUNUSED(property) ) override {
        ModelFaceDialog dlg(propGrid);
        dlg.SetFaceInfo(m_model, m_model->faceInfo);
        if (dlg.ShowModal() == wxID_OK) {
            m_model->faceInfo.clear();
            dlg.GetFaceInfo(m_model->faceInfo);
            wxVariant v(CLICK_TO_EDIT);
            SetValue(v);
            return true;
        }
        return false;
    }
protected:
    Model *m_model;
};
class StatesDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    StatesDialogAdapter(Model *model)
        : wxPGEditorDialogAdapter(), m_model(model) {
    }
    virtual bool DoShowDialog(wxPropertyGrid* propGrid,
        wxPGProperty* WXUNUSED(property)) override {
        ModelStateDialog dlg(propGrid);
        dlg.SetStateInfo(m_model, m_model->stateInfo);
        if (dlg.ShowModal() == wxID_OK) {
            m_model->stateInfo.clear();
            dlg.GetStateInfo(m_model->stateInfo);
            wxVariant v(CLICK_TO_EDIT);
            SetValue(v);
            return true;
        }
        return false;
    }
protected:
    Model *m_model;
};
class DimmingCurveDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    DimmingCurveDialogAdapter(const Model *model)
    : wxPGEditorDialogAdapter(), m_model(model) {
    }
    virtual bool DoShowDialog(wxPropertyGrid* propGrid,
                              wxPGProperty* WXUNUSED(property) ) override {
        ModelDimmingCurveDialog dlg(propGrid);
        std::map<std::string, std::map<std::string,std::string> > dimmingInfo;
        wxXmlNode *f = m_model->GetModelXml()->GetChildren();
        while (f != nullptr) {
            if ("dimmingCurve" == f->GetName()) {
                wxXmlNode *dc = f->GetChildren();
                while (dc != nullptr) {
                    std::string name = dc->GetName().ToStdString();
                    wxXmlAttribute *att = dc->GetAttributes();
                    while (att != nullptr) {
                        dimmingInfo[name][att->GetName().ToStdString()] = att->GetValue();
                        att = att->GetNext();
                    }
                    dc = dc->GetNext();
                }
            }
            f = f->GetNext();
        }
        if(dimmingInfo.empty()) {
            wxString b = m_model->GetModelXml()->GetAttribute("ModelBrightness","0");
            dimmingInfo["all"]["gamma"] = "1.0";
            dimmingInfo["all"]["brightness"] = b;
        }
        dlg.Init(dimmingInfo);
        if (dlg.ShowModal() == wxID_OK) {
            dimmingInfo.clear();
            dlg.Update(dimmingInfo);
            wxXmlNode *f1 = m_model->GetModelXml()->GetChildren();
            while (f1 != nullptr) {
                if ("dimmingCurve" == f1->GetName()) {
                    m_model->GetModelXml()->RemoveChild(f1);
                    delete f1;
                    f1 = m_model->GetModelXml()->GetChildren();
                } else {
                    f1 = f1->GetNext();
                }
            }
            f1 = new wxXmlNode(wxXML_ELEMENT_NODE , "dimmingCurve");
            m_model->GetModelXml()->AddChild(f1);
            for (auto it = dimmingInfo.begin(); it != dimmingInfo.end(); ++it) {
                wxXmlNode *dc = new wxXmlNode(wxXML_ELEMENT_NODE , it->first);
                f1->AddChild(dc);
                for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                    dc->AddAttribute(it2->first, it2->second);
                }
            }
            wxVariant v(CLICK_TO_EDIT);
            SetValue(v);
            return true;
        }
        return false;
    }
protected:
    const Model *m_model;
};
class SubModelsDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    SubModelsDialogAdapter(Model *model)
    : wxPGEditorDialogAdapter(), m_model(model) {
    }
    virtual bool DoShowDialog(wxPropertyGrid* propGrid,
                              wxPGProperty* WXUNUSED(property) ) override {
        SubModelsDialog dlg(propGrid);
        dlg.Setup(m_model);
        if (dlg.ShowModal() == wxID_OK) {
            dlg.Save();
            wxVariant v(CLICK_TO_EDIT);
            SetValue(v);
            return true;
        }
        return false;
    }
protected:
    Model *m_model;
};

class ControllerConnectionDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    ControllerConnectionDialogAdapter(Model *model)
        : wxPGEditorDialogAdapter(), m_model(model) {
    }
    virtual bool DoShowDialog(wxPropertyGrid* propGrid,
        wxPGProperty* property) override {
        ControllerConnectionDialog dlg(propGrid);
        dlg.Set(property->GetValue().GetString());
        if (dlg.ShowModal() == wxID_OK) {
            wxVariant v(dlg.Get());
            SetValue(v);
            return true;
        }
        return false;
    }
protected:
    Model *m_model;
};


class PopupDialogProperty : public wxStringProperty
{
public:
    PopupDialogProperty(Model *m,
                        const wxString& label,
                        const wxString& name,
                        const wxString& value,
                        int type)
    : wxStringProperty(label, name, value), m_model(m), m_tp(type) {
    }
    // Set editor to have button
    virtual const wxPGEditor* DoGetEditorClass() const override {
        return wxPGEditor_TextCtrlAndButton;
    }
    // Set what happens on button click
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const override {
        switch (m_tp) {
        case 1:
            return new StrandNodeNamesDialogAdapter(m_model);
        case 2:
            return new FacesDialogAdapter(m_model);
        case 3:
            return new DimmingCurveDialogAdapter(m_model);
        case 4:
            return new StatesDialogAdapter(m_model);
        case 5:
            return new SubModelsDialogAdapter(m_model);
        }
        return nullptr;
    }
protected:
    Model *m_model;
    int m_tp;
};

class StartChannelDialogAdapter : public wxPGEditorDialogAdapter
{
public:
    StartChannelDialogAdapter(Model *model)
    : wxPGEditorDialogAdapter(), m_model(model) {
    }
    virtual bool DoShowDialog(wxPropertyGrid* propGrid,
                              wxPGProperty* property) override {
        StartChannelDialog dlg(propGrid);
        dlg.Set(property->GetValue().GetString(), m_model->GetModelManager());
        if (dlg.ShowModal() == wxID_OK) {
            wxVariant v(dlg.Get());
            SetValue(v);
            return true;
        }
        return false;
    }
protected:
    Model *m_model;
};

class StartChannelProperty : public wxStringProperty
{
public:
    StartChannelProperty(Model *m,
                         int strand,
                        const wxString& label,
                        const wxString& name,
                        const wxString& value)
    : wxStringProperty(label, name, value), m_model(m), m_strand(strand) {
    }
    // Set editor to have button
    virtual const wxPGEditor* DoGetEditorClass() const override {
        return wxPGEditor_TextCtrlAndButton;
    }
    // Set what happens on button click
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const override {
        return new StartChannelDialogAdapter(m_model);
    }
protected:
    Model *m_model;
    int m_strand;
};

class ControllerConnectionProperty : public wxStringProperty
{
public:
    ControllerConnectionProperty(Model *m,
        const wxString& label,
        const wxString& name,
        const wxString& value)
        : wxStringProperty(label, name, value), m_model(m) {
    }
    // Set editor to have button
    virtual const wxPGEditor* DoGetEditorClass() const override {
        return wxPGEditor_TextCtrlAndButton;
    }
    // Set what happens on button click
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const override {
        return new ControllerConnectionDialogAdapter(m_model);
    }
protected:
    Model *m_model;
};

static wxArrayString NODE_TYPES;
static wxArrayString PIXEL_STYLES;
static wxArrayString LAYOUT_GROUPS;

wxArrayString Model::GetLayoutGroups(const ModelManager& mm)
{
    wxArrayString lg;
    lg.push_back("Default");
    lg.push_back("All Previews");
    lg.push_back("Unassigned");

    wxXmlNode* layouts_node = mm.GetLayoutsNode();
    for (wxXmlNode* e = layouts_node->GetChildren(); e != nullptr; e = e->GetNext())
    {
        if (e->GetName() == "layoutGroup")
        {
            wxString grp_name = e->GetAttribute("name");
            if (!grp_name.IsEmpty())
            {
                lg.push_back(grp_name.ToStdString());
            }
        }
    }

    return lg;
}

void Model::SetProperty(wxString property, wxString value, bool apply)
{
    wxString val = ModelXml->GetAttribute(property);
    if (val != "")
    {
        ModelXml->DeleteAttribute(property);
        ModelXml->AddAttribute(property, value);
    }
    else
    {
        ModelXml->AddAttribute(property, value);
    }
    if (apply)
    {
        SetFromXml(ModelXml);
    }
}

void Model::AddProperties(wxPropertyGridInterface *grid) {
    if (PIXEL_STYLES.empty()) {
        PIXEL_STYLES.push_back("Square");
        PIXEL_STYLES.push_back("Smooth");
        PIXEL_STYLES.push_back("Solid Circle");
        PIXEL_STYLES.push_back("Blended Circle");


        NODE_TYPES.push_back("RGB Nodes");
        NODE_TYPES.push_back("RBG Nodes");
        NODE_TYPES.push_back("GBR Nodes");
        NODE_TYPES.push_back("GRB Nodes");
        NODE_TYPES.push_back("BRG Nodes");
        NODE_TYPES.push_back("BGR Nodes");
        NODE_TYPES.push_back("3 Channel RGB");
        NODE_TYPES.push_back("4 Channel RGBW");
        NODE_TYPES.push_back("Strobes");
        NODE_TYPES.push_back("Single Color");
        NODE_TYPES.push_back("Single Color Intensity");
    }

    LAYOUT_GROUPS = Model::GetLayoutGroups(modelManager);

    wxPGProperty *p;
    wxPGProperty *sp;

    p = grid->Append(new wxPropertyCategory(DisplayAs, "ModelType"));
    p->GetCell(0).SetFgCol(*wxBLACK);

    AddTypeProperties(grid);

    if (HasOneString(DisplayAs)) {
        grid->Append(new StartChannelProperty(this, 0, "Start Channel", "ModelStartChannel", ModelXml->GetAttribute("StartChannel","1")));
    } else {
        bool hasIndiv = ModelXml->GetAttribute("Advanced", "0") == "1";
        p = grid->Append(new wxBoolProperty("Indiv Start Chans", "ModelIndividualStartChannels", hasIndiv));
        p->SetAttribute("UseCheckbox", true);
        p->Enable(parm1 > 1);
        sp = grid->AppendIn(p, new StartChannelProperty(this, 0, "Start Channel", "ModelStartChannel", ModelXml->GetAttribute("StartChannel","1")));
        if (hasIndiv) {
            int c = Model::HasOneString(DisplayAs) ? 1 : parm1;
            for (int x = 0; x < c; x++) {
                wxString nm = StartChanAttrName(x);
                std::string val = ModelXml->GetAttribute(nm).ToStdString();
                if (val == "") {
                    val = ComputeStringStartChannel(x);
                    ModelXml->DeleteAttribute(nm);
                    ModelXml->AddAttribute(nm, val);
                }
                if (x == 0) {
                    sp->SetLabel(nm);
                    sp->SetValue(val);
                } else {
                    sp = grid->AppendIn(p, new StartChannelProperty(this, x, nm, nm, val));
                }
            }
        }
    }

    int layout_group_number = 0;
    for( int grp=0; grp < LAYOUT_GROUPS.Count(); grp++)
    {
        if( LAYOUT_GROUPS[grp] == layout_group )
        {
            layout_group_number = grp;
            break;
        }
    }

    grid->Append(new wxStringProperty("Description", "Description", description));
    grid->Append(new wxEnumProperty("Preview", "ModelLayoutGroup", LAYOUT_GROUPS, wxArrayInt(), layout_group_number));

    p = grid->Append(new PopupDialogProperty(this, "Strand/Node Names", "ModelStrandNodeNames", CLICK_TO_EDIT, 1));
    grid->LimitPropertyEditing(p);
    p = grid->Append(new PopupDialogProperty(this, "Faces", "ModelFaces", CLICK_TO_EDIT, 2));
    grid->LimitPropertyEditing(p);
    p = grid->Append(new PopupDialogProperty(this, "Dimming Curves", "ModelDimmingCurves", CLICK_TO_EDIT, 3));
    grid->LimitPropertyEditing(p);
    p = grid->Append(new PopupDialogProperty(this, "States", "ModelStates", CLICK_TO_EDIT, 4));
    grid->LimitPropertyEditing(p);
    p = grid->Append(new PopupDialogProperty(this, "Sub-Models", "SubModels", CLICK_TO_EDIT, 5));
    grid->LimitPropertyEditing(p);
    grid->Append(new ControllerConnectionProperty(this, "Controller Connection", "ControllerConnection", ModelXml->GetAttribute("ControllerConnection", "")));

    p = grid->Append(new wxPropertyCategory("String Properties", "ModelStringProperties"));
    p->GetCell(0).SetFgCol(*wxBLACK);
    int i = NODE_TYPES.Index(StringType);
    if (i == wxNOT_FOUND) {
        i = NODE_TYPES.size() - 2;
    }
    grid->AppendIn(p, new wxEnumProperty("String Type", "ModelStringType", NODE_TYPES, wxArrayInt(), i));
    if (i == NODE_TYPES.size() - 1 || i == NODE_TYPES.size() - 2)  {
        //get the color
        wxColor v;
        if (StringType=="Single Color Red") {
            v = *wxRED;
        } else if (StringType=="Single Color Green" || StringType == "G") {
            v = *wxGREEN;
        } else if (StringType=="Single Color Blue" || StringType == "B") {
            v = *wxBLUE;
        } else if (StringType=="Single Color White" || StringType == "W") {
            v = *wxWHITE;
        } else if (StringType=="Single Color Custom" || StringType=="Single Color Intensity") {
            v = customColor.asWxColor();
        } else if (StringType[0] == '#') {
            v = xlColor(StringType).asWxColor();
        }
        grid->AppendIn(p, new wxColourProperty("Color", "ModelStringColor", v));
    } else {
        sp = grid->AppendIn(p, new wxColourProperty("Color", "ModelStringColor", *wxRED));
        sp->Enable(false);
    }

    p = grid->Append(new wxPropertyCategory("Appearance", "ModelAppearance"));
    p->GetCell(0).SetFgCol(*wxBLACK);
    sp = grid->AppendIn(p, new wxUIntProperty("Pixel Size", "ModelPixelSize", pixelSize));
    sp->SetAttribute("Min", 1);
    sp->SetAttribute("Max", 300);
    sp->SetEditor("SpinCtrl");

    grid->AppendIn(p, new wxEnumProperty("Pixel Style", "ModelPixelStyle", PIXEL_STYLES, wxArrayInt(), pixelStyle));
    sp = grid->AppendIn(p, new wxUIntProperty("Transparency", "ModelPixelTransparency", transparency));
    sp->SetAttribute("Min", 0);
    sp->SetAttribute("Max", 100);
    sp->SetEditor("SpinCtrl");
    sp = grid->AppendIn(p, new wxUIntProperty("Black Transparency", "ModelPixelBlackTransparency", blackTransparency));
    sp->SetAttribute("Min", 0);
    sp->SetAttribute("Max", 100);
    sp->SetEditor("SpinCtrl");

    DisableUnusedProperties(grid);
}

static wxString GetColorString(wxPGProperty *p, xlColor &xc) {
    wxString tp = "Single Color Custom";
    wxColour c;
    c << p->GetValue();
    if (c == *wxRED) {
        tp = "Single Color Red";
    } else if (c == *wxBLUE) {
        tp = "Single Color Blue";
    } else if (c == *wxGREEN) {
        tp = "Single Color Green";
    } else if (c == *wxWHITE) {
        tp = "Single Color White";
    } else {
        xc = c;
    }
    return tp;
}

int Model::OnPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event) {
    if (event.GetPropertyName() == "ModelPixelSize") {
        pixelSize = event.GetValue().GetLong();
        ModelXml->DeleteAttribute("PixelSize");
        ModelXml->AddAttribute("PixelSize", wxString::Format(wxT("%i"), pixelSize));
        IncrementChangeCount();
        return 3;
    } else if (event.GetPropertyName() == "ModelPixelStyle") {
        pixelStyle = event.GetValue().GetLong();
        ModelXml->DeleteAttribute("Antialias");
        ModelXml->AddAttribute("Antialias", wxString::Format(wxT("%i"), pixelStyle));
        IncrementChangeCount();
        return 3;
    } else if (event.GetPropertyName() == "ModelPixelTransparency") {
        transparency = event.GetValue().GetLong();
        ModelXml->DeleteAttribute("Transparency");
        ModelXml->AddAttribute("Transparency", wxString::Format(wxT("%i"), transparency));
        IncrementChangeCount();
        return 3;
    } else if (event.GetPropertyName() == "ModelPixelBlackTransparency") {
        blackTransparency = event.GetValue().GetLong();
        ModelXml->DeleteAttribute("BlackTransparency");
        ModelXml->AddAttribute("BlackTransparency", wxString::Format(wxT("%i"), blackTransparency));
        IncrementChangeCount();
        return 3;
    } else if (event.GetPropertyName() == "ModelStrandNodeNames") {
        SetFromXml(ModelXml, zeroBased);
        IncrementChangeCount();
        return 2;
    } else if (event.GetPropertyName() == "ModelDimmingCurves") {
        SetFromXml(ModelXml, zeroBased);
        IncrementChangeCount();
        return 2;
    } else if (event.GetPropertyName() == "ControllerConnection") {
        controller_connection = event.GetValue().GetString();
        ModelXml->DeleteAttribute("ControllerConnection");
        if (controller_connection != "")
        {
            ModelXml->AddAttribute("ControllerConnection", controller_connection);
        }
        IncrementChangeCount();
        return 2;
    }
    else if (event.GetPropertyName() == "SubModels") {
        SetFromXml(ModelXml, zeroBased);
        IncrementChangeCount();
        return 2;
    }
    else if (event.GetPropertyName() == "Description") {
        description = event.GetValue().GetString();
        ModelXml->DeleteAttribute("Description");
        if (description != "")
        {
            ModelXml->AddAttribute("Description", XmlSafe(description));
        }
        IncrementChangeCount();
        return 2;
    } else if (event.GetPropertyName() == "ModelFaces") {
        wxXmlNode *f = ModelXml->GetChildren();
        while (f != nullptr) {
            if ("faceInfo" == f->GetName()) {
                ModelXml->RemoveChild(f);
                delete f;
                f = ModelXml->GetChildren();
            } else {
                f = f->GetNext();
            }
        }
        Model::WriteFaceInfo(ModelXml, faceInfo);
        IncrementChangeCount();
        return 2;
    } else if (event.GetPropertyName() == "ModelStates") {
        wxXmlNode *f = ModelXml->GetChildren();
        while (f != nullptr) {
            if ("stateInfo" == f->GetName()) {
                ModelXml->RemoveChild(f);
                delete f;
                f = ModelXml->GetChildren();
            }
            else {
                f = f->GetNext();
            }
        }
        Model::WriteStateInfo(ModelXml, stateInfo);
        IncrementChangeCount();
        return 2;
    } else if (event.GetPropertyName() == "ModelStringColor"
               || event.GetPropertyName() == "ModelStringType") {
        wxPGProperty *p2 = grid->GetPropertyByName("ModelStringType");
        int i = p2->GetValue().GetLong();
        ModelXml->DeleteAttribute("StringType");
        if (i == NODE_TYPES.size() - 1 || i == NODE_TYPES.size() - 2) {
            wxPGProperty *p = grid->GetPropertyByName("ModelStringColor");
            xlColor c;
            wxString tp = GetColorString(p, c);
			if (NODE_TYPES[i] == "Single Color Intensity") {
				tp = "Single Color Intensity";
			}
            p->Enable();
            if (tp == "Single Color Custom" || tp == "Single Color Intensity") {
                ModelXml->DeleteAttribute("CustomColor");
                xlColor xc = c;
                ModelXml->AddAttribute("CustomColor", xc);
            }
            ModelXml->AddAttribute("StringType", tp);
        } else {
            ModelXml->AddAttribute("StringType", NODE_TYPES[i]);
            grid->GetPropertyByName("ModelStringColor")->Enable(false);
        }
        SetFromXml(ModelXml, zeroBased);
        IncrementChangeCount();
        return 3 | ((event.GetPropertyName() == "ModelStringType" ) ? 0x0008 : 0);
    } else if (event.GetPropertyName() == "ModelStartChannel" || event.GetPropertyName() == "ModelIndividualStartChannels.ModelStartChannel") {
        ModelXml->DeleteAttribute("StartChannel");
        ModelXml->AddAttribute("StartChannel", event.GetValue().GetString());
        if (ModelXml->GetAttribute("Advanced") == "1") {
            ModelXml->DeleteAttribute(StartChanAttrName(0));
            ModelXml->AddAttribute(StartChanAttrName(0), event.GetValue().GetString());
        }
        RecalcStartChannels();
        IncrementChangeCount();
        return 3 | 0x0008;
    } else if (event.GetPropertyName() == "ModelIndividualStartChannels") {
        ModelXml->DeleteAttribute("Advanced");
        int c = Model::HasOneString(DisplayAs) ? 1 : parm1;
        if (event.GetValue().GetBool()) {
            ModelXml->AddAttribute("Advanced", "1");
            for (int x = 0; x < c; x++) {
                if (ModelXml->GetAttribute(StartChanAttrName(x)) == "") {
                    ModelXml->DeleteAttribute(StartChanAttrName(x));
                    ModelXml->AddAttribute(StartChanAttrName(x),
                                           ComputeStringStartChannel(x));
                }
            }
        } else {
            for (int x = 0; x < c; x++) {
                ModelXml->DeleteAttribute(StartChanAttrName(x));
            }
        }
        RecalcStartChannels();
        AdjustStringProperties(grid, parm1);
        IncrementChangeCount();
        return 3 | 0x0008;
    } else if (event.GetPropertyName().StartsWith("ModelIndividualStartChannels.")) {
        wxString str = event.GetPropertyName();
        str = str.SubString(str.Find(".") + 1, str.length());
        ModelXml->DeleteAttribute(str);
        ModelXml->AddAttribute(str, event.GetValue().GetString());
        RecalcStartChannels();
        IncrementChangeCount();
        return 3 | 0x0008;
    } else if (event.GetPropertyName() == "ModelLayoutGroup") {
        layout_group = LAYOUT_GROUPS[event.GetValue().GetLong()];
        ModelXml->DeleteAttribute("LayoutGroup");
        ModelXml->AddAttribute("LayoutGroup", layout_group);
        IncrementChangeCount();
        return 3 | 0x0010;
    }

    int i = GetModelScreenLocation().OnPropertyGridChange(grid, event);

    if (i & 0x2) {
        GetModelScreenLocation().Write(ModelXml);
        SetFromXml(ModelXml, zeroBased);
        IncrementChangeCount();
    }

    return i;
}

void Model::AdjustStringProperties(wxPropertyGridInterface *grid, int newNum) {
    wxPropertyGrid *pg = (wxPropertyGrid*)grid;
    wxPGProperty *p = grid->GetPropertyByName("ModelIndividualStartChannels");
    if (p != nullptr) {
        pg->Freeze();
        bool adv = p->GetValue().GetBool();
        if (adv) {
            int count = p->GetChildCount();
            while (count > newNum) {
                count--;
                wxString nm = StartChanAttrName(count);
                wxPGProperty *sp = grid->GetPropertyByName("ModelIndividualStartChannels." + nm);
                if (sp != nullptr) {
                    grid->DeleteProperty(sp);
                }
            }
            while (count < newNum) {
                wxString nm = StartChanAttrName(count);
                std::string val = ModelXml->GetAttribute(nm).ToStdString();
                if (val == "") {
                    val = ComputeStringStartChannel(count);
                    ModelXml->DeleteAttribute(nm);
                    ModelXml->AddAttribute(nm, val);
                }
                grid->AppendIn(p, new StartChannelProperty(this, count, nm, nm, val));
                count++;
            }
        } else if (p->GetChildCount() > 1) {
            int count = p->GetChildCount();
            for (int x = 1; x < count; x++) {
                wxString nm = StartChanAttrName(x);
                wxPGProperty *sp = grid->GetPropertyByName("ModelIndividualStartChannels." + nm);
                if (sp != nullptr) {
                    grid->DeleteProperty(sp);
                }
            }
        }
        wxPGProperty *sp = grid->GetPropertyByName("ModelIndividualStartChannels.ModelStartChannel");
        if (adv) {
            sp->SetLabel(StartChanAttrName(0));
        } else {
            sp->SetLabel("Start Channel");
        }
        p->Enable(parm1 > 1);
        pg->Thaw();
        pg->RefreshGrid();
    }
}

void Model::ParseFaceInfo(wxXmlNode *f, std::map<std::string, std::map<std::string, std::string> > &faceInfo) {
    std::string name = f->GetAttribute("Name", "SingleNode").ToStdString();
    std::string type = f->GetAttribute("Type", "SingleNode").ToStdString();
    if (name == "") {
        name = type;
        f->DeleteAttribute("Name");
        f->AddAttribute("Name", type);
    }
    if (!(type == "SingleNode" || type == "NodeRange" || type == "Matrix")) {
        if (type == "Coro") {
            type = "SingleNode";
        } else {
            type = "Matrix";
        }
        f->DeleteAttribute("Type");
        f->AddAttribute("Type", type);
    }
    wxXmlAttribute *att = f->GetAttributes();
    while (att != nullptr) {
        if (att->GetName() != "Name")
        {
            if (att->GetName().Left(5) == "Mouth" || att->GetName().Left(4) == "Eyes")
            {
                faceInfo[name][att->GetName().ToStdString()] = FixFile("", att->GetValue());
            }
            else
            {
                faceInfo[name][att->GetName().ToStdString()] = att->GetValue();
            }
        }
        att = att->GetNext();
    }
}
void Model::WriteFaceInfo(wxXmlNode *rootXml, const std::map<std::string, std::map<std::string, std::string> > &faceInfo) {
    if (!faceInfo.empty()) {
        for (auto it = faceInfo.begin(); it != faceInfo.end(); ++it) {
            wxXmlNode *f = new wxXmlNode(rootXml, wxXML_ELEMENT_NODE , "faceInfo");
            std::string name = it->first;
            f->AddAttribute("Name", name);
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                f->AddAttribute(it2->first, it2->second);
            }
        }
    }
}

void Model::AddFace(wxXmlNode* n)
{
    ParseFaceInfo(n, faceInfo);
    Model::WriteFaceInfo(ModelXml, faceInfo);
}

void Model::AddState(wxXmlNode* n)
{
    ParseStateInfo(n, stateInfo);
    Model::WriteStateInfo(ModelXml, stateInfo);
}

void Model::AddSubmodel(wxXmlNode* n)
{
    ParseSubModel(n);

    // this may break if the submodel format changes and the user loads an old format ... if that happens this needs to go through a upgrade routine
    wxXmlNode *f = new wxXmlNode(wxXML_ELEMENT_NODE, "subModel");
    ModelXml->AddChild(f);
    for (auto a = n->GetAttributes(); a!= nullptr; a = a->GetNext())
    {
        f->AddAttribute(a->GetName(), a->GetValue());
    }
}

wxString Model::SerialiseFace() const
{
    wxString res = "";

    if (!faceInfo.empty()) {
        for (auto it = faceInfo.begin(); it != faceInfo.end(); ++it) {
            res += "    <faceInfo Name=\"" + it->first + "\" ";
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                res += it2->first + "=\"" + it2->second + "\" ";
            }
            res += "/>\n";
        }
    }

    return res;
}

void Model::ParseStateInfo(wxXmlNode *f, std::map<std::string, std::map<std::string, std::string> > &stateInfo) {
    std::string name = f->GetAttribute("Name", "SingleNode").ToStdString();
    std::string type = f->GetAttribute("Type", "SingleNode").ToStdString();
    if (name == "") {
        name = type;
        f->DeleteAttribute("Name");
        f->AddAttribute("Name", type);
    }
    if (!(type == "SingleNode" || type == "NodeRange")) {
        if (type == "Coro") {
            type = "SingleNode";
        }
        f->DeleteAttribute("Type");
        f->AddAttribute("Type", type);
    }
    wxXmlAttribute *att = f->GetAttributes();
    while (att != nullptr) {
        if (att->GetName() != "Name")
        {
                stateInfo[name][att->GetName().ToStdString()] = att->GetValue();
        }
        att = att->GetNext();
    }
}

void Model::WriteStateInfo(wxXmlNode *rootXml, const std::map<std::string, std::map<std::string, std::string> > &stateInfo) {
    if (!stateInfo.empty()) {
        for (auto it = stateInfo.begin(); it != stateInfo.end(); ++it) {
            std::string name = it->first;
            if (wxString(name).Trim(true).Trim(false) != "")
            {
                wxXmlNode *f = new wxXmlNode(rootXml, wxXML_ELEMENT_NODE, "stateInfo");
                f->AddAttribute("Name", name);
                for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                    if (wxString(it2->first).Trim(true).Trim(false) != "")
                        f->AddAttribute(it2->first, it2->second);
                }
            }
        }
    }
}

wxString Model::SerialiseState() const
{
    wxString res = "";

    if (!stateInfo.empty()) {
        for (auto it = stateInfo.begin(); it != stateInfo.end(); ++it) {
            res += "    <stateInfo Name=\"" + it->first + "\" ";
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                if (wxString(it2->first).Trim(false).Trim(true) != "")
                {
                    res += it2->first + "=\"" + it2->second + "\" ";
                }
            }
            res += "/>\n";
        }
    }

    return res;
}

std::string Model::ComputeStringStartChannel(int i) {
    if (i == 0) {
        return ModelXml->GetAttribute("StartChannel", "1").ToStdString();
    }
    wxString stch = ModelXml->GetAttribute(StartChanAttrName(i - 1));
    wxString sNet = "";
    int ChannelsPerString = CalcCannelsPerString();
    long StringStartChanLong = 0;
    if (stch.Contains(":")) {
        sNet = stch.SubString(0, stch.Find(":")-1);
        stch = stch.SubString(stch.Find(":") + 1, stch.size());
        long startNetwork;
        if (sNet.ToLong(&startNetwork) && startNetwork > 0) {
            startNetwork--; // Zero based index
            if (stch.ToLong(&StringStartChanLong) && StringStartChanLong > 0) {
                // get the string end channel
                long ststch;
                Output* o = modelManager.GetOutputManager()->GetOutput(StringStartChanLong + ChannelsPerString + 1, ststch);
                if (o != nullptr)
                {
                    return wxString::Format("%i:%i",o->GetOutputNumber(), ststch).ToStdString();
                }
            }
        } else if (stch.ToLong(&StringStartChanLong) && StringStartChanLong > 0) {
            long StringEndChan=StringStartChanLong + ChannelsPerString;
            stch = sNet + wxString::Format(":%d", StringEndChan);
        }
    }
    if (stch.ToLong(&StringStartChanLong) && StringStartChanLong > 0) {
        long StringEndChan=StringStartChanLong + ChannelsPerString;
        stch = wxString::Format(wxT("%i"), StringEndChan);
    }
    return stch.ToStdString();
}

int Model::GetNumStrands() const {
    return 1;
}

wxXmlNode* Model::GetModelXml() const {
    return this->ModelXml;
}

bool Model::ModelRenamed(const std::string &oldName, const std::string &newName) {
    bool changed = false;
    std::string sc = ModelXml->GetAttribute("StartChannel","1").ToStdString();
    if (sc[0] == '@' || sc[0] == '<' || sc[0] == '>') {
        std::string mn = sc.substr(1, sc.find(':')-1);
        if (mn == oldName) {
            sc = sc[0] + newName + sc.substr(sc.find(':'), sc.size());
            ModelXml->DeleteAttribute("StartChannel");
            ModelXml->AddAttribute("StartChannel", sc);
            changed = true;
        }
    }

    for (size_t i=0; i<stringStartChan.size(); i++) {
        std::string tempstr = StartChanAttrName(i);
        if (ModelXml->HasAttribute(tempstr)) {
            sc = ModelXml->GetAttribute(tempstr, "1").ToStdString();
            if (sc[0] == '@' || sc[0] == '<' || sc[0] == '>') {
                std::string mn = sc.substr(1, sc.find(':')-1);
                if (mn == oldName) {
                    sc = sc[0] + newName + sc.substr(sc.find(':'), sc.size());
                    ModelXml->DeleteAttribute(tempstr);
                    ModelXml->AddAttribute(tempstr, sc);
                    changed = true;
                }
            }
        }
    }
    if (changed) {
        SetFromXml(ModelXml, zeroBased);
    }
    return changed;
}

bool Model::IsValidStartChannelString() const
{
    wxString sc = ModelXml->GetAttribute("StartChannel");

    if (sc.IsNumber() && wxAtol(sc) > 0 && ! sc.Contains('.')) return true; // absolule

    if (!sc.Contains(':')) return false; // all other formats need a colon

    wxArrayString parts = wxSplit(sc, ':');

    if (parts.size() > 3) return false;

    if (parts[0][0] == '#')
    {
        if (parts.size() == 2)
        {
            Output* o = modelManager.GetOutputManager()->GetOutput(wxAtoi(parts[0].substr(1)), "");
            if (o != nullptr &&
                (parts[1].IsNumber() && wxAtol(parts[1]) > 0 && !parts[1].Contains('.')))
            {
                return true;
            }
        }
        else if (parts.size() == 3)
        {
            wxString ip = parts[0].substr(1);
            Output* o = modelManager.GetOutputManager()->GetOutput(wxAtoi(parts[1]), ip.ToStdString());
            if (IPOutput::IsIPValid(ip.ToStdString()) && o != nullptr &&
                (parts[2].IsNumber() && wxAtol(parts[2]) > 0 && !parts[2].Contains('.')))
            {
                return true;
            }
        }
    }
    else if (parts[0][0] == '>' || parts[0][0] == '@')
    {
        if ((parts.size() == 2) &&
            (parts[1].IsNumber() && wxAtol(parts[1]) > 0 && !parts[1].Contains('.')))
        {
            // dont bother checking the model name ... other processes will check for that
            return true;
        }
    }
    else if ((parts.size() == 2) &&
             (parts[0].IsNumber() && wxAtol(parts[0]) > 0 && !parts[0].Contains('.')) &&
             (parts[1].IsNumber() && wxAtol(parts[1]) > 0 && !parts[1].Contains('.'))) // output:startch
    {
        return true;
    }

    return false;
}

bool Model::UpdateStartChannelFromChannelString(std::map<std::string, Model*>& models, std::list<std::string>& used)
{
    bool valid = false;

    used.push_back(GetName());

    std::string dependsonmodel;
    ModelStartChannel = ModelXml->GetAttribute("StartChannel");
    long StartChannel = GetNumberFromChannelString(ModelStartChannel, valid, dependsonmodel);
    while (!valid && dependsonmodel != "" && std::find(used.begin(), used.end(), dependsonmodel) == used.end())
    {
        Model* m = models[dependsonmodel];
        if (m != nullptr)
        {
            valid = m->UpdateStartChannelFromChannelString(models, used);
        }
        if (valid)
        {
            StartChannel = GetNumberFromChannelString(ModelStartChannel, valid, dependsonmodel);
        }
    }

    if (valid)
    {
        size_t NumberOfStrings = HasOneString(DisplayAs) ? 1 : parm1;
        int ChannelsPerString = CalcCannelsPerString();
        SetStringStartChannels(zeroBased, NumberOfStrings, StartChannel, ChannelsPerString);
    }

    CouldComputeStartChannel = valid;
    return valid;
}

int Model::GetNumberFromChannelString(const std::string &sc) const {
    bool v = false;
    std::string dependsonmodel;
    return GetNumberFromChannelString(sc, v, dependsonmodel);
}

int Model::GetNumberFromChannelString(const std::string &str, bool &valid, std::string& dependsonmodel) const {
    std::string sc(str);
    int output = 1;
    valid = true;
    if (sc.find(":") != std::string::npos) {
        std::string start = sc.substr(0, sc.find(":"));
        sc = sc.substr(sc.find(":") + 1);
        if (start[0] == '@' || start[0] == '<' || start[0] == '>') {
            int returnChannel = wxAtoi(sc);
            bool fromStart = start[0] == '@';
            start = start.substr(1, start.size());
            dependsonmodel = start;
            Model *m = modelManager[start];
            if (m != nullptr && m->CouldComputeStartChannel) {
                if (fromStart) {
                    int i = m->GetFirstChannel();
                    if (i == -1 && m == this && stringStartChan.size() > 0) {
                        i = stringStartChan[0];
                    }
                    return i + returnChannel;
                }
                else {
                    return m->GetLastChannel() + returnChannel + 1;
                }
            }
            else {
                valid = false;
                output = 1;
            }
        }
        else if (start[0] == '#') {
            wxString ss = wxString(str);
            wxArrayString cs = wxSplit(ss.SubString(1, ss.Length()), ':');
            int returnChannel = 1;
            int returnUniverse = 1;
            if (cs.Count() == 3)
            {
                // #ip:universe:channel
                returnUniverse = wxAtoi(cs[1]);
                returnChannel = wxAtoi(cs[2]);

                int res = modelManager.GetOutputManager()->GetAbsoluteChannel(cs[0].Trim(false).Trim(true).ToStdString(), returnUniverse - 1, returnChannel - 1);
                if (res <= 0)
                {
                    res = 1;
                    valid = false;
                }
                return res;
            }
            else if (cs.Count() == 2)
            {
                // #universe:channel
                returnChannel = wxAtoi(sc);
                returnUniverse = wxAtoi(ss.SubString(1, ss.Find(":") - 1));

                // find output based on universe number ...
                int res = modelManager.GetOutputManager()->GetAbsoluteChannel("", returnUniverse - 1, returnChannel - 1);
                if (res <= 0)
                {
                    res = 1;
                    valid = false;
                }
                return res;
            }
            else
            {
                valid = false;
                return 1;
            }
        }
        else
        {
            output = wxAtoi(start);
            if (output == 0) {
                output = 1; // 1 based
            }
        }
    }
    int returnChannel = wxAtoi(sc);
    if (output > 1) {
        returnChannel = modelManager.GetOutputManager()->GetAbsoluteChannel(output - 1, returnChannel - 1);
        if (returnChannel < 1)
        {
            valid = false;
            returnChannel = 1;
        }
    }
    else if (returnChannel < 1)
    {
        valid = false;
        returnChannel = 1;
    }

    return returnChannel;
}

std::list<int> Model::ParseFaceNodes(std::string channels)
{
    std::list<int> res;

    wxStringTokenizer wtkz(channels, ",");
    while (wtkz.HasMoreTokens())
    {
        wxString valstr = wtkz.GetNextToken();

        int start, end;
        if (valstr.Contains("-")) {
            int idx = valstr.Index('-');
            start = wxAtoi(valstr.Left(idx));
            end = wxAtoi(valstr.Right(valstr.size() - idx - 1));
        }
        else {
            start = end = wxAtoi(valstr);
        }
        if (start > end) {
            start = end;
        }
        start--;
        end--;
        for (int n = start; n <= end; n++) {
            res.push_back(n);
        }
    }

    return res;
}

void Model::SetFromXml(wxXmlNode* ModelNode, bool zb) {
    if (modelDimmingCurve != nullptr) {
        delete modelDimmingCurve;
        modelDimmingCurve = nullptr;
    }
    for (auto it = subModels.begin(); it != subModels.end(); ++it) {
        Model *m = *it;
        delete m;
    }
    subModels.clear();

    wxString tempstr,channelstr;
    long StartChannel;

    zeroBased = zb;
    ModelXml=ModelNode;
    StrobeRate=0;
    Nodes.clear();

    name=ModelNode->GetAttribute("name").ToStdString();
    DisplayAs=ModelNode->GetAttribute("DisplayAs").ToStdString();
    controller_connection = ModelNode->GetAttribute("ControllerConnection").ToStdString();
    StringType=ModelNode->GetAttribute("StringType").ToStdString();
    SingleNode=HasSingleNode(StringType);
    SingleChannel=HasSingleChannel(StringType);
    rgbOrder = SingleNode ? "RGB" : StringType.substr(0, 3);
    description = UnXmlSafe(ModelNode->GetAttribute("Description").ToStdString());

    tempstr=ModelNode->GetAttribute("parm1");
    tempstr.ToLong(&parm1);
    tempstr=ModelNode->GetAttribute("parm2");
    tempstr.ToLong(&parm2);
    tempstr=ModelNode->GetAttribute("parm3");
    tempstr.ToLong(&parm3);
    tempstr=ModelNode->GetAttribute("StrandNames");
    strandNames.clear();
    while (tempstr.size() > 0) {
        std::string t2 = tempstr.ToStdString();
        if (tempstr[0] == ',') {
            t2 = "";
            tempstr = tempstr(1, tempstr.length());
        } else if (tempstr.Contains(",")) {
            t2 = tempstr.SubString(0, tempstr.Find(",") - 1);
            tempstr = tempstr.SubString(tempstr.Find(",") + 1, tempstr.length());
        } else {
            tempstr = "";
        }
        strandNames.push_back(t2);
    }
    tempstr=ModelNode->GetAttribute("NodeNames");
    nodeNames.clear();
    while (tempstr.size() > 0) {
        std::string t2 = tempstr.ToStdString();
        if (tempstr[0] == ',') {
            t2 = "";
            tempstr = tempstr(1, tempstr.length());
        } else if (tempstr.Contains(",")) {
            t2 = tempstr.SubString(0, tempstr.Find(",") - 1);
            tempstr = tempstr.SubString(tempstr.Find(",") + 1, tempstr.length());
        } else {
            tempstr = "";
        }
        nodeNames.push_back(t2);
    }

    CouldComputeStartChannel = false;
    std::string  dependsonmodel;
    StartChannel = GetNumberFromChannelString(ModelNode->GetAttribute("StartChannel","1").ToStdString(), CouldComputeStartChannel, dependsonmodel);
    tempstr=ModelNode->GetAttribute("Dir");
    IsLtoR=tempstr != "R";
    if (ModelNode->HasAttribute("StartSide")) {
        tempstr=ModelNode->GetAttribute("StartSide");
        isBotToTop = (tempstr == "B");
    } else {
        isBotToTop=true;
    }
    customColor = xlColor(ModelNode->GetAttribute("CustomColor", "#000000").ToStdString());

    long n;
    tempstr=ModelNode->GetAttribute("Antialias","1");
    tempstr.ToLong(&n);
    pixelStyle = n;
    tempstr=ModelNode->GetAttribute("PixelSize","2");
    tempstr.ToLong(&n);
    pixelSize = n;
    tempstr=ModelNode->GetAttribute("Transparency","0");
    tempstr.ToLong(&n);
    transparency = n;
    blackTransparency = wxAtoi(ModelNode->GetAttribute("BlackTransparency","0"));

    layout_group = ModelNode->GetAttribute("LayoutGroup","Unassigned");

    ModelStartChannel = ModelNode->GetAttribute("StartChannel");

    // calculate starting channel numbers for each string
    size_t NumberOfStrings= HasOneString(DisplayAs) ? 1 : parm1;
    int ChannelsPerString=CalcCannelsPerString();

    SetStringStartChannels(zeroBased, NumberOfStrings, StartChannel, ChannelsPerString);
    GetModelScreenLocation().Read(ModelNode);

    InitModel();

    size_t NodeCount=GetNodeCount();
    for(size_t i=0; i<NodeCount; i++) {
        Nodes[i]->sparkle = rand() % 10000;
    }

    wxXmlNode *f = ModelNode->GetChildren();
    faceInfo.clear();
    stateInfo.clear();
    wxXmlNode * dimmingCurveNode = nullptr;
    while (f != nullptr) {
        if ("faceInfo" == f->GetName()) {
            ParseFaceInfo(f, faceInfo);
        }
        else if ("stateInfo" == f->GetName()) {
            ParseStateInfo(f, stateInfo);
        }
        else if ("dimmingCurve" == f->GetName()) {
            dimmingCurveNode = f;
            modelDimmingCurve = DimmingCurve::createFromXML(f);
        }
        else if ("subModel" == f->GetName()) {
            ParseSubModel(f);
        }
        f = f->GetNext();
    }

    if (ModelNode->HasAttribute("ModelBrightness") && modelDimmingCurve == nullptr) {
        int b = wxAtoi(ModelNode->GetAttribute("ModelBrightness"));
        if (b != 0) {
            modelDimmingCurve = DimmingCurve::createBrightnessGamma(b, 1.0);
        }
    }
    if (modelDimmingCurve == nullptr && dimmingCurveNode != nullptr) {
        ModelNode->RemoveChild(dimmingCurveNode);
    }
    IncrementChangeCount();
}

void Model::RemoveSubModel(const std::string &name) {
    for (auto a = subModels.begin(); a != subModels.end(); ++a) {
        Model *m = *a;
        if (m->GetName() == name) {
            delete m;
            subModels.erase(a);
        }
    }
}

Model *Model::GetSubModel(const std::string &name) {
    for (auto a = subModels.begin(); a != subModels.end(); ++a) {
        if ((*a)->GetName() == name) {
            return *a;
        }
    }
    return nullptr;
}

void Model::ParseSubModel(wxXmlNode *node) {
    subModels.push_back(new SubModel(this, node));
}
int Model::CalcCannelsPerString() {
    int ChannelsPerString = parm2*GetNodeChannelCount(StringType);
    if (SingleChannel)
        ChannelsPerString=1;
    else if (SingleNode)
        ChannelsPerString=GetNodeChannelCount(StringType);

    return ChannelsPerString;
}

void Model::SetStringStartChannels(bool zeroBased, int NumberOfStrings, int StartChannel, int ChannelsPerString) {
    std::string tempstr = ModelXml->GetAttribute("Advanced", "0").ToStdString();
    bool HasIndividualStartChans=tempstr == "1";
    stringStartChan.clear();
    stringStartChan.resize(NumberOfStrings);
    for (int i=0; i<NumberOfStrings; i++) {
        tempstr = StartChanAttrName(i);
        if (!zeroBased && HasIndividualStartChans && ModelXml->HasAttribute(tempstr)) {
            bool b = false;
            std::string dependsonmodel;
            stringStartChan[i] = GetNumberFromChannelString(ModelXml->GetAttribute(tempstr, "1").ToStdString(), b, dependsonmodel)-1;
            CouldComputeStartChannel &= b;
        } else {
            stringStartChan[i] = (zeroBased? 0 : StartChannel-1) + i*ChannelsPerString;
        }
    }
}

void Model::InitModel() {
}

void Model::GetNodeChannelValues(size_t nodenum, unsigned char *buf) {
    Nodes[nodenum]->GetForChannels(buf);
}

void Model::SetNodeChannelValues(size_t nodenum, const unsigned char *buf) {
    Nodes[nodenum]->SetFromChannels(buf);
}

xlColor Model::GetNodeColor(size_t nodenum) const {
    xlColor color;
    Nodes[nodenum]->GetColor(color);
    return color;
}

xlColor Model::GetNodeMaskColor(size_t nodenum) const {
    if (nodenum >= Nodes.size()) return xlWHITE; // this shouldnt happen but it does if you have a custom model with no nodes in it
    xlColor color;
    Nodes[nodenum]->GetMaskColor(color);
    return color;
}

void Model::SetNodeColor(size_t nodenum, const xlColor &c) {
    Nodes[nodenum]->SetColor(c);
}

bool Model::IsNodeInBufferRange(size_t nodeNum, int x1, int y1, int x2, int y2) {
    if (nodeNum < Nodes.size()) {
        for (auto a = Nodes[nodeNum]->Coords.begin(); a != Nodes[nodeNum]->Coords.end(); ++a) {
            if (a->bufX >= x1 && a->bufX <= x2
                && a->bufY >= y1 && a->bufY <= y2) {
                return true;
            }
        }
    }
    return false;
}

// only valid for rgb nodes and dumb strings (not traditional strings)
wxChar Model::GetChannelColorLetter(wxByte chidx) {
    return rgbOrder[chidx];
}

int CountChar(const std::string& s, char c)
{
    int count = 0;

    for (int i = 0; i < s.size(); ++i)
    {
        if (s[i] == c) count++;
    }

    return count;
}

std::string Model::GetStartChannelInDisplayFormat()
{
    if (ModelStartChannel == "")
    {
        return "(1)";
    }
    else if (ModelStartChannel[0] == '#' || ModelStartChannel[0] == '>' || ModelStartChannel[0] == '@' || CountChar(ModelStartChannel, ':') > 0)
    {
        return wxString::Format("%s (%u)", ModelStartChannel, GetNumberFromChannelString(ModelStartChannel)).ToStdString();
    }
    else
    {
        return ModelStartChannel;
    }
}

std::string Model::GetLastChannelInStartChannelFormat(OutputManager* outputManager)
{
    std::string modelFormat = ModelStartChannel;
    char firstChar = ModelStartChannel[0];

    unsigned int lastChannel = GetLastChannel()+1;

    if (firstChar == '@' || firstChar == '>' && CountChar(ModelStartChannel, ':') == 1)
    {
        std::string referencedModel = ModelStartChannel.substr(1, ModelStartChannel.find(':') - 1);
        Model *m = modelManager[referencedModel];

        if (m != nullptr)
        {
            std::string end = m->GetLastChannelInStartChannelFormat(outputManager);
            if (end != "")
            {
                if (end[0] == '#')
                {
                    firstChar = '#';
                    modelFormat = end;
                }
                else if (CountChar(end, ':') == 1)
                {
                    firstChar = '0';
                    modelFormat = end;
                }
            }
        }
    }

    if (firstChar == '#')
    {
        // universe:channel
        long startChannel;
        Output* output = outputManager->GetOutput(lastChannel, startChannel);

        if (output == nullptr) {
            return wxString::Format("%u", lastChannel).ToStdString();
        }

        if (output->IsOutputCollection())
        {
            output = output->GetActualOutput(lastChannel);
            startChannel = lastChannel - output->GetStartChannel() + 1;
        }

        if (CountChar(modelFormat, ':') == 1)
        {
            return wxString::Format("#%d:%ld (%u)", output->GetUniverse(), startChannel, lastChannel).ToStdString();
        }
        else
        {
            std::string ip = "<err>";
            if (output->IsIpOutput())
            {
                ip = ((IPOutput*)output)->GetIP();
            }
            return wxString::Format("#%s:%d:%ld (%u)", ip, output->GetUniverse(), startChannel, lastChannel).ToStdString();
        }
    }
    else if (firstChar == '@' || firstChar == '>' || CountChar(modelFormat, ':') == 0)
    {
        // absolute
        return wxString::Format("%u", lastChannel).ToStdString();
    }
    else
    {
        // output:channel
        long startChannel;
        Output* output = outputManager->GetOutput(lastChannel, startChannel);

        if (output == nullptr)
        {
            return wxString::Format("%u", lastChannel).ToStdString();
        }
        else
        {
            return wxString::Format("%d:%ld (%u)", output->GetOutputNumber(), startChannel, lastChannel).ToStdString();
        }
    }
}

unsigned int Model::GetLastChannel() {
    unsigned int LastChan=0;
    size_t NodeCount=GetNodeCount();
    for(size_t idx=0; idx<NodeCount; idx++) {
        if (Nodes[idx]->ActChan == (unsigned int)-1)
        {
            return (unsigned int)NodeCount * Nodes[idx]->GetChanCount() - 1;
        }
        LastChan=std::max(LastChan,Nodes[idx]->ActChan + Nodes[idx]->GetChanCount() - 1);
    }
    return LastChan;
}
unsigned int Model::GetFirstChannel() {
    unsigned int LastChan=-1;
    size_t NodeCount=GetNodeCount();
    for(size_t idx=0; idx<NodeCount; idx++) {
        LastChan=std::min(LastChan,Nodes[idx]->ActChan);
    }
    return LastChan;
}

unsigned int Model::GetNumChannels() {
    return GetLastChannel() - GetFirstChannel() + 1;
}

void Model::SetOffset(double xPct, double yPct) {

    if (GetModelScreenLocation().IsLocked()) return;

    GetModelScreenLocation().SetOffset(xPct, yPct);
    IncrementChangeCount();
}

void Model::Lock(bool lock)
{
    GetModelScreenLocation().Lock(lock);
    GetModelXml()->DeleteAttribute("Locked");
    if (lock)
    {
        GetModelXml()->AddAttribute("Locked", "1");
    }
    IncrementChangeCount();
}

void Model::AddOffset(double xPct, double yPct) {

    if (GetModelScreenLocation().IsLocked()) return;

    GetModelScreenLocation().AddOffset(xPct, yPct);
    IncrementChangeCount();
}

// initialize screen coordinates
// parm1=Number of Strings/Arches/Canes
// parm2=Pixels Per String/Arch/Cane
void Model::SetLineCoord() {
    float x,y;
    float idx=0;
    size_t NodeCount=GetNodeCount();
    int numlights=parm1*parm2;
    float half=numlights/2;
    GetModelScreenLocation().SetRenderSize(numlights, numlights*2);

    for(size_t n=0; n<NodeCount; n++) {
        size_t CoordCount=GetCoordCount(n);
        for(size_t c=0; c < CoordCount; c++) {
            x=idx;
            x=IsLtoR ? x - half : half - x;
            y=0;
            Nodes[n]->Coords[c].screenX=x;
            Nodes[n]->Coords[c].screenY=y + numlights;
            idx++;
        }
    }
}

void Model::SetBufferSize(int NewHt, int NewWi) {
    BufferHt = NewHt;
    BufferWi = NewWi;
    IncrementChangeCount();
}

// not valid for Frame or Custom
int Model::NodesPerString() {
    return SingleNode ? 1 : parm2;
}

int Model::NodeStartChannel(size_t nodenum) const {
    return Nodes.size() && nodenum < Nodes.size() ? Nodes[nodenum]->ActChan: 0; //avoid memory access error if no nods -DJ
}

const std::string &Model::NodeType(size_t nodenum) const {
    return Nodes.size() && nodenum < Nodes.size() ? Nodes[nodenum]->GetNodeType(): NodeBaseClass::RGB; //avoid memory access error if no nods -DJ
}

NodeBaseClass* Model::createNode(int ns, const std::string &StringType, size_t NodesPerString, const std::string &rgbOrder) {
    NodeBaseClass *ret = nullptr;
    if (StringType=="Single Color Red" || StringType == "R") {
        ret = new NodeClassRed(ns, NodesPerString);
    } else if (StringType=="Single Color Green" || StringType == "G") {
        ret = new NodeClassGreen(ns,NodesPerString);
    } else if (StringType=="Single Color Blue" || StringType == "B") {
        ret = new NodeClassBlue(ns,NodesPerString);
    } else if (StringType=="Single Color White" || StringType == "W") {
        ret = new NodeClassWhite(ns,NodesPerString);
    } else if (StringType[0] == '#') {
        ret = new NodeClassCustom(ns,NodesPerString, xlColor(StringType));
    } else if (StringType=="Strobes White 3fps" || StringType=="Strobes") {
        ret = new NodeClassWhite(ns,NodesPerString);
    } else if (StringType=="4 Channel RGBW" || StringType == "RGBW") {
        ret = new NodeClassRGBW(ns,NodesPerString);
    } else {
        ret = new NodeBaseClass(ns,1,rgbOrder);
    }
    ret->model = this;
    return ret;
}

void Model::GetBufferSize(const std::string &type, const std::string &transform, int &bufferWi, int &bufferHi) const {
    if (type == DEFAULT) {
        bufferHi = this->BufferHt;
        bufferWi = this->BufferWi;
    } else if (type == SINGLE_LINE) {
        bufferHi = 1;
        bufferWi = Nodes.size();
    } else if (type == AS_PIXEL) {
        bufferHi = 1;
        bufferWi = 1;
    } else if (type == VERT_PER_STRAND) {
        bufferHi = GetNumStrands();
        bufferWi = 1;
        for (int x = 0; x < bufferHi; x++) {
            bufferWi = std::max(bufferWi, GetStrandLength(x));
        }
    } else if (type == HORIZ_PER_STRAND) {
        bufferWi = GetNumStrands();
        bufferHi = 1;
        for (int x = 0; x < bufferWi; x++) {
            bufferHi = std::max(bufferHi, GetStrandLength(x));
        }
    } else {
        //if (type == PER_PREVIEW) {
        //default is to go ahead and build the full node buffer
        std::vector<NodeBaseClassPtr> newNodes;
        InitRenderBufferNodes(type, "None", newNodes, bufferWi, bufferHi);
    }
    AdjustForTransform(transform, bufferWi, bufferHi);
}

void Model::AdjustForTransform(const std::string &transform,
                               int &bufferWi, int &bufferHi) const {
    if (transform == "Rotate CC 90" || transform == "Rotate CW 90") {
        int x = bufferHi;
        bufferHi = bufferWi;
        bufferWi = x;
    }
}

static inline void SetCoords(NodeBaseClass::CoordStruct &it2, int x, int y) {
    it2.bufX = x;
    it2.bufY = y;
}

static inline void SetCoords(NodeBaseClass::CoordStruct &it2, int x, int y, int maxX, int maxY, int scale) {
    if (maxX != -1) {
        x = x * maxX;
        x = x / scale;
    }
    if (maxY != -1) {
        y = y * maxY;
        y = y / scale;
    }
    it2.bufX = x;
    it2.bufY = y;
}

void Model::ApplyTransform(const std::string &type,
                    std::vector<NodeBaseClassPtr> &newNodes,
                    int &bufferWi, int &bufferHi) const {
    //"Rotate CC 90", "Rotate CW 90", "Rotate 180", "Flip Vertical", "Flip Horizontal",
    if (type == "None") {
        return;
    } else if (type == "Rotate 180") {
        for (size_t x = 0; x < newNodes.size(); x++) {
            for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                SetCoords(*it2, bufferWi - it2->bufX - 1, bufferHi - it2->bufY - 1);
            }
        }
    } else if (type == "Flip Vertical") {
        for (size_t x = 0; x < newNodes.size(); x++) {
            for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                SetCoords(*it2, it2->bufX, bufferHi - it2->bufY - 1);
            }
        }
    } else if (type == "Flip Horizontal") {
        for (size_t x = 0; x < newNodes.size(); x++) {
            for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                SetCoords(*it2, bufferWi - it2->bufX - 1, it2->bufY);
            }
        }
    } else if (type == "Rotate CW 90") {
        for (size_t x = 0; x < newNodes.size(); x++) {
            for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                SetCoords(*it2, bufferHi - it2->bufY - 1, it2->bufX);
            }
        }
        int tmp = bufferHi;
        bufferHi = bufferWi;
        bufferWi = tmp;
    } else if (type == "Rotate CC 90") {
        for (int x = 0; x < newNodes.size(); x++) {
            for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                SetCoords(*it2, it2->bufY, bufferWi - it2->bufX - 1);
            }
        }
        int tmp = bufferHi;
        bufferHi = bufferWi;
        bufferWi = tmp;
    }
}

void Model::InitRenderBufferNodes(const std::string &type,
                                  const std::string &transform,
                                  std::vector<NodeBaseClassPtr> &newNodes, int &bufferWi, int &bufferHt) const {
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    int firstNode = newNodes.size();

    // want to see if i can catch something that causes this to crash
    if (firstNode + Nodes.size() <= 0 || firstNode + Nodes.size() > 50000)
    {
        logger_base.warn("XXX Model::InitRenderBufferNodes firstNode + Nodes.size() = %ld.", (long)firstNode + Nodes.size());
    }

    newNodes.reserve(firstNode + Nodes.size());
    for (auto it = Nodes.begin(); it != Nodes.end(); ++it) {
        newNodes.push_back(NodeBaseClassPtr(it->get()->clone()));
    }
    if (type == DEFAULT) {
        bufferHt = this->BufferHt;
        bufferWi = this->BufferWi;
    } else if (type == SINGLE_LINE) {
        bufferHt = 1;
        bufferWi = newNodes.size();
        int cnt = 0;
        for (int x = firstNode; x < newNodes.size(); x++) {
            if (newNodes[x] == nullptr)
            {
                logger_base.crit("XXX Model::InitRenderBufferNodes newNodes[x] is null ... this is going to crash.");
            }
            for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                SetCoords(*it2, cnt, 0);
            }
            cnt++;
        }
    } else if (type == AS_PIXEL) {
        bufferHt = 1;
        bufferWi = 1;
        for (int x = firstNode; x < newNodes.size(); x++) {
            if (newNodes[x] == nullptr)
            {
                logger_base.crit("XXX Model::InitRenderBufferNodes newNodes[x] is null ... this is going to crash.");
            }
            for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                SetCoords(*it2, 0, 0);
            }
        }
    } else if (type == HORIZ_PER_STRAND) {
        bufferWi = GetNumStrands();
        bufferHt = 1;
        for (int x = 0; x < bufferWi; x++) {
            bufferHt = std::max(bufferHt, GetStrandLength(x));
        }
        int cnt = 0;
        int strand = 0;
        int strandLen = GetStrandLength(0);
        for (int x = firstNode; x < newNodes.size();) {
            if (cnt >= strandLen) {
                strand++;
                strandLen = GetStrandLength(strand);
                cnt = 0;
            } else {
                if (newNodes[x] == nullptr)
                {
                    logger_base.crit("AAA Model::InitRenderBufferNodes newNodes[x] is null ... this is going to crash.");
                }
                for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                    SetCoords(*it2, strand, cnt, -1, bufferHt, strandLen);
                }
                cnt++;
                x++;
            }
        }
    } else if (type == VERT_PER_STRAND) {
        bufferHt = GetNumStrands();
        bufferWi = 1;
        for (int x = 0; x < bufferHt; x++) {
            bufferWi = std::max(bufferWi, GetStrandLength(x));
        }
        int cnt = 0;
        int strand = 0;
        int strandLen = GetStrandLength(0);
        for (int x = firstNode; x < newNodes.size();) {
            if (cnt >= strandLen) {
                strand++;
                strandLen = GetStrandLength(strand);
                cnt = 0;
            } else {
                if (newNodes[x] == nullptr)
                {
                    logger_base.crit("BBB Model::InitRenderBufferNodes newNodes[x] is null ... this is going to crash.");
                }
                for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                    SetCoords(*it2, cnt, strand, bufferWi, -1, strandLen);
                }
                cnt++;
                x++;
            }
        }
    } else if (type == PER_PREVIEW || type == PER_PREVIEW_NO_OFFSET) {
        float maxX = -1000000;
        float minX = 1000000;
        float maxY = -1000000;
        float minY = 1000000;
        float sx,sy;
        GetModelScreenLocation().PrepareToDraw();

        for (int x = firstNode; x < newNodes.size(); x++) {
            if (newNodes[x] == nullptr)
            {
                logger_base.crit("CCC Model::InitRenderBufferNodes newNodes[x] is null ... this is going to crash.");
            }
            for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                sx = it2->screenX;
                sy = it2->screenY;

                GetModelScreenLocation().TranslatePoint(sx, sy);

                if (sx > maxX) {
                    maxX = sx;
                }
                if (sx < minX) {
                    minX = sx;
                }
                if (sy > maxY) {
                    maxY = sy;
                }
                if (sy < minY) {
                    minY = sy;
                }
            }
        }
        int offx = minX;
        int offy = minY;
        bool noOff = type == PER_PREVIEW_NO_OFFSET;
        if (noOff) {
            offx = 0;
            offy = 0;
        }
        bufferHt = bufferWi = -1;
        for (int x = firstNode; x < newNodes.size(); x++) {
            if (newNodes[x] == nullptr)
            {
                logger_base.crit("DDD Model::InitRenderBufferNodes newNodes[x] is null ... this is going to crash.");
            }
            for (auto it2 = newNodes[x]->Coords.begin(); it2 != newNodes[x]->Coords.end(); ++it2) {
                sx = it2->screenX;
                sy = it2->screenY;

                GetModelScreenLocation().TranslatePoint(sx, sy);

                SetCoords(*it2, std::round(sx - offx), std::round(sy - offy));
                if (it2->bufX > bufferWi) {
                    bufferWi = it2->bufX;
                }
                if (it2->bufY > bufferHt) {
                    bufferHt = it2->bufY;
                }

                if (noOff) {
                    it2->screenX = sx;
                    it2->screenY = sy;
                }
            }
        }
        if (!noOff) {
            bufferHt++;
            bufferWi++;
        } else {
            bufferHt = std::round(maxY - minY + 1.0f);
            bufferWi = std::round(maxX - minX + 1.0f);
        }
    } else {
        bufferHt = this->BufferHt;
        bufferWi = this->BufferWi;
    }
    ApplyTransform(transform, newNodes, bufferWi, bufferHt);

    //wxASSERT(bufferWi != 0);
    //wxASSERT(bufferHt != 0);
}

std::string Model::GetNextName() {
    if (nodeNames.size() > Nodes.size()) {
        return nodeNames[Nodes.size()];
    }
    return "";
}
// set size of Nodes vector and each Node's Coords vector
void Model::SetNodeCount(size_t NumStrings, size_t NodesPerString, const std::string &rgbOrder) {
    size_t n;
    if (SingleNode) {
        if (StringType=="Single Color Red") {
            for(n=0; n<NumStrings; n++) {
                Nodes.push_back(NodeBaseClassPtr(new NodeClassRed(n,NodesPerString, GetNextName())));
                Nodes.back()->model = this;
            }
        } else if (StringType=="Single Color Green") {
            for(n=0; n<NumStrings; n++) {
                Nodes.push_back(NodeBaseClassPtr(new NodeClassGreen(n,NodesPerString, GetNextName())));
                Nodes.back()->model = this;
            }
        } else if (StringType=="Single Color Blue") {
            for(n=0; n<NumStrings; n++) {
                Nodes.push_back(NodeBaseClassPtr(new NodeClassBlue(n,NodesPerString, GetNextName())));
                Nodes.back()->model = this;
            }
        } else if (StringType=="Single Color White") {
            for(n=0; n<NumStrings; n++) {
                Nodes.push_back(NodeBaseClassPtr(new NodeClassWhite(n,NodesPerString, GetNextName())));
                Nodes.back()->model = this;
            }
        } else if (StringType=="Strobes White 3fps" || StringType=="Strobes") {
            StrobeRate=7;  // 1 out of every 7 frames
            for(n=0; n<NumStrings; n++) {
                Nodes.push_back(NodeBaseClassPtr(new NodeClassWhite(n,NodesPerString, GetNextName())));
                Nodes.back()->model = this;
            }
        } else if (StringType=="Single Color Custom") {
            for(n=0; n<NumStrings; n++) {
                Nodes.push_back(NodeBaseClassPtr(new NodeClassCustom(n,NodesPerString, customColor, GetNextName())));
                Nodes.back()->model = this;
            }
        } else if (StringType=="Single Color Intensity") {
            for(n=0; n<NumStrings; n++) {
                Nodes.push_back(NodeBaseClassPtr(new NodeClassIntensity(n,NodesPerString, customColor, GetNextName())));
                Nodes.back()->model = this;
            }
        } else if (StringType=="4 Channel RGBW") {
            for(n=0; n<NumStrings; n++) {
                Nodes.push_back(NodeBaseClassPtr(new NodeClassRGBW(n,NodesPerString, GetNextName())));
                Nodes.back()->model = this;
            }
        } else {
            // 3 Channel RGB
            for(n=0; n<NumStrings; n++) {
                Nodes.push_back(NodeBaseClassPtr(new NodeBaseClass(n,NodesPerString, "RGB", GetNextName())));
                Nodes.back()->model = this;
            }
        }
    } else if (NodesPerString == 0) {
        Nodes.push_back(NodeBaseClassPtr(new NodeBaseClass(0, 0, rgbOrder, GetNextName())));
        Nodes.back()->model = this;
    } else {
        size_t numnodes=NumStrings*NodesPerString;
        for(n=0; n<numnodes; n++) {
            Nodes.push_back(NodeBaseClassPtr(new NodeBaseClass(n/NodesPerString, 1, rgbOrder, GetNextName())));
            Nodes.back()->model = this;
        }
    }
}

int Model::GetNodeChannelCount(const std::string & nodeType) {
    if (nodeType.compare(0, 12, "Single Color") == 0) {
        return 1;
    } else if (nodeType == "Strobes White 3fps") {
        return 1;
    } else if (nodeType == "Strobes") {
        return 1;
    } else if (nodeType == "4 Channel RGBW") {
        return 4;
    }
    return 3;
}

// returns a number where the first node is 1
int Model::GetNodeNumber(size_t nodenum) {
    if (nodenum >= Nodes.size()) return 0;
    //if (Nodes[nodenum].bufX < 0) return 0;
    int sn=Nodes[nodenum]->StringNum;
    return (Nodes[nodenum]->ActChan - stringStartChan[sn]) / 3 + sn*NodesPerString() + 1;
}

size_t Model::GetNodeCount() const {
    return Nodes.size();
}

size_t Model::GetActChanCount() const
{
    size_t NodeCnt = GetNodeCount();
    if (NodeCnt == 0) {
        return 0;
    }

    size_t count = 0;
    for (int x = 0; x < NodeCnt; x++) {
        count += Nodes[x]->GetChanCount();
    }

    return count;
}

int Model::GetChanCount() const {
    size_t NodeCnt=GetNodeCount();
    if (NodeCnt == 0) {
        return 0;
    }
    int min = 999999999;
    int max = 0;

    for (int x = 0; x < NodeCnt; x++) {
        int i = Nodes[x]->ActChan;
        if (i < min) {
            min = i;
        }
        i += Nodes[x]->GetChanCount();
        if (i > max) {
            max = i;
        }
    }
    return max - min;
}

int Model::GetChanCountPerNode() const {
    size_t NodeCnt = GetNodeCount();
    if (NodeCnt == 0) {
        return 0;
    }
    return Nodes[0]->GetChanCount();
}

size_t Model::GetCoordCount(size_t nodenum) const {
    return nodenum < Nodes.size() ? Nodes[nodenum]->Coords.size() : 0;
}
int Model::GetNodeStringNumber(size_t nodenum) const {
    return nodenum < Nodes.size() ? Nodes[nodenum]->StringNum : 0;
}

void Model::GetNodeCoords(int nodeidx, std::vector<wxPoint> &pts) {
    for (int x = 0; x < Nodes[nodeidx]->Coords.size(); x++) {
        pts.push_back(wxPoint(Nodes[nodeidx]->Coords[x].bufX, Nodes[nodeidx]->Coords[x].bufY));
    }
}

bool Model::IsCustom(void) {
    return (DisplayAs == "Custom");
}

//convert # to AA format so it matches Custom Model grid display:
//this makes it *so* much easier to visually compare with Custom Model grid display
//A - Z == 1 - 26
//AA - AZ == 27 - 52
//BA - BZ == 53 - 78
//etc
static wxString AA(int x) {
    wxString retval;
    --x;
    //    if (x >= 26 * 26) { retval += 'A' + x / (26 * 26); x %= 26 * 26; }
    if (x >= 26) {
        retval += 'A' + x / 26 - 1;
        x %= 26;
    }
    retval += 'A' + x;
    return retval;
}

//add just the node#s to a choice list:
//NO add parsed info to choice list or check list box:
size_t Model::GetChannelCoords(wxArrayString& choices) { //wxChoice* choices1, wxCheckListBox* choices2, wxListBox* choices3)
    //    if (choices1) choices1->Clear();
    //    if (choices2) choices2->Clear();
    //    if (choices3) choices3->Clear();
    //    if (choices1) choices1->Append(wxT("0: (none)"));
    //    if (choices2) choices2->Append(wxT("0: (none)"));
    //    if (choices3) choices3->Append(wxT("0: (none)"));
    size_t NodeCount = GetNodeCount();
    for (size_t n = 0; n < NodeCount; n++) {
        wxString newstr;
        //        debug(10, "model::node[%d/%d]: #coords %d, ach# %d, str %d", n, NodeCount, Nodes[n]->Coords.size(), Nodes[n]->StringNum, Nodes[n]->ActChan);
        if (Nodes[n]->Coords.empty()) continue;
        //        newstr = wxString::Format(wxT("%i"), GetNodeNumber(n));
        //        choices.Add(newstr);
        choices.Add(GetNodeXY(n));
        //        if (choices1) choices1->Append(newstr);
        //        if (choices2) choices2->Append(newstr);
        //        if (choices3)
        //        {
        //            wxArrayString strary;
        //            choices3->InsertItems(strary, choices3->GetCount() + 0);
        //        }
    }
    return choices.GetCount(); //choices1? choices1->GetCount(): 0) + (choices2? choices2->GetCount(): 0);
}

//get parsed node info:
std::string Model::GetNodeXY(const std::string& nodenumstr) {
    long nodenum;
    size_t NodeCount = GetNodeCount();
    try {
        nodenum = std::stod(nodenumstr);
        for (size_t inx = 0; inx < NodeCount; inx++) {
            if (Nodes[inx]->Coords.empty()) continue;
            if (GetNodeNumber(inx) == nodenum) return GetNodeXY(inx);
        }
    } catch ( ... ) {

    }
    return nodenumstr; //not found?
}

std::string Model::GetNodeXY(int nodeinx) {
    if ((nodeinx < 0) || (nodeinx >= GetNodeCount())) return "";
    if (Nodes[nodeinx]->Coords.empty()) return "";
    if (GetCoordCount(nodeinx) > 1) //show count and first + last coordinates
        if (IsCustom())
            return wxString::Format(wxT("%d: %d# @%s%d-%s%d"), GetNodeNumber(nodeinx), GetCoordCount(nodeinx), AA(Nodes[nodeinx]->Coords.front().bufX + 1), BufferHt - Nodes[nodeinx]->Coords.front().bufY, AA(Nodes[nodeinx]->Coords.back().bufX + 1), BufferHt - Nodes[nodeinx]->Coords.back().bufY).ToStdString(); //NOTE: only need first (X,Y) for each channel, but show last and count as well; Y is in reverse order
        else
            return wxString::Format(wxT("%d: %d# @(%d,%d)-(%d,%d"), GetNodeNumber(nodeinx), GetCoordCount(nodeinx), Nodes[nodeinx]->Coords.front().bufX + 1, BufferHt - Nodes[nodeinx]->Coords.front().bufY, Nodes[nodeinx]->Coords.back().bufX + 1, BufferHt - Nodes[nodeinx]->Coords.back().bufY).ToStdString(); //NOTE: only need first (X,Y) for each channel, but show last and count as well; Y is in reverse order
        else //just show singleton
            if (IsCustom())
                return wxString::Format(wxT("%d: @%s%d"), GetNodeNumber(nodeinx), AA(Nodes[nodeinx]->Coords.front().bufX + 1), BufferHt - Nodes[nodeinx]->Coords.front().bufY).ToStdString();
            else
                return wxString::Format(wxT("%d: @(%d,%d)"), GetNodeNumber(nodeinx), Nodes[nodeinx]->Coords.front().bufX + 1, BufferHt - Nodes[nodeinx]->Coords.front().bufY).ToStdString();
}

//extract first (X,Y) from string formatted above:
bool Model::ParseFaceElement(const std::string& multi_str, std::vector<wxPoint>& first_xy) {
    //    first_xy->x = first_xy->y = 0;
    //    first_xy.clear();
    wxStringTokenizer wtkz(multi_str, "+");
    while (wtkz.HasMoreTokens()) {
        wxString str = wtkz.GetNextToken();
        if (str.empty()) continue;
        if (str.Find('@') == wxNOT_FOUND) continue; //return false;

        wxString xystr = str.AfterFirst('@');
        if (xystr.empty()) continue; //return false;
        long xval = 0, yval = 0;
        if (xystr[0] == '(') {
            xystr.Remove(0, 1);
            if (!xystr.BeforeFirst(',').ToLong(&xval)) continue; //return false;
            if (!xystr.AfterFirst(',').BeforeFirst(')').ToLong(&yval)) continue; //return false;
        } else {
            int parts = 0;
            while (!xystr.empty() && (xystr[0] >= 'A') && (xystr[0] <= 'Z')) {
                xval *= 26;
                xval += xystr[0] - 'A' + 1;
                xystr.Remove(0, 1);
                parts |= 1;
            }
            while (!xystr.empty() && (xystr[0] >= '0') && (xystr[0] <= '9')) {
                yval *= 10;
                yval += xystr[0] - '0';
                xystr.Remove(0, 1);
                parts |= 2;
            }
            if (parts != 3) continue; //return false;
            if (!xystr.empty() && (xystr[0] != '-')) continue; //return false;
        }
        wxPoint newxy(xval, yval);
        first_xy.push_back(newxy);
    }

    return !first_xy.empty(); //true;
}

//extract first (X,Y) from string formatted above:
bool Model::ParseStateElement(const std::string& multi_str, std::vector<wxPoint>& first_xy) {
    //    first_xy->x = first_xy->y = 0;
    //    first_xy.clear();
    wxStringTokenizer wtkz(multi_str, "+");
    while (wtkz.HasMoreTokens()) {
        wxString str = wtkz.GetNextToken();
        if (str.empty()) continue;
        if (str.Find('@') == wxNOT_FOUND) continue; //return false;

        wxString xystr = str.AfterFirst('@');
        if (xystr.empty()) continue; //return false;
        long xval = 0, yval = 0;
        if (xystr[0] == '(') {
            xystr.Remove(0, 1);
            if (!xystr.BeforeFirst(',').ToLong(&xval)) continue; //return false;
            if (!xystr.AfterFirst(',').BeforeFirst(')').ToLong(&yval)) continue; //return false;
        }
        else {
            int parts = 0;
            while (!xystr.empty() && (xystr[0] >= 'A') && (xystr[0] <= 'Z')) {
                xval *= 26;
                xval += xystr[0] - 'A' + 1;
                xystr.Remove(0, 1);
                parts |= 1;
            }
            while (!xystr.empty() && (xystr[0] >= '0') && (xystr[0] <= '9')) {
                yval *= 10;
                yval += xystr[0] - '0';
                xystr.Remove(0, 1);
                parts |= 2;
            }
            if (parts != 3) continue; //return false;
            if (!xystr.empty() && (xystr[0] != '-')) continue; //return false;
        }
        wxPoint newxy(xval, yval);
        first_xy.push_back(newxy);
    }

    return !first_xy.empty(); //true;
}

void Model::ExportAsCustomXModel() const {

    wxString name = ModelXml->GetAttribute("name");
    wxLogNull logNo; //kludge: avoid "error 0" message from wxWidgets after new file is written
    wxString filename = wxFileSelector(_("Choose output file"), wxEmptyString, name, wxEmptyString, "Custom Model files (*.xmodel)|*.xmodel", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (filename.IsEmpty()) return;

    wxFile f(filename);
    //    bool isnew = !wxFile::Exists(filename);
    if (!f.Create(filename, true) || !f.IsOpened()) retmsg(wxString::Format("Unable to create file %s. Error %d\n", filename, f.GetLastError()));

    wxString cm = "";

    float minsx = 99999;
    float minsy = 99999;
    float maxsx = -1;
    float maxsy = -1;

    size_t nodeCount = GetNodeCount();
    for (size_t i = 0; i < nodeCount; i++) {
        float Sbufx = Nodes[i]->Coords[0].screenX;
        float Sbufy = Nodes[i]->Coords[0].screenY;
        if (Sbufx < minsx) minsx = Sbufx;
        if (Sbufx > maxsx) maxsx = Sbufx;
        if (Sbufy < minsy) minsy = Sbufy;
        if (Sbufy > maxsy) maxsy = Sbufy;
    }

    int minx = std::floor(minsx);
    int miny = std::floor(minsy);
    int maxx = std::ceil(maxsx);
    int maxy = std::ceil(maxsy);
    int sizex = maxx - minx + 1;
    int sizey = maxy - miny + 1;

    int* nodeLayout = (int*)malloc(sizey * sizex * sizeof(int));
    memset(nodeLayout, 0x00, sizey * sizex * sizeof(int));

    for (int i = 0; i < nodeCount; i++)
    {
        int x = Nodes[i]->Coords[0].screenX - minx;
        int y = sizey - (Nodes[i]->Coords[0].screenY - miny) - 1;
        nodeLayout[y*sizex + x] = i+1;
    }

    for (int i = 0; i < sizey * sizex; i++)
    {
        if (i != 0)
        {
            if (i % sizex == 0)
            {
                cm += ";";
            }
            else
            {
                cm += ",";
            }
        }
        if (nodeLayout[i] != 0)
        {
            cm += wxString::Format("%i", nodeLayout[i]);
        }
    }

    free(nodeLayout);

    wxString p1 = wxString::Format("%i", sizex);
    wxString p2 = wxString::Format("%i", sizey);
    wxString st = ModelXml->GetAttribute("StringType");
    wxString ps = ModelXml->GetAttribute("PixelSize");
    wxString t = ModelXml->GetAttribute("Transparency");
    wxString mb = ModelXml->GetAttribute("ModelBrightness");
    wxString a = ModelXml->GetAttribute("Antialias");
    wxString sn = ModelXml->GetAttribute("StrandNames");
    wxString nn = ModelXml->GetAttribute("NodeNames");
    wxString v = xlights_version_string;
    f.Write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<custommodel \n");
    f.Write(wxString::Format("name=\"%s\" ", name));
    f.Write(wxString::Format("parm1=\"%s\" ", p1));
    f.Write(wxString::Format("parm2=\"%s\" ", p2));
    f.Write(wxString::Format("StringType=\"%s\" ", st));
    f.Write(wxString::Format("Transparency=\"%s\" ", t));
    f.Write(wxString::Format("PixelSize=\"%s\" ", ps));
    f.Write(wxString::Format("ModelBrightness=\"%s\" ", mb));
    f.Write(wxString::Format("Antialias=\"%s\" ", a));
    f.Write(wxString::Format("StrandNames=\"%s\" ", sn));
    f.Write(wxString::Format("NodeNames=\"%s\" ", nn));
    f.Write("CustomModel=\"");
    f.Write(cm);
    f.Write("\" ");
    f.Write(wxString::Format("SourceVersion=\"%s\" ", v));
    f.Write(" >\n");
    wxString face = SerialiseFace();
    if (face != "")
    {
        f.Write(face);
    }
    wxString state = SerialiseState();
    if (state != "")
    {
        f.Write(state);
    }
    wxString submodel = SerialiseSubmodel();
    if (submodel != "")
    {
        f.Write(submodel);
    }
    f.Write("</custommodel>");
    f.Close();
}

std::string Model::ChannelLayoutHtml(OutputManager* outputManager) {
    size_t NodeCount=GetNodeCount();
    size_t i;
    int n, s;
    wxString bgcolor;
    std::vector<int> chmap;
    chmap.resize(BufferHt * BufferWi,0);
    bool IsCustom = DisplayAs == "Custom";
    std::string direction;
    if (IsCustom) {
        direction="n/a";
    } else if (!IsLtoR) {
        if(!isBotToTop)
            direction="Top Right";
        else
            direction="Bottom Right";
    } else {
        if (!isBotToTop)
            direction="Top Left";
        else
            direction="Bottom Left";
    }

    long sc;
    Output* o = outputManager->GetOutput(this->GetFirstChannel(), sc);

    std::string html = "<html><body><table border=0>";
    html+="<tr><td>Name:</td><td>"+name+"</td></tr>";
    html+="<tr><td>Display As:</td><td>"+DisplayAs+"</td></tr>";
    html+="<tr><td>String Type:</td><td>"+StringType+"</td></tr>";
    html+="<tr><td>Start Corner:</td><td>"+direction+"</td></tr>";
    html+=wxString::Format("<tr><td>Total nodes:</td><td>%d</td></tr>",(int)NodeCount);
    html+=wxString::Format("<tr><td>Height:</td><td>%d</td></tr>",BufferHt);
    if (o != nullptr)
        html += wxString::Format("<tr><td>Controller:</td><td>%s:%s</td></tr>", o->GetCommPort(), o->GetDescription());
    if (wxString(controller_connection).Contains(":"))
    {
        wxArrayString cc = wxSplit(controller_connection, ':');
        html += wxString::Format("<tr><td>Pixel protocol:</td><td>%s</td></tr>", cc[0]);
        if (GetNumStrings() ==  1)
            html += wxString::Format("<tr><td>Controller Connection:</td><td>%s</td></tr>", cc[1]);
        else
            html += wxString::Format("<tr><td>Controller Connections:</td><td>%s-%d</td></tr>", cc[1], wxAtoi(cc[1])+ GetNumPhysicalStrings() -1);
    }
    html+="</table><p>Node numbers starting with 1 followed by string number:</p><table border=1>";

    if (BufferHt == 1) {
        // single line or arch or cane
        html+="<tr>";
        for(i=1; i<=NodeCount; i++) {
            n=IsLtoR ? i : NodeCount-i+1;
            s=Nodes[n-1]->StringNum+1;
            bgcolor=s%2 == 1 ? "#ADD8E6" : "#90EE90";
            html += wxString::Format("<td bgcolor='"+bgcolor+"'>n%ds%d</td>",n,s);
        }
        html+="</tr>";
    } else if (BufferHt > 1) {
        // horizontal or vertical matrix or frame
        for(i=0; i<NodeCount; i++) {
            size_t idx = Nodes[i]->Coords[0].bufY * BufferWi + Nodes[i]->Coords[0].bufX;
            if (idx < chmap.size()) chmap[idx]=i + 1;
        }
        for(int y = BufferHt-1; y>=0; y--) {
            html+="<tr>";
            for(int x = 0; x<BufferWi; x++) {
                n=chmap[y*BufferWi+x];
                if (n==0) {
                    html+="<td></td>";
                } else {
                    s=Nodes[n-1]->StringNum+1;
                    bgcolor=s%2 == 1 ? "#ADD8E6" : "#90EE90";
                    html+=wxString::Format("<td bgcolor='"+bgcolor+"'>n%ds%d</td>",n,s);
                }
            }
            html+="</tr>";
        }
    } else {
        html+="<tr><td>Error - invalid height</td></tr>";
    }
    html+="</table></body></html>";
    return html;
}

// initialize screen coordinates
void Model::CopyBufCoord2ScreenCoord() {
    size_t NodeCount=GetNodeCount();
    int xoffset=BufferWi/2;
    int yoffset=BufferHt/2;
    for(size_t n=0; n<NodeCount; n++) {
        size_t CoordCount=GetCoordCount(n);
        for(size_t c=0; c < CoordCount; c++) {
            Nodes[n]->Coords[c].screenX = Nodes[n]->Coords[c].bufX - xoffset;
            Nodes[n]->Coords[c].screenY = Nodes[n]->Coords[c].bufY - yoffset;
        }
    }
    GetModelScreenLocation().SetRenderSize(BufferWi,BufferHt);
}

void Model::UpdateXmlWithScale() {
    GetModelScreenLocation().Write(ModelXml);
    ModelXml->DeleteAttribute("StartChannel");
    if (ModelXml->HasAttribute("versionNumber"))
        ModelXml->DeleteAttribute("versionNumber");
    ModelXml->AddAttribute("versionNumber", "1");
    ModelXml->AddAttribute("StartChannel", ModelStartChannel);
}

bool Model::IsContained(ModelPreview* preview, int x1, int y1, int x2, int y2) {
    SetMinMaxModelScreenCoordinates(preview);
    return GetModelScreenLocation().IsContained(x1, y1, x2, y2);
}


bool Model::HitTest(ModelPreview* preview,int x,int y) {
    int y1 = preview->GetVirtualCanvasHeight()-y;
    SetMinMaxModelScreenCoordinates(preview);
    return GetModelScreenLocation().HitTest(x, y1);
}


wxCursor Model::CheckIfOverHandles(int &handle, wxCoord x,wxCoord y) {
    return GetModelScreenLocation().CheckIfOverHandles(handle, x, y);
}
wxCursor Model::InitializeLocation(int &handle, wxCoord x,wxCoord y) {
    return GetModelScreenLocation().InitializeLocation(handle, x, y, Nodes);
}

void Model::ApplyTransparency(xlColor &color, int transparency) {
    if (transparency) {
        float t = 100.0f - transparency;
        t *= 2.55f;
        transparency = t;
        color.alpha = transparency > 255 ? 255 : (transparency < 0 ? 0 : transparency);
    }
}

// display model using colors stored in each node
// used when preview is running
void Model::DisplayModelOnWindow(ModelPreview* preview, DrawGLUtils::xlAccumulator &va, const xlColor *c, bool allowSelected) {
    size_t NodeCount=Nodes.size();
    xlColor color;
    if (c != nullptr) {
        color = *c;
    }
    int w, h;
    preview->GetVirtualCanvasSize(w, h);

	 ModelScreenLocation& screenLocation = GetModelScreenLocation();
	 screenLocation.SetPreviewSize(w, h, std::vector<NodeBaseClassPtr>());

    screenLocation.PrepareToDraw();

    int vcount = 0;
    for (auto it = Nodes.begin(); it != Nodes.end(); ++it) {
        vcount += it->get()->Coords.size();
    }
    if (pixelStyle > 1) {
        int f = pixelSize;
        if (pixelSize < 16) {
            f = 16;
        }
        vcount = vcount * f * 3;
    }
    if (vcount > maxVertexCount) {
        maxVertexCount = vcount;
    }
    va.PreAlloc(maxVertexCount);

    int first = 0; int last = NodeCount;
    int buffFirst = -1; int buffLast = -1;
    bool left = true;

    while (first < last) {
        int n = 0;
        if (left) {
            n = first;
            first++;
            if (NodeRenderOrder() == 1) {
                if (buffFirst == -1) {
                    buffFirst = Nodes[n]->Coords[0].bufX;
                }
                if (first < NodeCount && buffFirst != Nodes[first]->Coords[0].bufX) {
                    left = false;
                }
            }
        } else {
            last--;
            n = last;
            if (buffLast == -1) {
                buffLast = Nodes[n]->Coords[0].bufX;
            }
            if (last > 0 && buffFirst != Nodes[last - 1]->Coords[0].bufX) {
                left = true;
            }
        }
        if (c == nullptr) {
            Nodes[n]->GetColor(color);
            if (Nodes[n]->model->modelDimmingCurve != nullptr) {
                Nodes[n]->model->modelDimmingCurve->reverse(color);
            }
            if (Nodes[n]->model->StrobeRate) {
                int r = rand() % 5;
                if (r != 0) {
                    color = xlBLACK;
                }
            }
        }
        size_t CoordCount=GetCoordCount(n);
        for(size_t c2=0; c2 < CoordCount; c2++) {
            // draw node on screen
            float sx = Nodes[n]->Coords[c2].screenX;;
            float sy = Nodes[n]->Coords[c2].screenY;
            GetModelScreenLocation().TranslatePoint(sx, sy);

            if (pixelStyle < 2) {
                if (splitRGB) {
                    if ((color.Red() == color.Blue()) && (color.Blue() == color.Green())) {
                        xlColor c3(color);
                        ApplyTransparency(c3, color == xlBLACK ? blackTransparency : transparency);
                        va.AddVertex(sx, sy, c3);
                    } else {
                        xlColor c3(color.Red(), 0 , 0);
                        if (c3 != xlBLACK) {
                            ApplyTransparency(c3, transparency);
                            va.AddVertex(sx-pixelSize, sy+pixelSize/2.0f, c3);
                        }
                        c3.Set(0, color.Green(), 0);
                        if (c3 != xlBLACK) {
                            ApplyTransparency(c3, transparency);
                            va.AddVertex(sx, sy-pixelSize, c3);
                        }
                        c3.Set(0, 0, color.Blue());
                        if (c3 != xlBLACK) {
                            ApplyTransparency(c3, transparency);
                            va.AddVertex(sx+pixelSize, sy+pixelSize/2.0f, c3);
                        }
                    }
                } else {
                    xlColor c3(color);
                    ApplyTransparency(c3, color == xlBLACK ? blackTransparency : transparency);
                    va.AddVertex(sx, sy, c3);
                }
            } else {
                xlColor ccolor(color);
                xlColor ecolor(color);
                int trans = color == xlBLACK ? blackTransparency : transparency;
                ApplyTransparency(ccolor, trans);
                ApplyTransparency(ecolor, pixelStyle == 2 ? trans : 100);
                va.AddTrianglesCircle(sx, sy, ((float)pixelSize) / 2.0f, ccolor, ecolor);
            }
        }
    }
    if (pixelStyle > 1) {
        va.Finish(GL_TRIANGLES);
    } else {
        va.Finish(GL_POINTS, pixelStyle == 1 ? GL_POINT_SMOOTH : 0, preview->calcPixelSize(pixelSize));
    }
    if (Selected && c != nullptr && allowSelected) {
        GetModelScreenLocation().DrawHandles(va);
    }
}

wxString Model::GetNodeNear(ModelPreview* preview, wxPoint pt)
{
    int w, h;
    preview->GetSize(&w, &h);
    float scaleX = float(w) * 0.95 / GetModelScreenLocation().RenderWi;
    float scaleY = float(h) * 0.95 / GetModelScreenLocation().RenderHt;
    float scale = scaleY < scaleX ? scaleY : scaleX;

    float pointScale = scale;
    if (pointScale > 2.5) {
        pointScale = 2.5;
    }
    if (pointScale > GetModelScreenLocation().RenderHt) {
        pointScale = GetModelScreenLocation().RenderHt;
    }
    if (pointScale > GetModelScreenLocation().RenderWi) {
        pointScale = GetModelScreenLocation().RenderWi;
    }

    float px = pt.x;
    float py = h - pt.y;


    int i = 1;
    for (auto it = Nodes.begin(); it != Nodes.end(); ++it) {
        auto c = it->get()->Coords;
        for (auto it2 = c.begin(); it2 != c.end(); ++it2)
        {
            float sx=it2->screenX;
            float sy=it2->screenY;

            if (!GetModelScreenLocation().IsCenterBased()) {
                sx -= GetModelScreenLocation().RenderWi / 2.0;
                sy *= GetModelScreenLocation().GetVScaleFactor();
                if (GetModelScreenLocation().GetVScaleFactor() < 0) {
                    sy += GetModelScreenLocation().RenderHt / 2.0;
                } else {
                    sy -= GetModelScreenLocation().RenderHt / 2.0;
                }
            }
            sy = ((sy*scale)+(h/2));
            sx = (sx*scale)+(w/2);

            if (sx >= (px - pointScale) && sx <= (px + pointScale)  &&
                sy >= (py - pointScale) && sy <= (py + pointScale))
            {
                return wxString::Format(wxT("%i"),i);
            }
        }
        i++;
    }
    return "";
}

void Model::DisplayEffectOnWindow(ModelPreview* preview, double pointSize) {
    bool success = preview->StartDrawing(pointSize);

    if(success) {
        xlColor color;
        int w, h;

        preview->GetSize(&w, &h);

        float scaleX = float(w) * 0.95 / GetModelScreenLocation().RenderWi;
        float scaleY = float(h) * 0.95 / GetModelScreenLocation().RenderHt;
        float scale=scaleY < scaleX ? scaleY : scaleX;

        float pointScale = scale;
        if (pointScale > 2.5) {
            pointScale = 2.5;
        }
        if (pointScale > GetModelScreenLocation().RenderHt) {
            pointScale = GetModelScreenLocation().RenderHt;
        }
        if (pointScale > GetModelScreenLocation().RenderWi) {
            pointScale = GetModelScreenLocation().RenderWi;
        }

        LOG_GL_ERRORV(glPointSize(preview->calcPixelSize(pixelSize*pointScale)));
        int lastPixelStyle = pixelStyle;
        int lastPixelSize = pixelSize;

        // layer calculation and map to output
        size_t NodeCount=Nodes.size();
        int vcount = 0;
        for (auto it = Nodes.begin(); it != Nodes.end(); ++it) {
            vcount += it->get()->Coords.size();
        }
        if (vcount > maxVertexCount) {
            maxVertexCount = vcount;
        }
        DrawGLUtils::xlAccumulator va(maxVertexCount);
        float sx,sy;
        int first = 0; int last = NodeCount;
        int buffFirst = -1; int buffLast = -1;
        bool left = true;
        while (first < last) {
            int n = 0;
            if (left) {
                n = first;
                first++;
                if (NodeRenderOrder() == 1) {
                    if (buffFirst == -1) {
                        buffFirst = Nodes[n]->Coords[0].bufX;
                    }
                    if (first < NodeCount && buffFirst != Nodes[first]->Coords[0].bufX) {
                        left = false;
                    }
                }
            } else {
                last--;
                n = last;
                if (buffLast == -1) {
                    buffLast = Nodes[n]->Coords[0].bufX;
                }
                if (last > 0 && buffFirst != Nodes[last - 1]->Coords[0].bufX) {
                    left = true;
                }
            }

            Nodes[n]->GetColor(color);
            if (Nodes[n]->model->modelDimmingCurve != nullptr) {
                Nodes[n]->model->modelDimmingCurve->reverse(color);
            }
            if (Nodes[n]->model->StrobeRate) {
                int r = rand() % 5;
                if (r != 0) {
                    color = xlBLACK;
                }
            }
            size_t CoordCount=GetCoordCount(n);
            for(size_t c=0; c < CoordCount; c++) {
                // draw node on screen
                sx=Nodes[n]->Coords[c].screenX;
                sy=Nodes[n]->Coords[c].screenY;

                if (!GetModelScreenLocation().IsCenterBased()) {
                    sx -= GetModelScreenLocation().RenderWi / 2.0;
                    sy *= GetModelScreenLocation().GetVScaleFactor();
                    if (GetModelScreenLocation().GetVScaleFactor() < 0) {
                        sy += GetModelScreenLocation().RenderHt / 2.0;
                    } else {
                        sy -= GetModelScreenLocation().RenderHt / 2.0;
                    }
                }
                float newsy = ((sy*scale)+(h/2));


                if (lastPixelStyle != Nodes[n]->model->pixelStyle
                    || lastPixelSize != Nodes[n]->model->pixelSize) {

                    if (va.count && (lastPixelStyle < 2 || Nodes[n]->model->pixelStyle < 2)) {
                        if (lastPixelStyle > 1) {
                            va.Finish(GL_TRIANGLES);
                        } else {
                            va.Finish(GL_POINTS, lastPixelStyle == 1 ? GL_POINT_SMOOTH : 0, preview->calcPixelSize(lastPixelSize * pointScale));
                        }
                    }
                    lastPixelStyle = Nodes[n]->model->pixelStyle;
                    lastPixelSize = Nodes[n]->model->pixelSize;
                }

                if (lastPixelStyle < 2) {
                    if (splitRGB) {
                        float sxn = (sx*scale)+(w/2);
                        float syn = newsy;

                        if ((color.Red() == color.Blue()) && (color.Blue() == color.Green())) {
                            xlColor c2(color);
                            ApplyTransparency(c2, color == xlBLACK ? blackTransparency : transparency);
                            va.AddVertex(sxn, syn, c2);
                        } else {
                            xlColor c2(color.Red(), 0 , 0);
                            ApplyTransparency(c2, transparency);
                            if (c2 != xlBLACK) {
                                ApplyTransparency(c2, transparency);
                                va.AddVertex(sxn-pixelSize, syn+pixelSize/2.0f, c2);
                            }
                            c2.Set(0, color.Green(), 0);
                            if (c2 != xlBLACK) {
                                ApplyTransparency(c2, transparency);
                                va.AddVertex(sxn, syn-pixelSize, c2);
                            }
                            c2.Set(0, 0, color.Blue());
                            if (c2 != xlBLACK) {
                                ApplyTransparency(c2, transparency);
                                va.AddVertex(sxn+pixelSize, syn+pixelSize/2.0f, c2);
                            }
                        }
                    } else {
                        xlColor c2(color);
                        ApplyTransparency(c2, color == xlBLACK ? Nodes[n]->model->blackTransparency
                                                            : Nodes[n]->model->transparency);

                        sx = (sx*scale)+(w/2);
                        va.AddVertex(sx, newsy, c2);
                    }
                } else {
                    xlColor ccolor(color);
                    xlColor ecolor(color);
                    int trans = color == xlBLACK ? Nodes[n]->model->blackTransparency : Nodes[n]->model->transparency;
                    ApplyTransparency(ccolor, trans);
                    ApplyTransparency(ecolor, lastPixelStyle == 2 ? trans : 100);
                    va.AddTrianglesCircle((sx*scale)+(w/2), newsy, lastPixelSize*pointScale, ccolor, ecolor);
                }
            }
        }
        if (va.count) {
            if (va.count > maxVertexCount) {
                maxVertexCount = va.count;
            }
            if (lastPixelStyle > 1) {
                va.Finish(GL_TRIANGLES);
            } else {
                va.Finish(GL_POINTS, lastPixelStyle == 1 ? GL_POINT_SMOOTH : 0, preview->calcPixelSize(lastPixelSize * pointScale));
            }
            DrawGLUtils::Draw(va);
        }
        preview->EndDrawing();
    }
}

void Model::SetMinMaxModelScreenCoordinates(ModelPreview* preview) {
    int w, h;
    preview->GetVirtualCanvasSize(w, h);
    SetMinMaxModelScreenCoordinates(w, h);
}

void Model::SetMinMaxModelScreenCoordinates(int w, int h) {
    GetModelScreenLocation().SetPreviewSize(w, h, Nodes);
}

void Model::MoveHandle(ModelPreview* preview, int handle, bool ShiftKeyPressed, int mouseX,int mouseY) {

    if (GetModelScreenLocation().IsLocked()) return;

    int i = GetModelScreenLocation().MoveHandle(preview, handle, ShiftKeyPressed, mouseX, mouseY);
    GetModelScreenLocation().Write(ModelXml);
    if (i) {
        SetFromXml(ModelXml);
    }
    IncrementChangeCount();
}

void Model::SelectHandle(int handle) {
    GetModelScreenLocation().SelectHandle(handle);
}

int Model::GetSelectedHandle() {
    return GetModelScreenLocation().GetSelectedHandle();
}

int Model::GetNumHandles() {
    return GetModelScreenLocation().GetNumHandles();
}

int Model::GetSelectedSegment() {
    return GetModelScreenLocation().GetSelectedSegment();
}

bool Model::HasCurve(int segment) {
    return GetModelScreenLocation().HasCurve(segment);
}

void Model::SetCurve(int segment, bool create) {
    return GetModelScreenLocation().SetCurve(segment, create);
}

void Model::AddHandle(ModelPreview* preview, int mouseX, int mouseY) {
    GetModelScreenLocation().AddHandle(preview, mouseX, mouseY);
}

void Model::InsertHandle(int after_handle) {

    if (GetModelScreenLocation().IsLocked()) return;

    GetModelScreenLocation().InsertHandle(after_handle);
}

void Model::DeleteHandle(int handle) {

    if (GetModelScreenLocation().IsLocked()) return;

    GetModelScreenLocation().DeleteHandle(handle);
}

void Model::SetTop(ModelPreview* preview,int y) {

    if (GetModelScreenLocation().IsLocked()) return;

    SetMinMaxModelScreenCoordinates(preview);
    GetModelScreenLocation().SetTop(y);
    IncrementChangeCount();
}

void Model::SetBottom(ModelPreview* preview,int y) {

    if (GetModelScreenLocation().IsLocked()) return;

    SetMinMaxModelScreenCoordinates(preview);
    GetModelScreenLocation().SetBottom(y);
    IncrementChangeCount();
}

void Model::SetLeft(ModelPreview* preview,int x) {

    if (GetModelScreenLocation().IsLocked()) return;

    SetMinMaxModelScreenCoordinates(preview);
    GetModelScreenLocation().SetLeft(x);
    IncrementChangeCount();
}

void Model::SetRight(ModelPreview* preview,int x) {

    if (GetModelScreenLocation().IsLocked()) return;

    SetMinMaxModelScreenCoordinates(preview);
    GetModelScreenLocation().SetRight(x);
    IncrementChangeCount();
}

int Model::GetTop(ModelPreview* preview) {
    SetMinMaxModelScreenCoordinates(preview);
    return GetModelScreenLocation().GetTop();
}

int Model::GetWidth(ModelPreview* preview) {
    SetMinMaxModelScreenCoordinates(preview);
    return GetModelScreenLocation().GetMWidth();
}

int Model::GetHeight(ModelPreview* preview) {
    SetMinMaxModelScreenCoordinates(preview);
    return GetModelScreenLocation().GetMHeight();
}

void Model::SetWidth(ModelPreview* preview, int w) {

    if (GetModelScreenLocation().IsLocked()) return;

    SetMinMaxModelScreenCoordinates(preview);
    GetModelScreenLocation().SetMWidth(w);
    IncrementChangeCount();
}

void Model::SetHeight(ModelPreview* preview, int h) {

    if (GetModelScreenLocation().IsLocked()) return;

    SetMinMaxModelScreenCoordinates(preview);
    GetModelScreenLocation().SetMHeight(h);

    // this is necessary for three point models
    GetModelScreenLocation().Write(ModelXml);
    SetFromXml(ModelXml);

    IncrementChangeCount();
}

int Model::GetBottom(ModelPreview* preview) {
    SetMinMaxModelScreenCoordinates(preview);
    return GetModelScreenLocation().GetBottom();
}

int Model::GetLeft(ModelPreview* preview) {
    SetMinMaxModelScreenCoordinates(preview);
    return GetModelScreenLocation().GetLeft();
}

int Model::GetRight(ModelPreview* preview) {
    SetMinMaxModelScreenCoordinates(preview);
    return GetModelScreenLocation().GetRight();
}

float Model::GetHcenterOffset() {
    return GetModelScreenLocation().GetHcenterOffset();
}

float Model::GetVcenterOffset() {
    return GetModelScreenLocation().GetVcenterOffset();
}
void Model::SetHcenterOffset(float offset) {

    if (GetModelScreenLocation().IsLocked()) return;

    GetModelScreenLocation().SetHcenterOffset(offset);
    IncrementChangeCount();
}

void Model::SetVcenterOffset(float offset) {

    if (GetModelScreenLocation().IsLocked()) return;

    GetModelScreenLocation().SetVcenterOffset(offset);
    IncrementChangeCount();
}

int Model::GetStrandLength(int strand) const {
    return GetNodeCount() / GetNumStrands();
}

int Model::MapToNodeIndex(int strand, int node) const {
    if ((DisplayAs == wxT("Vert Matrix") || DisplayAs == wxT("Horiz Matrix") || DisplayAs == wxT("Matrix")) && SingleChannel) {
        return node;
    }
    if (GetNumStrands() == 1) {
        return node;
    }
    if (SingleNode) {
        return strand;
    }
    return (strand * parm2 / parm3) + node;
}

void Model::RecalcStartChannels()
{
    modelManager.OldRecalcStartChannels();
    //modelManager.NewRecalcStartChannels();
}

void Model::AddSizeLocationProperties(wxPropertyGridInterface *grid) {
    GetModelScreenLocation().AddSizeLocationProperties(grid);
}

void Model::SetLayoutGroup(const std::string &grp) {
    layout_group = grp;
    ModelXml->DeleteAttribute("LayoutGroup");
    ModelXml->AddAttribute("LayoutGroup", grp);
    IncrementChangeCount();
}

bool Model::SupportsXlightsModel()
{
    return false;
}

void Model::ImportXlightsModel(std::string filename, xLightsFrame* xlights, float& min_x, float& max_x, float& min_y, float& max_y)
{
}

void Model::ExportXlightsModel()
{
}

Model* Model::GetXlightsModel(Model* model, std::string &last_model, xLightsFrame* xlights, bool &cancelled)
{
    if (last_model == "")
    {
        wxString filename = wxFileSelector(_("Choose model file"), wxEmptyString, wxEmptyString, wxEmptyString, "xLights Model files (*.xmodel)|*.xmodel|LOR prop files (*.lff;*.lpf)|*.lff;*.lpf", wxFD_OPEN);
        if (filename.IsEmpty()) {
            cancelled = true;
            return model;
        }
        last_model = filename.ToStdString();
    }

    // if it isnt an xmodel then it is custom
    if (!wxString(last_model).Lower().EndsWith(".xmodel")) return model;

    wxXmlDocument doc(last_model);
    if (doc.IsOk())
    {
        wxXmlNode* root = doc.GetRoot();

        if (root->GetName() == "custommodel") {
            return model;
        } else if (root->GetName() == "polylinemodel") {
            // not a custom model so delete the default model that was created
            std::string startChannel = model->GetModelXml()->GetAttribute("StartChannel", "1").ToStdString();
            if( model != nullptr ) {
                delete model;
            }
            model = xlights->AllModels.CreateDefaultModel("Poly Line", startChannel);
            model->SetLayoutGroup(model->GetLayoutGroup());
            model->Selected = true;
            return model;
        }
        else if (root->GetName() == "matrixmodel") {

            // grab the attributes I want to keep
            std::string startChannel = model->GetModelXml()->GetAttribute("StartChannel", "1").ToStdString();
            auto x = model->GetHcenterOffset();
            auto y = model->GetVcenterOffset();
            auto w = ((BoxedScreenLocation&)model->GetModelScreenLocation()).GetScaleX();
            auto h = ((BoxedScreenLocation&)model->GetModelScreenLocation()).GetScaleY();

            // not a custom model so delete the default model that was created
            if (model != nullptr) {
                delete model;
            }
            model = xlights->AllModels.CreateDefaultModel("Matrix", startChannel);
            model->SetHcenterOffset(x);
            model->SetVcenterOffset(y);
            ((BoxedScreenLocation&)model->GetModelScreenLocation()).SetScale(w, h);
            model->SetLayoutGroup(model->GetLayoutGroup());
            model->Selected = true;
            return model;
        }
        else if (root->GetName() == "starmodel") {

            // grab the attributes I want to keep
            std::string startChannel = model->GetModelXml()->GetAttribute("StartChannel", "1").ToStdString();
            auto x = model->GetHcenterOffset();
            auto y = model->GetVcenterOffset();
            auto w = ((BoxedScreenLocation&)model->GetModelScreenLocation()).GetScaleX();
            auto h = ((BoxedScreenLocation&)model->GetModelScreenLocation()).GetScaleY();

            // not a custom model so delete the default model that was created
            if (model != nullptr) {
                delete model;
            }
            model = xlights->AllModels.CreateDefaultModel("Star", startChannel);
            model->SetHcenterOffset(x);
            model->SetVcenterOffset(y);
            ((BoxedScreenLocation&)model->GetModelScreenLocation()).SetScale(w, h);
            model->SetLayoutGroup(model->GetLayoutGroup());
            model->Selected = true;
            return model;
        }
        else if (root->GetName() == "treemodel") {

            // grab the attributes I want to keep
            std::string startChannel = model->GetModelXml()->GetAttribute("StartChannel", "1").ToStdString();
            auto x = model->GetHcenterOffset();
            auto y = model->GetVcenterOffset();
            auto w = ((BoxedScreenLocation&)model->GetModelScreenLocation()).GetScaleX();
            auto h = ((BoxedScreenLocation&)model->GetModelScreenLocation()).GetScaleY();

            // not a custom model so delete the default model that was created
            if (model != nullptr) {
                delete model;
            }
            model = xlights->AllModels.CreateDefaultModel("Tree", startChannel);
            model->SetHcenterOffset(x);
            model->SetVcenterOffset(y);
            ((BoxedScreenLocation&)model->GetModelScreenLocation()).SetScale(w, h);
            model->SetLayoutGroup(model->GetLayoutGroup());
            model->Selected = true;
            return model;
        }
        else if (root->GetName() == "dmxmodel") {

            // grab the attributes I want to keep
            std::string startChannel = model->GetModelXml()->GetAttribute("StartChannel", "1").ToStdString();
            auto x = model->GetHcenterOffset();
            auto y = model->GetVcenterOffset();
            auto w = ((BoxedScreenLocation&)model->GetModelScreenLocation()).GetScaleX();
            auto h = ((BoxedScreenLocation&)model->GetModelScreenLocation()).GetScaleY();

            // not a custom model so delete the default model that was created
            if (model != nullptr) {
                delete model;
            }
            model = xlights->AllModels.CreateDefaultModel("DMX", startChannel);
            model->SetHcenterOffset(x);
            model->SetVcenterOffset(y);
            ((BoxedScreenLocation&)model->GetModelScreenLocation()).SetScale(w, h);
            model->SetLayoutGroup(model->GetLayoutGroup());
            model->Selected = true;
            return model;
        }
        else
        {
            cancelled = true;
        }
    }
    return model;
}

wxString Model::SerialiseSubmodel() const
{
    wxString res = "";

    wxXmlNode * root = GetModelXml();
    wxXmlNode * child = root->GetChildren();
    while (child != nullptr) {
        if (child->GetName() == "subModel") {
            wxXmlDocument new_doc;
            new_doc.SetRoot(new wxXmlNode(*child));
            wxStringOutputStream stream;
            new_doc.Save(stream);
            wxString s = stream.GetString();
            s = s.SubString(s.Find("\n")+1, s.Length()); // skip over xml format header
            res += s;
        }
        child = child->GetNext();
    }

    return res;
}

bool Model::IsControllerConnectionValid() const
{
    return (Model::IsProtocolValid(GetProtocol()) && GetPort() > 0);
}

std::string Model::GetProtocol() const
{
    wxArrayString cc = wxSplit(controller_connection, ':');

    if (cc.size() > 0)
    {
        return cc[0].Lower().ToStdString();
    }

    return "";
}

int Model::GetPort() const
{
    wxArrayString cc = wxSplit(controller_connection, ':');

    if (cc.size() > 1)
    {
        int port = wxAtoi(cc[1]);
        return port;
    }

    return 0;
}

void Model::SetControllerConnection(const std::string& controllerConnection)
{
    controller_connection = controllerConnection;
    ModelXml->DeleteAttribute("ControllerConnection");
    if (controller_connection != "")
    {
        ModelXml->AddAttribute("ControllerConnection", controller_connection);
    }
    IncrementChangeCount();
}

std::list<std::string> Model::GetProtocols()
{
    std::list<std::string> res;

    res.push_back("WS2811");
    res.push_back("GECE");
    res.push_back("TM18XX");
    res.push_back("LX1203");
    res.push_back("WS2801");
    res.push_back("TLS3001");
    res.push_back("LPD6803");
    res.push_back("DMX");
    res.push_back("PixelNet");
    res.push_back("Renard");

    return res;
}

std::list<std::string> Model::GetLCProtocols()
{
    auto protocols = Model::GetProtocols();

    for (auto p = protocols.begin(); p != protocols.end(); ++p)
    {
        *p = wxString(*p).Lower().ToStdString();
    }

    return protocols;
}

bool Model::IsProtocolValid(std::string protocol)
{
    auto protocols = Model::GetLCProtocols();
    return (std::find(protocols.begin(), protocols.end(), protocol) != protocols.end());
}

std::list<std::string> Model::GetFaceFiles() const
{
    std::list<std::string> res;

    for (auto it = faceInfo.begin(); it != faceInfo.end(); ++it)
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            if (wxFile::Exists(it2->second))
            {
                res.push_back(it2->second);
            }
        }
    }

    return res;
}

std::vector<std::string> Model::GetModelState() const
{
    return modelState;
}

void Model::SaveModelState( std::vector<std::string>& state )
{
    modelState = state;
}
