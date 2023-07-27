/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "universal_property_editor.h"

#include <sup/gui/widgets/custom_header_view.h>

#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("UniversalPropertyEditor");
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";
}  // namespace

namespace sequencergui
{

UniversalPropertyEditor::UniversalPropertyEditor(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_component_provider(mvvm::CreateProvider<mvvm::PropertyViewModel>(m_tree_view))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  m_custom_header->setStretchLastSection(true);
  m_tree_view->setHeader(m_custom_header);
  ReadSettings();
  AdjustTreeAppearance();
}

UniversalPropertyEditor::~UniversalPropertyEditor()
{
  WriteSettings();
}

void UniversalPropertyEditor::SetItem(mvvm::SessionItem *item)
{
  m_component_provider->SetItem(item);
  AdjustTreeAppearance();
}

void UniversalPropertyEditor::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kHeaderStateSettingName))
  {
    m_custom_header->SetAsFavoriteState(settings.value(kHeaderStateSettingName).toByteArray());
  }
}

void UniversalPropertyEditor::WriteSettings()
{
  QSettings settings;
  if (m_custom_header->HasFavoriteState())
  {
    settings.setValue(kHeaderStateSettingName, m_custom_header->GetFavoriteState());
  }
}

void UniversalPropertyEditor::AdjustTreeAppearance()
{
  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  else
  {
    m_tree_view->resizeColumnToContents(0);
  }
}

}  // namespace sequencergui
