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

#include "anyvalue_editor_toolbar.h"

#include <mvvm/widgets/widget_utils.h>

#include <sup/gui/anyvalueeditor/anyvalue_editor_actions.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QPushButton>
#include <QToolButton>

namespace
{

//! Returns icon from name. For the moment expects that png version of icon is available in the
//! resource folder.
QIcon GetIcon(const QString &icon_name)
{
  const QString icon_extension("svg");
  return QIcon(QString(":/icons/%1.%2").arg(icon_name, icon_extension));
}

}  // namespace

namespace sup::gui
{
AnyValueEditorToolBar::AnyValueEditorToolBar(AnyValueEditorActions *actions, QWidget *parent)
    : QToolBar(parent)
    , m_add_anyvalue_button(new QToolButton)
    , m_remove_button(new QToolButton)
    , m_hide_pannel_button(new QToolButton)
    , m_export_button(new QToolButton)
    , m_actions(actions)
    , m_create_anyvalue_menu(CreateAddAnyValueMenu())
{
  setIconSize(sup::gui::utils::ToolBarIconSize());

  m_add_anyvalue_button->setText("Add");
  m_add_anyvalue_button->setIcon(GetIcon("plus-circle-outline"));
  m_add_anyvalue_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_add_anyvalue_button->setToolTip(
      "Add new AnyValue to the view. If the view already\n"
      "contains AnyValue, try to add new AnyValue as a\n"
      "field to current selection");
  m_add_anyvalue_button->setPopupMode(QToolButton::InstantPopup);
  m_add_anyvalue_button->setMenu(m_create_anyvalue_menu.get());
  addWidget(m_add_anyvalue_button);

  m_remove_button->setText("Remove");
  m_remove_button->setIcon(GetIcon("beaker-remove-outline"));
  m_remove_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_remove_button->setToolTip("Remove selected item and all it's children");
  connect(m_remove_button, &QToolButton::clicked, actions,
          &AnyValueEditorActions::OnRemoveSelected);
  addWidget(m_remove_button);

  InsertStrech();

  m_hide_pannel_button->setText("JSON");
  m_hide_pannel_button->setIcon(GetIcon("code-json"));
  m_hide_pannel_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_hide_pannel_button->setToolTip("Hide/show the panel with JSON presentation");
  connect(m_hide_pannel_button, &QToolButton::clicked, this,
          &AnyValueEditorToolBar::HidePannelButtonRequest);
  addWidget(m_hide_pannel_button);

  m_export_button->setText("Export");
  m_export_button->setIcon(GetIcon("file-export-outline"));
  m_export_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_export_button->setToolTip("Export AnyValue to JSON file");
  connect(m_export_button, &QToolButton::clicked, this,
          &AnyValueEditorToolBar::ExportToFileRequest);
  addWidget(m_export_button);
}

AnyValueEditorToolBar::~AnyValueEditorToolBar() = default;

void AnyValueEditorToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

std::unique_ptr<QMenu> AnyValueEditorToolBar::CreateAddAnyValueMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  {  // struct
    auto action = result->addAction("struct");
    connect(action, &QAction::triggered, this, [this]() { m_actions->OnAddAnyValueStruct(); });
  }

  {  // array
    auto action = result->addAction("array");
    connect(action, &QAction::triggered, this, [this]() { m_actions->OnAddAnyValueArray(); });
  }

  {
    auto scalar_menu = result->addMenu("scalar");
    for (const auto &name : sup::gui::GetScalarTypeNames())
    {
      auto on_action = [name, this]() { m_actions->OnAddAnyValueScalar(name); };
      auto action = scalar_menu->addAction(QString::fromStdString(name));
      connect(action, &QAction::triggered, this, on_action);
    }
  }

  result->addSeparator();

  auto action = result->addAction("Import from file");
  connect(action, &QAction::triggered, this, &AnyValueEditorToolBar::ImportFromFileRequest);

  return result;
}

}  // namespace sup::gui
