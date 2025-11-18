/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencer_composer_view_v2.h"

#include "composer_tools_panel.h"
#include "procedure_composer_combo_panel.h"
#include "splittable_widget.h"

#include <oac_tree_gui/composer/widget_focus_handler.h>
#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace oac_tree_gui
{

SequencerComposerViewV2::SequencerComposerViewV2(sup::gui::IAppCommandService& command_service,
                                                 QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new QSplitter)
    , m_composer_tools_panel(new ComposerToolsPanel(command_service))
    , m_procedure_editor_area_widget(new SplittableWidget(CreateProcedureEditorCallback()))
    , m_focus_handler(std::make_unique<WidgetFocusHandler<ProcedureComposerComboPanel>>())
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_composer_tools_panel);
  m_splitter->addWidget(m_procedure_editor_area_widget);
  m_splitter->setSizes({200, 400});

  m_procedure_editor_area_widget->InitWidget();
}

SequencerComposerViewV2::~SequencerComposerViewV2() = default;

void SequencerComposerViewV2::SetModel(SequencerModel* model)
{
  m_composer_tools_panel->SetModel(model);
  m_model = model;
}

std::unique_ptr<QWidget> SequencerComposerViewV2::CreateProcedureEditor()
{
  auto get_procedures_callback = [this]() { return m_model->GetProcedures(); };
  auto result = std::make_unique<ProcedureComposerComboPanel>(get_procedures_callback);
  auto result_ptr = result.get();

  auto on_add_panel = [this]()
  {
    auto sending_panel = qobject_cast<ProcedureComposerComboPanel*>(sender());
    m_procedure_editor_area_widget->AddWidget(sending_panel);
  };
  connect(result.get(), &ProcedureComposerComboPanel::splitViewRequest, this, on_add_panel);

  auto on_remove_panel = [this]()
  {
    auto sending_panel = qobject_cast<ProcedureComposerComboPanel*>(sender());
    m_focus_handler->RemoveWidget(sending_panel);
    SplittableWidget::CloseWidget(sending_panel);
  };
  connect(result.get(), &ProcedureComposerComboPanel::closeViewRequest, this, on_remove_panel);

  auto on_focus_request = [this]()
  {
    qDebug() << "SequencerComposerViewV2::on_focus_request" << (this);

    auto sending_panel = qobject_cast<ProcedureComposerComboPanel*>(sender());
    m_focus_handler->SetInFocus(sending_panel);
  };
  connect(result.get(), &ProcedureComposerComboPanel::panelFocusRequest, this, on_focus_request);

  m_focus_handler->AddWidget(result_ptr);
  return result;
}

std::function<std::unique_ptr<QWidget>()> SequencerComposerViewV2::CreateProcedureEditorCallback()
{
  return [this]() { return this->CreateProcedureEditor(); };
}

}  // namespace oac_tree_gui
