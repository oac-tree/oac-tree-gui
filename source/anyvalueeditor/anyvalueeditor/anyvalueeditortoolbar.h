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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITORTOOLBAR_H
#define ANYVALUEEDITOR_ANYVALUEEDITORTOOLBAR_H

#include <QToolBar>
#include <memory>

class QToolButton;
class QMenu;

namespace anyvalueeditor
{

class AnyValueEditorActions;

//! A toolbar on top of AnyValueEditor.

class AnyValueEditorToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit AnyValueEditorToolBar(AnyValueEditorActions* actions, QWidget* parent = nullptr);
  ~AnyValueEditorToolBar() override;

private:
  void InsertStrech();
  std::unique_ptr<QMenu> CreateInsertAfterMenu();

  QToolButton* m_add_anyvalue_button{nullptr};
  QToolButton* m_add_field_button{nullptr};
  QToolButton* m_add_field_button_v2{nullptr};
  QToolButton* m_insert_field_button{nullptr};
  QToolButton* m_remove_button{nullptr};

  std::unique_ptr<QMenu> m_insert_after_menu;
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITORTOOLBAR_H
