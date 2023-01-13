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

#include "anyvalueeditor/anyvalue_editor_toolbar.h"

#include <anyvalueeditor/anyvalue_editor_actions.h>

#include <mvvm/widgets/widget_utils.h>

#include <sup/gui/dto/conversion_utils.h>

#include <QMenu>
#include <QPushButton>
#include <QToolButton>

namespace anyvalueeditor
{
AnyValueEditorToolBar::AnyValueEditorToolBar(AnyValueEditorActions *actions, QWidget *parent)
    : QToolBar(parent)
    , m_add_anyvalue_button(new QToolButton)
    , m_add_field_button(new QToolButton)
    , m_remove_button(new QToolButton)
    , m_actions(actions)
    , m_create_anyvalue_menu(AddAnyValueMenu(false))
    , m_add_field_menu(AddAnyValueMenu(true))
{
  setIconSize(QSize(24, 24));

  m_add_anyvalue_button->setText("Create AnyValue");
  m_add_anyvalue_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  m_add_anyvalue_button->setToolTip(
      "Creates new top level AnyValue. \nIt will be appended to the list of existing anyvalues");
  m_add_anyvalue_button->setPopupMode(QToolButton::InstantPopup);
  m_add_anyvalue_button->setMenu(m_create_anyvalue_menu.get());
  addWidget(m_add_anyvalue_button);

  m_add_field_button->setText("Add field");
  m_add_field_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  m_add_field_button->setToolTip("Add field after current selection.");
  m_add_field_button->setPopupMode(QToolButton::InstantPopup);
  m_add_field_button->setMenu(m_add_field_menu.get());
  addWidget(m_add_field_button);

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

std::unique_ptr<QMenu> AnyValueEditorToolBar::AddAnyValueMenu(bool to_selected)
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  {  // struct
    auto action = result->addAction("struct");
    connect(action, &QAction::triggered, this,
            [this, to_selected]() { m_actions->OnAddAnyValueStruct(to_selected); });
  }

  {  // array
    auto action = result->addAction("array");
    connect(action, &QAction::triggered, this,
            [this, to_selected]() { m_actions->OnAddAnyValueArray(to_selected); });
  }

  {
    auto scalar_menu = result->addMenu("scalar");
    for (const auto &name : sup::gui::GetScalarTypeNames())
    {
      auto on_action = [name, this, to_selected]()
      { m_actions->OnAddAnyValueScalar(name, to_selected); };
      auto action = scalar_menu->addAction(QString::fromStdString(name));
      connect(action, &QAction::triggered, this, on_action);
    }
  }

  return result;
}

}  // namespace anyvalueeditor
