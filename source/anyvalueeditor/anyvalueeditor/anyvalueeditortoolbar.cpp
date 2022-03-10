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

#include "anyvalueeditor/anyvalueeditortoolbar.h"

#include "anyvalueeditor/anyvalueeditoractions.h"

#include "mvvm/widgets/widgetutils.h"

#include <QPushButton>
#include <QToolButton>

namespace anyvalueeditor
{
AnyValueEditorToolBar::AnyValueEditorToolBar(AnyValueEditorActions *actions, QWidget *parent)
    : QToolBar(parent)
    , m_add_anyvalue_button(new QToolButton)
    , m_add_field_button(new QToolButton)
    , m_insert_field_button(new QToolButton)
    , m_remove_button(new QToolButton)
{
  setIconSize(QSize(24, 24));

  m_add_anyvalue_button->setText("Create AnyValue");
  m_add_anyvalue_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  m_add_anyvalue_button->setToolTip(
      "Creates new top level AnyValue. \nIt will be appended to the list of existing anyvalues");
  connect(m_add_anyvalue_button, &QToolButton::clicked, actions,
          &AnyValueEditorActions::OnAddAnyValue);
  addWidget(m_add_anyvalue_button);

  addSeparator();

  m_add_field_button->setText("Add field");
  m_add_field_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  m_add_field_button->setToolTip(
      "Add field to the selected AnyValue. \nField will be added currently selected field.");
  connect(m_add_field_button, &QToolButton::clicked, actions,
          &AnyValueEditorActions::OnAddField);
  addWidget(m_add_field_button);

  m_insert_field_button->setText("Insert field");
  m_insert_field_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  m_insert_field_button->setToolTip(
      "Insert field into the selected field. \nApplicable if selected item is a struct of array");
  connect(m_insert_field_button, &QToolButton::clicked, actions,
          &AnyValueEditorActions::OnInsertField);
  addWidget(m_insert_field_button);

  addSeparator();

  m_remove_button->setText("Remove");
  m_remove_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  m_remove_button->setToolTip("Remove selected item and all it's children");
  connect(m_remove_button, &QToolButton::clicked, actions,
          &AnyValueEditorActions::OnRemoveSelected);
  addWidget(m_remove_button);
}

AnyValueEditorToolBar::~AnyValueEditorToolBar() = default;

void AnyValueEditorToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace anyvalueeditor
