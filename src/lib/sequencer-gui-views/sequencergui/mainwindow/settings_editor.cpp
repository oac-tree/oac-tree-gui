/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "settings_editor.h"

#include <QListView>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

SettingsEditor::SettingsEditor(QWidget *parent)
    : QWidget(parent)
    , m_splitter(new QSplitter)
    , m_top_item_list_view(new QListView)
    , m_settings_view(new QTreeView)
{
  auto layout = QVBoxLayout(this);
  layout.addWidget(m_splitter);

  m_splitter->addWidget(m_top_item_list_view);
  m_splitter->addWidget(m_settings_view);
}

SettingsEditor::~SettingsEditor() = default;

}  // namespace sequencergui
