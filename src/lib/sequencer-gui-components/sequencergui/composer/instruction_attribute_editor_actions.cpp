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
}

}  // namespace sequencergui
