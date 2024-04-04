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

#ifndef SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_
#define SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_

#include <sequencergui/composer/attribute_editor_context.h>

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
 * The handler uses a callback to deduce which attribute is currently selected. Attribute actions
 * are defined on the board of AttributeEditorActions.
 */
class AttributeEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit AttributeEditorActionHandler(AttributeEditorContext context, QObject* parent = nullptr);
  ~AttributeEditorActionHandler() override;

  /**
   * @brief Checks if one can manipulate enable/disable flag for the currently selected attribute.
   */
  bool CanToggleEnabledFlag() const;

  /**
   * @brief Toggle currently selected attribute between enabled/disabled.
   */
  void OnToggleEnabledFlag();

  /**
   * @brief Checks if the currently selected attribute can be set to its default type.
   *
   * The default attribute type is what the Sequencer attribute system reports (string, integer,
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
  sup::gui::AnyValueItem *GetSelectedAttributeItem() const;

  AttributeEditorContext m_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_
