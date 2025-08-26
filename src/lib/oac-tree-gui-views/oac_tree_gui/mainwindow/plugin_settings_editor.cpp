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

#include "plugin_settings_editor.h"

#include <oac_tree_gui/model/sequencer_settings_constants.h>
#include <oac_tree_gui/model/sequencer_settings_model.h>

#include <mvvm/utils/string_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

namespace oac_tree_gui
{

PluginSettingsEditor::PluginSettingsEditor(QWidget *parent_widget)
    : sup::gui::SessionItemWidget(parent_widget)
    , m_dir_list_checkbox(new QCheckBox)
    , m_dir_list_edit(new QTextEdit)
    , m_plugin_list_checkbox(new QCheckBox)
    , m_plugin_list_edit(new QTextEdit)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(mvvm::utils::UnitSize(0.5), 0, 0, 0);
  layout->setSpacing(mvvm::utils::UnitSize(1.75));
  layout->addLayout(CreateDescriptionLayout().release());
  layout->addLayout(CreateDirListLayout().release());
  layout->addLayout(CreatePluginListLayout().release());
}

void PluginSettingsEditor::SetItem(mvvm::SessionItem *item)
{
  (void)item;
}

std::unique_ptr<QLayout> PluginSettingsEditor::CreateDescriptionLayout()
{
  auto result = std::make_unique<QVBoxLayout>();
  result->setSpacing(mvvm::utils::UnitSize(0.5));

  auto label = new QLabel("Plugin settings");
  QFont font = label->font();
  font.setBold(true);
  label->setFont(font);
  result->addWidget(label);

  return result;
}

std::unique_ptr<QLayout> PluginSettingsEditor::CreateDirListLayout()
{
  auto result = std::make_unique<QVBoxLayout>();
  result->setSpacing(mvvm::utils::UnitSize(0.5));

  auto h_layout = std::make_unique<QHBoxLayout>();
  h_layout->addWidget(m_dir_list_checkbox);
  // h_layout->addSpacing(mvvm::utils::UnitSize(0.25));
  h_layout->addWidget(new QLabel("Use custom plugin directories to search for plugins"));
  h_layout->addStretch(1);

  result->addLayout(h_layout.release());
  // result->addSpacing(mvvm::utils::UnitSize(0.5));
  result->addWidget(m_dir_list_edit);

  return result;
}

std::unique_ptr<QLayout> PluginSettingsEditor::CreatePluginListLayout()
{
  auto result = std::make_unique<QVBoxLayout>();
  result->setSpacing(mvvm::utils::UnitSize(0.5));

  auto h_layout = std::make_unique<QHBoxLayout>();
  h_layout->addWidget(m_plugin_list_checkbox);
  // h_layout->addSpacing(mvvm::utils::UnitSize(0.25));
  h_layout->addWidget(new QLabel("Use custom plugin directories to search for plugins"));
  h_layout->addStretch(1);

  result->addLayout(h_layout.release());
  // result->addSpacing(mvvm::utils::UnitSize(0.5));
  result->addWidget(m_plugin_list_edit);

  return result;
}

}  // namespace oac_tree_gui
