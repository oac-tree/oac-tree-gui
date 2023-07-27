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

#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

UniversalPropertyEditor::UniversalPropertyEditor(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<mvvm::PropertyViewModel>(m_tree_view))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);
}

UniversalPropertyEditor::~UniversalPropertyEditor() = default;

void UniversalPropertyEditor::SetItem(mvvm::SessionItem *item)
{
  m_component_provider->SetItem(item);
}

}  // namespace sequencergui
