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

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERTREETOOLBAR_H
#define SEQUENCERGUI_COMPOSER_COMPOSERTREETOOLBAR_H

#include <QToolBar>
#include <memory>

namespace sequencergui
{
//! A toolbar on top of ComposerTreeWidget with control elements to add/remove instructions or
//! variables.

class ComposerTreeToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit ComposerTreeToolBar(QWidget* parent = nullptr);
  ~ComposerTreeToolBar() override;

  void SetActions(const QList<QAction*>& actions);
  void SetWidgets(const QList<QWidget*>& widgets);

private:
  void AddDotsMenu();
  void InsertStrech();
  void SetupMenu();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERTREETOOLBAR_H
