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

#ifndef SEQUENCERGUI_MAINWINDOW_USERINTERACTOR_H
#define SEQUENCERGUI_MAINWINDOW_USERINTERACTOR_H

#include <string>

class QWidget;

namespace mvvm
{
enum class SaveChangesAnswer;
}

namespace sequencergui
{

class RecentProjectSettings;

//! Provide save/discard/cancel and similar dialogs on user request.
//! Intended to work in pair with ProjectManagerDecorator.

class UserInteractor
{
public:
  UserInteractor(RecentProjectSettings* settings, QWidget* parent);

  std::string OnSelectDirRequest();

  std::string OnCreateDirRequest();

  mvvm::SaveChangesAnswer OnSaveChangesRequest();

private:
  std::string SummonSelectDialog() const;

  RecentProjectSettings* m_settings{nullptr};
  QWidget* m_parent{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_USERINTERACTOR_H
