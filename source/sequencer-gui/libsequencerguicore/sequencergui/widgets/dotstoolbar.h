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

#ifndef SEQUENCERGUI_WIDGETS_DOTSTOOLBAR_H
#define SEQUENCERGUI_WIDGETS_DOTSTOOLBAR_H

#include <QToolBar>
#include <memory>

namespace sequencergui
{

//! Wide toolbar with dots menu for dynamic content. Intended for vertical panels with switcheable content.

class DotsToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit DotsToolBar(QWidget* parent = nullptr);
  ~DotsToolBar() override;

  void SetActions(const QList<QAction*>& actions);
  void SetWidgets(const QList<QWidget*>& widgets);

private:
  void AddDotsMenu();
  void InsertStrech();
  void SetupMenu();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_DOTSTOOLBAR_H
