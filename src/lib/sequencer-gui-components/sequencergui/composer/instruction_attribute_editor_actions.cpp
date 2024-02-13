/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_attribute_editor_actions.h"

#include "instruction_attribute_editor_context.h"

#include <sequencergui/model/attribute_item.h>
#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace sequencergui
{

InstructionAttributeEditorActions::InstructionAttributeEditorActions(
    InstructionAttributeEditorContext context, QObject *parent)
    : QObject(parent)
    , m_modify_attribute_menu(std::make_unique<QMenu>())
    , m_modify_attribute_action(new QWidgetAction(this))
    , m_editor_context(std::move(context))
{
  m_modify_attribute_menu->setToolTipsVisible(true);
  connect(m_modify_attribute_menu.get(), &QMenu::aboutToShow, this,
          &InstructionAttributeEditorActions::OnAboutToShowMenu);

  // We wrap QToolButton into QWidgetAction to have a menu with instant popup capabilties (which is
  // a QToolButton feature) and still be able to pass actions around.
  auto modify_attribute_button = new QToolButton;
  modify_attribute_button->setText("Modify attribute");
  modify_attribute_button->setIcon(sup::gui::utils::GetIcon("page-previous-outline.svg"));
  modify_attribute_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  modify_attribute_button->setPopupMode(QToolButton::InstantPopup);
  modify_attribute_button->setMenu(m_modify_attribute_menu.get());
  modify_attribute_button->setToolTip("Modifies currently selected attribute");
  m_modify_attribute_action->setDefaultWidget(modify_attribute_button);
}

InstructionAttributeEditorActions::~InstructionAttributeEditorActions() = default;

QList<QAction *> InstructionAttributeEditorActions::GetToolBarActions() const
{
  return QList<QAction *>({m_modify_attribute_action});
}

void InstructionAttributeEditorActions::OnAboutToShowMenu()
{
  m_modify_attribute_menu->clear();

  // We clear menu and modify it with entries. It is done just a moment before showing the menu, to
  // take into account current selection and properly mark actions as enabled/disabled.

  auto enable_action = m_modify_attribute_menu->addAction("Attribute is enabled");
  enable_action->setToolTip("Attribute with enabled flag set will be propagated to domain.");
  enable_action->setCheckable(true);
  enable_action->setEnabled(false);

  auto set_default_value_action = m_modify_attribute_menu->addAction("Set default value");
  set_default_value_action->setToolTip("The attribute will be set to its default value");
  set_default_value_action->setEnabled(false);

  m_modify_attribute_menu->addSeparator();

  auto set_placeholder_action = m_modify_attribute_menu->addAction("Set placeholder attribute");
  set_placeholder_action->setToolTip(
      "Attribute will be defined as string, allowing to use placeholders $par and references "
      "@par");
  set_placeholder_action->setEnabled(false);

  if (auto attribute_item = GetSelectedAttributeItem(); attribute_item)
  {
    enable_action->setEnabled(true);
    enable_action->setChecked(attribute_item->IsPresent());
    auto on_unset = [attribute_item]()
    { attribute_item->SetPresentFlag(!attribute_item->IsPresent()); };
    connect(enable_action, &QAction::triggered, this, on_unset);

    set_default_value_action->setEnabled(true);
    auto on_default_attribute = [attribute_item]() { attribute_item->SetAttributeFromTypeName(); };
    connect(set_default_value_action, &QAction::triggered, this, on_default_attribute);

    set_placeholder_action->setEnabled(true);
    auto on_placeholder = [attribute_item]() { attribute_item->SetAttributeAsString("$par"); };
    connect(set_placeholder_action, &QAction::triggered, this, on_placeholder);
  }
}

AttributeItem *InstructionAttributeEditorActions::GetSelectedAttributeItem()
{
  return dynamic_cast<AttributeItem *>(m_editor_context.selected_item_callback());
}

}  // namespace sequencergui
