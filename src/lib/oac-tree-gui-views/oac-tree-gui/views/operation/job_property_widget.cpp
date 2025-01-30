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

#include "job_property_widget.h"

#include <oac-tree-gui/model/job_item.h>

#include <mvvm/views/property_tree_view.h>

#include <QListWidget>
#include <QVBoxLayout>

namespace oac_tree_gui
{

JobPropertyWidget::JobPropertyWidget(QWidget *parent_widget)
    : QWidget(parent_widget), m_property_tree_view(new mvvm::PropertyTreeView)
{
  setWindowTitle("JOB PROPERTIES");
  setToolTip("Properties of currently selected job");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_property_tree_view);
}

void JobPropertyWidget::SetJob(JobItem *item)
{
  m_property_tree_view->SetItem(item);
}

}  // namespace oac_tree_gui
