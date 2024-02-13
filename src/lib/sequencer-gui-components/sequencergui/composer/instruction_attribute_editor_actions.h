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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_ATTRIBUTE_EDITOR_ACTIONS_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_ATTRIBUTE_EDITOR_ACTIONS_H_

#include <QObject>

#include <memory>
#include <sequencergui/composer/instruction_attribute_editor_context.h>

class QWidgetAction;
class QMenu;
class QAction;

namespace sequencergui
{

class AttributeItem;

/**
 * @brief The InstructionAttributeEditorActions class represent various actions which can be
 * performed with instruction attributes.
 *
 * @details It's a part of InstructionAttributeEditor. It is responsible for consrtuction of context
 * menu for instruction property editor. Handles the logic which actions can be applied to given
 * selection.
 */
class InstructionAttributeEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit InstructionAttributeEditorActions(InstructionAttributeEditorContext context, QObject* parent = nullptr);
  ~InstructionAttributeEditorActions() override;

  /**
   * @brief Returns list of actions intended for a toolbar.
   */
  QList<QAction*> GetToolBarActions() const;

  /**
   * @brief Setup empty menu with actions available for given attribute.
   */
  static void SetupMenu(QMenu& menu, AttributeItem* attribute_item);

private:
  void OnAboutToShowMenu();
  AttributeItem* GetSelectedAttributeItem();

  //!< the menu attached to action, generated on the fly
  std::unique_ptr<QMenu> m_modify_attribute_menu;

  //!< a tool button with instant menu to embed in a toolbar
  QWidgetAction* m_modify_attribute_action{nullptr};

  InstructionAttributeEditorContext m_editor_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_ATTRIBUTE_EDITOR_ACTIONS_H_
