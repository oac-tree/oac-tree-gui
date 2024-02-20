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

#include "attribute_editor_actions.h"

#include "instruction_attribute_editor_context.h"

#include <sequencergui/model/attribute_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/attribute_item_transform_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace
{
const QString kEditAnyValueToolTip =
    "Edit AnyValue in external editor. Only certain variables as well as\n"
    "some EPICS related instructions can have complex AnyValues";
}  // namespace

namespace sequencergui
{

AttributeEditorActions::AttributeEditorActions(InstructionAttributeEditorContext context,
                                               QObject *parent)
    : QObject(parent)
    , m_modify_attribute_menu(std::make_unique<QMenu>())
    , m_modify_attribute_action(new QWidgetAction(this))
    , m_edit_anyvalue_action(new QWidgetAction(this))
    , m_editor_context(std::move(context))
{
  m_modify_attribute_menu->setToolTipsVisible(true);
  connect(m_modify_attribute_menu.get(), &QMenu::aboutToShow, this,
          &AttributeEditorActions::OnAboutToShowMenu);

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

  auto edit_anyvalue_button = new QToolButton;
  edit_anyvalue_button->setText("Edit AnyValue");
  edit_anyvalue_button->setIcon(sup::gui::utils::GetIcon("file-tree-outline.svg"));
  edit_anyvalue_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  edit_anyvalue_button->setToolTip(kEditAnyValueToolTip);
  connect(edit_anyvalue_button, &QToolButton::clicked, this,
          &AttributeEditorActions::EditAnyvalueRequest);
  m_edit_anyvalue_action->setDefaultWidget(edit_anyvalue_button);
}

AttributeEditorActions::~AttributeEditorActions() = default;

QList<QAction *> AttributeEditorActions::GetToolBarActions() const
{
  return QList<QAction *>({m_modify_attribute_action, m_edit_anyvalue_action});
}

void AttributeEditorActions::SetupMenu(QMenu &menu, sup::gui::AnyValueItem *attribute_item)
{
  // We clear menu and modify it with entries. It is done just a moment before showing the menu, to
  // take into account current selection and properly mark actions as enabled/disabled.

  menu.setToolTipsVisible(true);

  auto enable_action = AddEnableAttributeAction(menu, attribute_item);

  menu.addSeparator();
  auto set_default_value_action = AddSetDefaultValueAction(menu, attribute_item);
  auto set_placeholder_action = AddSetPlaceholderValueAction(menu, attribute_item);

  menu.addSeparator();
  auto edit_anyvalue_action = AddEditAnyValueAction(menu, attribute_item);

  if (attribute_item)
  {
    // all actions have been created in disable state, should re-enable some
    const bool is_anyvalue = attribute_item->GetTagIndex().tag == itemconstants::kAnyValueTag;

    enable_action->setEnabled(true);
    set_default_value_action->setEnabled(!is_anyvalue);
    set_placeholder_action->setEnabled(!is_anyvalue);
    edit_anyvalue_action->setEnabled(is_anyvalue);
  }
}

void AttributeEditorActions::OnAboutToShowMenu()
{
  m_modify_attribute_menu->clear();
  SetupMenu(*m_modify_attribute_menu, GetSelectedAnyValueItem());
}

QAction *AttributeEditorActions::AddEnableAttributeAction(QMenu &menu,
                                                          sup::gui::AnyValueItem *attribute_item)
{
  auto result = menu.addAction("Attribute is enabled");
  result->setToolTip("Attribute with enabled flag set will be propagated to domain");
  result->setCheckable(true);

  result->setChecked(attribute_item && IsAttributePresent(*attribute_item));

  auto on_action = [attribute_item]()
  { SetAttributePresentFlag(!IsAttributePresent(*attribute_item), *attribute_item); };
  connect(result, &QAction::triggered, on_action);
  result->setEnabled(false);

  return result;
}

QAction *AttributeEditorActions::AddSetDefaultValueAction(QMenu &menu,
                                                          sup::gui::AnyValueItem *attribute_item)
{
  auto result = menu.addAction("Set default value");
  result->setToolTip("The attribute will be set to its default value");
  auto on_action = [attribute_item]() { SetAttributeFromTypeName(*attribute_item); };
  connect(result, &QAction::triggered, on_action);
  result->setEnabled(false);

  return result;
}

QAction *AttributeEditorActions::AddSetPlaceholderValueAction(
    QMenu &menu, sup::gui::AnyValueItem *attribute_item)
{
  auto result = menu.addAction("Set placeholder attribute");
  result->setToolTip(
      "Attribute will be defined as string, allowing to use placeholders $par and references "
      "@par");
  auto on_action = [attribute_item]() { SetAttributeAsString("$par", *attribute_item); };
  connect(result, &QAction::triggered, on_action);
  result->setEnabled(false);

  return result;
}

QAction *AttributeEditorActions::AddEditAnyValueAction(QMenu &menu,
                                                       sup::gui::AnyValueItem *attribute_item)
{
  auto result = menu.addAction("Set placeholder attribute");
  result->setText("Edit AnyValue");
  result->setIcon(sup::gui::utils::GetIcon("file-tree-outline.svg"));
  result->setToolTip(kEditAnyValueToolTip);
  connect(result, &QAction::triggered, this, &AttributeEditorActions::EditAnyvalueRequest);
  result->setEnabled(false);

  return result;
}

sup::gui::AnyValueItem *AttributeEditorActions::GetSelectedAnyValueItem()
{
  return dynamic_cast<sup::gui::AnyValueItem *>(m_editor_context.selected_item_callback());
}

}  // namespace sequencergui
