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

#include "attribute_editor_action_handler.h"

#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/attribute_item_transform_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/widgets/action_menu.h>
#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>

namespace
{
const QString kEditAnyValueToolTip =
    "Edit AnyValue in external editor. Only certain variables as well as\n"
    "some EPICS related instructions can have complex AnyValues";
}  // namespace

namespace sequencergui
{

AttributeEditorActions::AttributeEditorActions(const AttributeEditorActionHandler *handler,
                                               QObject *parent)
    : QObject(parent)
    , m_handler(handler)
    , m_modify_attribute_menu(std::make_unique<QMenu>())
    , m_modify_attribute_action(new sup::gui::ActionMenu(this))
    , m_edit_anyvalue_action(new QAction(this))
{
  m_modify_attribute_menu->setToolTipsVisible(true);
  connect(m_modify_attribute_menu.get(), &QMenu::aboutToShow, this,
          &AttributeEditorActions::OnAboutToShowMenu);

  m_modify_attribute_action->setText("Modify attribute");
  m_modify_attribute_action->setIcon(sup::gui::utils::GetIcon("page-previous-outline.svg"));
  m_modify_attribute_action->setMenu(m_modify_attribute_menu.get());
  m_modify_attribute_action->setToolTip("Modifies currently selected attribute");
  m_action_map.Add(ActionKey::kModifyAttribute, m_modify_attribute_action);

  m_edit_anyvalue_action->setText("Edit AnyValue");
  m_edit_anyvalue_action->setIcon(sup::gui::utils::GetIcon("file-tree-outline.svg"));
  m_edit_anyvalue_action->setToolTip(kEditAnyValueToolTip);
  connect(m_edit_anyvalue_action, &QAction::triggered, this,
          &AttributeEditorActions::EditAnyvalueRequest);
  m_action_map.Add(ActionKey::kEditAnyValue, m_edit_anyvalue_action);
}

AttributeEditorActions::~AttributeEditorActions() = default;

QList<QAction *> AttributeEditorActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void AttributeEditorActions::SetupMenu(QMenu &menu)
{
  menu.setToolTipsVisible(true);

  AddToggleExposedFlagAction(menu);

  menu.addSeparator();
  AddSetDefaultValueAction(menu);
  AddSetPlaceholderValueAction(menu);

  menu.addSeparator();
  AddEditAnyValueAction(menu);
}

void AttributeEditorActions::OnAboutToShowMenu()
{
  m_modify_attribute_menu->clear();
  SetupMenu(*m_modify_attribute_menu);
}

void AttributeEditorActions::AddToggleExposedFlagAction(QMenu &menu)
{
  auto result = menu.addAction("Attribute exposed flag");
  result->setToolTip(
      "Attribute with exposed flag set will be propagated to sequencer domain,\n"
      "if flag is unchecked, the sequencer will not see it.");
  result->setCheckable(true);
  connect(result, &QAction::triggered, m_handler,
          &AttributeEditorActionHandler::OnToggleExposedFlag);
  result->setChecked(m_handler->GetExposedFlag());
  result->setEnabled(m_handler->CanToggleExposedFlag());
}

void AttributeEditorActions::AddSetDefaultValueAction(QMenu &menu)
{
  auto action = menu.addAction("Set default value");
  action->setToolTip("The attribute will be set to its default value");
  connect(action, &QAction::triggered, m_handler,
          &AttributeEditorActionHandler::OnSetAsDefaultType);
  action->setEnabled(m_handler->CanSetDefaultType());
}

void AttributeEditorActions::AddSetPlaceholderValueAction(QMenu &menu)
{
  auto action = menu.addAction("Set placeholder attribute");
  action->setToolTip(
      "Attribute will be defined as string, allowing to use placeholders $par and references "
      "@par");
  connect(action, &QAction::triggered, m_handler,
          &AttributeEditorActionHandler::OnSetPlaceholderType);
  action->setEnabled(m_handler->CanSetPlaceholderType());
}

void AttributeEditorActions::AddEditAnyValueAction(QMenu &menu)
{
  auto action = menu.addAction("Set placeholder attribute");
  action->setText("Edit AnyValue");
  action->setIcon(sup::gui::utils::GetIcon("file-tree-outline.svg"));
  action->setToolTip(kEditAnyValueToolTip);
  connect(action, &QAction::triggered, this, &AttributeEditorActions::EditAnyvalueRequest);
  action->setEnabled(m_handler->CanEditAnyValue());
}

}  // namespace sequencergui
