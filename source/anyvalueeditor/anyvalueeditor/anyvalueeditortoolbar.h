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

namespace anyvalueeditor
{

//! A toolbar on top of AnyValueEditor.

class AnyValueEditorToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit AnyValueEditorToolBar(QWidget* parent = nullptr);
  ~AnyValueEditorToolBar() override;

signals:
  void insertIntoRequest(const QString& name);
  void insertAfterRequest(const QString& name);
  void removeSelectedRequest();

private:
  void InsertStrech();

  QToolButton* m_insert_after_button{nullptr};
  QToolButton* m_insert_into_button{nullptr};
  QToolButton* m_remove_button{nullptr};
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITORTOOLBAR_H
