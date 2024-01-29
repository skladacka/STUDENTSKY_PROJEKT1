/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/
//  Created by Daniel Kulp on 2/26/15.
//  Copyright (c) 2015 Daniel Kulp. All rights reserved.

#pragma once

#include <string>
#include "sequencer/Element.h"
#include "sequencer/Effect.h"
#include "sequencer/EffectLayer.h"

class RenderCommandEvent;
class SelectedEffectChangedEvent;

wxDECLARE_EVENT(EVT_RENDER_RANGE, RenderCommandEvent);
wxDECLARE_EVENT(EVT_SELECTED_EFFECT_CHANGED, SelectedEffectChangedEvent);

class RenderCommandEvent : public wxCommandEvent {
public:
    RenderCommandEvent() :  wxCommandEvent(EVT_RENDER_RANGE) {}
    RenderCommandEvent(const std::string &name,
                       int s,
                       int e,
                       bool c = true,
                       bool d = false)
        : wxCommandEvent(EVT_RENDER_RANGE),
          start(s), end(e), model(name),
          clear(c), deleted(d) {

    }
    RenderCommandEvent(const RenderCommandEvent &evt) : wxCommandEvent(evt), start(evt.start),
        end(evt.end),
        model(evt.model), clear(evt.clear), deleted(evt.deleted) {

    }

    virtual ~RenderCommandEvent() {}
    wxCommandEvent* Clone() const {return new RenderCommandEvent(*this);}


    int start, end;
    std::string model;
    bool clear;
    bool deleted;

    DECLARE_DYNAMIC_CLASS( RenderCommandEvent )

};

class SelectedEffectChangedEvent : public wxCommandEvent {
public:
    SelectedEffectChangedEvent(Effect *e, bool n, bool uui = true, bool ubtn = false)
        : wxCommandEvent(EVT_SELECTED_EFFECT_CHANGED),
            effect(e), isNew(n), updateUI(uui), updateBtn(ubtn) {
        if (e != nullptr)
        {
            // save the necessary details to identify the effect
            _elementName = e->GetParentEffectLayer()->GetParentElement()->GetFullName();
            NodeLayer*  nodeLayer = dynamic_cast<NodeLayer*>(e->GetParentEffectLayer());
            if (nodeLayer != nullptr)
            {
                StrandElement* se = (StrandElement*)e->GetParentEffectLayer()->GetParentElement();
                int baseindex = se->GetEffectLayer(se->GetEffectLayerCount()-1)->GetIndex();
                _node = nodeLayer->GetIndex() - baseindex;
            }
            else
            {
                _node = -1;
            }
            _layer = e->GetParentEffectLayer()->GetLayerNumber();
            _startTime = e->GetStartTimeMS();
        }
        else
        {
            _elementName = "";
            _node = -1;
            _layer = -1;
            _startTime = -100;
        }
    }
    SelectedEffectChangedEvent(const SelectedEffectChangedEvent &evt)
    : wxCommandEvent(evt), effect(evt.effect), isNew(evt.isNew), updateUI(evt.updateUI), updateBtn(evt.updateBtn), _elementName(evt._elementName), _layer(evt._layer), _startTime(evt._startTime), _node(evt._node) {
    }

    virtual ~SelectedEffectChangedEvent() {}
    wxCommandEvent* Clone() const {return new SelectedEffectChangedEvent(*this);}


    Effect *effect;
    bool isNew;
    bool updateUI;
    bool updateBtn;
    std::string _elementName;
    int _node;
    int _layer;
    int _startTime;
    DECLARE_DYNAMIC_CLASS( SelectedEffectChangedEvent )

protected:
    SelectedEffectChangedEvent()
        : wxCommandEvent(EVT_SELECTED_EFFECT_CHANGED), effect(nullptr), isNew(true), updateUI(true) {}

};
