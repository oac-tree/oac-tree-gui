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

#include "composer_combo_panel.h"
#include "composer_tools_panel.h"
#include "splittable_widget.h"

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
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_composer_tools_panel);
  m_splitter->addWidget(m_procedure_editor_area_widget);
  m_splitter->setSizes({200, 400});
}

SequencerComposerViewV2::~SequencerComposerViewV2() = default;

void SequencerComposerViewV2::SetModel(SequencerModel* model)
{
  m_composer_tools_panel->SetModel(model);
}

std::unique_ptr<QWidget> SequencerComposerViewV2::CreateProcedureEditor()
{
  auto result = std::make_unique<ComposerComboPanel>();
  auto result_ptr = result.get();

  connect(result.get(), &ComposerComboPanel::splitViewRequest, this,
          [this, result_ptr]() { m_procedure_editor_area_widget->AddWidget(result_ptr); });

  connect(result.get(), &ComposerComboPanel::closeViewRequest, this,
          [result_ptr]() { SplittableWidget::CloseWidget(result_ptr); });

  return result;
}

std::function<std::unique_ptr<QWidget>()> SequencerComposerViewV2::CreateProcedureEditorCallback()
{
  return [this]() { return this->CreateProcedureEditor(); };
}

}  // namespace oac_tree_gui
