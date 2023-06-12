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

#include "operation_job_panel_toolbar.h"

#include <sequencergui/widgets/style_utils.h>
#include <sup/gui/widgets/style_utils.h>

#include <QToolButton>

namespace sequencergui
{

OperationJobPanelToolBar::OperationJobPanelToolBar(QWidget *parent) : QToolBar(parent)
{
  setIconSize(sup::gui::utils::ToolBarIconSize());

  auto import_button = new QToolButton;
  import_button->setText("New job");
  import_button->setIcon(styleutils::GetIcon("file-plus-outline"));
  import_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  import_button->setToolTip("Submit existing sequencer XML procedure for execution");
  connect(import_button, &QToolButton::clicked, this, &OperationJobPanelToolBar::ImportJobRequest);
  addWidget(import_button);

  auto regenerate_button = new QToolButton;
  regenerate_button->setText("Reload");
  regenerate_button->setIcon(styleutils::GetIcon("refresh"));
  regenerate_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  regenerate_button->setToolTip("Reload selected procedure");
  connect(regenerate_button, &QToolButton::clicked, this,
          &OperationJobPanelToolBar::RegenerateJobRequest);
  addWidget(regenerate_button);

  auto remove_button = new QToolButton;
  remove_button->setText("Remove");
  remove_button->setIcon(styleutils::GetIcon("beaker-remove-outline"));
  remove_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  remove_button->setToolTip("Remove selected procedure from the list");
  connect(remove_button, &QToolButton::clicked, this, &OperationJobPanelToolBar::RemoveJobRequest);
  addWidget(remove_button);
}

}  // namespace sequencergui
