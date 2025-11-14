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
#include "splittable_widget.h"

#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>

namespace oac_tree_gui
{

SequencerComposerViewV2::SequencerComposerViewV2(sup::gui::IAppCommandService& command_service,
                                                 QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new QSplitter)
    , m_procedure_editor_area_widget(new SplittableWidget(CreateProcedureEditorCallback()))
{
  (void)command_service;

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  auto sidebar = new QLabel("ABC");
  m_splitter->addWidget(sidebar);
  m_splitter->addWidget(m_procedure_editor_area_widget);
}

SequencerComposerViewV2::~SequencerComposerViewV2() = default;

std::unique_ptr<QWidget> SequencerComposerViewV2::CreateProcedureEditor()
{
  auto result = std::make_unique<ComposerComboPanel>();

  return result;
}

std::function<std::unique_ptr<QWidget>()> SequencerComposerViewV2::CreateProcedureEditorCallback()
{
  return [this]() { return this->CreateProcedureEditor(); };
}

}  // namespace oac_tree_gui
