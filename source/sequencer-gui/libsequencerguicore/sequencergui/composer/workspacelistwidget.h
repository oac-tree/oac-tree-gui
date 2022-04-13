/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_WORKSPACELISTWIDGET_H
#define SEQUENCERGUI_COMPOSER_WORKSPACELISTWIDGET_H

#include <QWidget>

namespace sequencergui
{

//! List of workspace variables at the right of SequencerComposerView.
//! Defines set of actions to add/remove variables. Part of ComposerProcedureEditor.

class WorkspaceListWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WorkspaceListWidget(QWidget* parent = nullptr);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_WORKSPACELISTWIDGET_H
