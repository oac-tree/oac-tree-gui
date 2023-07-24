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

#ifndef SEQUENCERGUI_WIDGETS_STEADY_MENU_H_
#define SEQUENCERGUI_WIDGETS_STEADY_MENU_H_

#include <QMenu>

namespace sequencergui
{

/**
 * @brief The SteadyMenu class is a menu that doesn't close after the user has made its choice.
 *
 * @details It is intended to show checkable actions and prevent the menu from closing while the
 * user checks/uncheck actions in the list.
 */

class SteadyMenu : public QMenu
{
  Q_OBJECT

public:
  explicit SteadyMenu(QWidget* parent = nullptr);
  ~SteadyMenu() override;

protected:
  void mouseReleaseEvent(QMouseEvent* event) override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_STEADY_MENU_H_
