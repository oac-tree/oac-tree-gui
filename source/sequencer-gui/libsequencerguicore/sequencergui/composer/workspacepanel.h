/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_WORKSPACEPANEL_H
#define SEQUENCERGUI_COMPOSER_WORKSPACEPANEL_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

class QListWidget;

namespace sequencergui
{

//! Collapsible list with types of workspace variables.

class WorkspacePanel : public CollapsibleWidget
{
  Q_OBJECT

public:
  WorkspacePanel(QWidget* parent = nullptr);

private:
  QListWidget* m_list_widget{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_COMPOSER_WORKSPACEPANEL_H
