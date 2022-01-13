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

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERTREETOOLBAR_H
#define SEQUENCERGUI_COMPOSER_COMPOSERTREETOOLBAR_H

#include <QToolBar>
#include <memory>

class QToolButton;
class QLabel;
class QMenu;
class QPushButton;

namespace sequencergui
{
//! A toolbar on top of ComposerTreeWidget with control elements to add/remove instructions or
//! variables.

class ComposerTreeToolBar : public QToolBar
{
  Q_OBJECT

public:
  ComposerTreeToolBar(QWidget* parent = nullptr);
  ~ComposerTreeToolBar();

signals:
  void insertIntoRequest(const QString& name);
  void insertAfterRequest(const QString& name);
  void removeSelectedRequest();

private:
  void AddDotsMenu();
  void InsertStrech();
  void SetupMenu();

  std::unique_ptr<QMenu> CreateInsertAfterMenu();
  std::unique_ptr<QMenu> CreateInsertIntoMenu();

  QToolButton* m_insert_after_button{nullptr};
  QToolButton* m_insert_into_button{nullptr};
  QToolButton* m_remove_button{nullptr};

  std::unique_ptr<QMenu> m_insert_into_menu;
  std::unique_ptr<QMenu> m_insert_after_menu;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERTREETOOLBAR_H
