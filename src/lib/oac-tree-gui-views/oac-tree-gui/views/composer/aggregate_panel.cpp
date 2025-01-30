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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "aggregate_panel.h"

#include "instruction_toolkit_tree_view.h"

#include <oac-tree-gui/model/aggregate_factory.h>
#include <oac-tree-gui/viewmodel/toolkit_viewmodel.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

AggregatePanel::AggregatePanel(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_toolkit_viewmodel(new ToolKitViewModel(this))
    , m_tree_view(new InsructionToolKitTreeView)
{
  setWindowTitle("AGGREGATES");

  m_tree_view->setModel(m_toolkit_viewmodel);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  const AggregateFactory factory;
  m_toolkit_viewmodel->PopulateModel(factory.GetKeys());
}

}  // namespace oac_tree_gui
