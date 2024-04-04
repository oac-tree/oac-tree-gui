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
  explicit AttributeEditorActionHandler(AttributeEditorContext context,
                                        QObject* parent = nullptr);
  ~AttributeEditorActionHandler() override;



};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_
