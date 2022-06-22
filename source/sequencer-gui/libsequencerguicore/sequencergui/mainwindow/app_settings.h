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

#ifndef SEQUENCERGUI_MAINWINDOW_APP_SETTINGS_H_
#define SEQUENCERGUI_MAINWINDOW_APP_SETTINGS_H_

namespace sequencergui
{

enum class BehaviorTreeStyle
{
  kTopToBottom,
  kLeftToRight
};

class AppSettings
{
public:
  AppSettings();

  BehaviorTreeStyle GetTreeStyle() const;

  bool IsTopToBottomStyle() const;

private:
  BehaviorTreeStyle m_tree_style;
};
}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_APP_SETTINGS_H_
