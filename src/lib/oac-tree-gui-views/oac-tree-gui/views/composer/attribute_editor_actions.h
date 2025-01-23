/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_ATTRIBUTE_EDITOR_ACTIONS_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_ATTRIBUTE_EDITOR_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QObject>
#include <memory>

class QMenu;
class QAction;

namespace sup::gui
{
class ActionMenu;
}  // namespace sup::gui

namespace sequencergui
{

class AttributeEditorActionHandler;

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
  /**
   * @brief The ActionKey enum defines keys for all available actions.
   */
  enum class ActionKey : std::uint8_t
  {
    kModifyAttribute,
    kEditAnyValue,
    kTotalCount
  };

  explicit AttributeEditorActions(const AttributeEditorActionHandler* handler,
                                  QObject* parent_object = nullptr);
  ~AttributeEditorActions() override;

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

  /**
   * @brief Setup menu with actions available for given attribute.
   *
   * The menu can come from outside (context menu), or it can be the menu of our own tool button.
   */
  void SetupMenu(QMenu& menu);

signals:
  void EditAnyvalueRequest();

private:
  /**
   * @brief Populates menu of the button with actions available for given attribute.
   */
  void OnAboutToShowMenu();

  /**
   * @brief Adds action to the menu which will toggle exposed flag.
   */
  void AddToggleExposedFlagAction(QMenu& menu);

  /**
   * @brief Adds action to set attribute default value.
   */
  void AddSetDefaultValueAction(QMenu& menu);

  /**
   * @brief Adds action to set attribute placeholder value.
   */
  void AddSetPlaceholderValueAction(QMenu& menu);

  /**
   * @brief Adds action to edit AnyValue.
   */
  void AddEditAnyValueAction(QMenu& menu);

  const AttributeEditorActionHandler* m_handler{nullptr};

  //!< the menu attached to action, generated on the fly
  std::unique_ptr<QMenu> m_modify_attribute_menu;

  //!< a tool button with instant menu to embed in a toolbar
  sup::gui::ActionMenu* m_modify_attribute_action{nullptr};

  //!< a tool button to summon AnyValue editor
  QAction* m_edit_anyvalue_action{nullptr};

  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_ATTRIBUTE_EDITOR_ACTIONS_H_
