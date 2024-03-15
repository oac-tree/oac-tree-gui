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

#ifndef SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTIONS_H_
#define SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTIONS_H_

#include <sequencergui/composer/instruction_attribute_editor_context.h>

#include <QObject>
#include <memory>

class QWidgetAction;
class QMenu;
class QAction;

namespace sup::gui
{
class AnyValueItem;
}

namespace sequencergui
{

/**
 * @brief The AttributeEditorActions class represents various actions that can be performed with
 * instruction/variable attributes.
 *
 * It's a part of InstructionAttributeEditor and WorkspaceEditor. It is responsible for the
 * construction of a context menu for the attribute property editor with actions properly enabled
 * or disabled.
 */
class AttributeEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit AttributeEditorActions(InstructionAttributeEditorContext context,
                                  QObject* parent = nullptr);
  ~AttributeEditorActions() override;

  /**
   * @brief Returns list of actions intended for a toolbar.
   */
  QList<QAction*> GetToolBarActions() const;

  /**
   * @brief Setup menu with actions available for given attribute.
   *
   * The menu can come from outside (context menu), or it can be the menu of our own tool button.
   */
  void SetupMenu(QMenu& menu, sup::gui::AnyValueItem* attribute_item);

signals:
  void EditAnyvalueRequest();

private:
  /**
   * @brief Populates menu of the button with actions available for given attribute.
   */
  void OnAboutToShowMenu();

  /**
   * @brief Adds action to the menu which will toggle attribute between enabled/disabled.
   */
  QAction* AddEnableAttributeAction(QMenu& menu, sup::gui::AnyValueItem* attribute_item);

  /**
   * @brief Adds action to set attribute default value.
   */
  static QAction* AddSetDefaultValueAction(QMenu& menu, sup::gui::AnyValueItem* attribute_item);

  /**
   * @brief Adds action to set attribute placeholder value.
   */
  static QAction* AddSetPlaceholderValueAction(QMenu& menu, sup::gui::AnyValueItem* attribute_item);

  /**
   * @brief Adds action to edit AnyValue.
   */
  QAction* AddEditAnyValueAction(QMenu& menu, sup::gui::AnyValueItem* attribute_item);

  sup::gui::AnyValueItem* GetSelectedAnyValueItem();

  //!< the menu attached to action, generated on the fly
  std::unique_ptr<QMenu> m_modify_attribute_menu;

  //!< a tool button with instant menu to embed in a toolbar
  QWidgetAction* m_modify_attribute_action{nullptr};

  //!< a tool button to summon AnyValue editor
  QWidgetAction* m_edit_anyvalue_action{nullptr};

  InstructionAttributeEditorContext m_editor_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTIONS_H_
