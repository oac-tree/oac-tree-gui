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

#include "job_property_widget.h"

#include <sequencergui/model/job_item.h>

#include <mvvm/widgets/property_tree_view.h>

#include <QListWidget>
#include <QVBoxLayout>

namespace sequencergui
{

JobPropertyWidget::JobPropertyWidget(QWidget *parent)
    : QWidget(parent), m_property_tree_view(new mvvm::PropertyTreeView)
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

}  // namespace sequencergui
