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

#ifndef SEQUENCERGUI_MAINWINDOW_ACTIONMANAGER_H
#define SEQUENCERGUI_MAINWINDOW_ACTIONMANAGER_H

#include <QWidget>

class QMainWindow;

namespace sequencergui
{

//! Vertical panel located on the left of XMLTreeView

class ActionManager : public QObject
{
  Q_OBJECT

public:
  explicit ActionManager(QMainWindow* mainwindow);
  ~ActionManager() override;

};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_ACTIONMANAGER_H
