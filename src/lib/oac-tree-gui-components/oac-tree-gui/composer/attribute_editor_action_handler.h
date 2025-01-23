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

#ifndef SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_
#define SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_

#include <oac-tree-gui/composer/attribute_editor_context.h>

#include <QObject>

namespace sup::gui
{
class AnyValueItem;
}  // namespace sup::gui

namespace sequencergui
{

/**
 * @brief The AttributeEditorActionHandler class implements the logic to handle actions related to
 * instruction/variable attribute editing.
 *
 * Actions itself are defined on the board of AttributeEditorActions. The handler uses a callback to
 * deduce which attribute is currently selected.
 */
class AttributeEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit AttributeEditorActionHandler(AttributeEditorContext context,
                                        QObject* parent_object = nullptr);
  ~AttributeEditorActionHandler() override;

  /**
   * @brief Checks if one can manipulate exposed flag for the currently selected attribute.
   *
   * Exposed attribute will be propagated to the sequencer domain.
   */
  bool CanToggleExposedFlag() const;

  /**
   * @brief Returns the value of the exposed flags for the currently selected attribute.
   */
  bool GetExposedFlag() const;

  /**
   * @brief Toggle currently selected attribute between expose/not expose.
   */
  void OnToggleExposedFlag();

  /**
   * @brief Checks if the currently selected attribute can be set to its default type.
   *
   * The default attribute type is what the oac-tree attribute system reports (string, integer,
   * float, etc). The alternative is the so-called placeholder string-like $par attribute which is
   * used in certain Include instruction scenario.
   */
  bool CanSetDefaultType() const;

  /**
   * @brief Resets attribute to its default type.
   * @see AttributeEditorActionHandler::CanSetDefaultType.
   */
  void OnSetAsDefaultType();

  /**
   * @brief Checks if currently selected attribute can be switched to placeholder type.
   * @see AttributeEditorActionHandler::CanSetDefaultType.
   */
  bool CanSetPlaceholderType() const;

  /**
   * @brief Resets attribute to placeholder type.
   * @see AttributeEditorActionHandler::CanSetDefaultType.
   */
  void OnSetPlaceholderType();

  /**
   * @brief Checks if it is possible to call external AnyValueEditor for the currently selected
   * attribute.
   *
   * We do not implement OnEditAnyValue since it is handled separately by
   * InstructionEditorActionHandler and WorkspaceEditorActionHandler.
   */
  bool CanEditAnyValue() const;

private:
  /**
   * @brief Returns currently seleced attribute item.
   */
  sup::gui::AnyValueItem* GetSelectedAttributeItem() const;

  /**
   * @brief Checks if the currently selected attribute represents a full-fledged AnyValue.
   */
  bool IsAnyValue() const;

  AttributeEditorContext m_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_
