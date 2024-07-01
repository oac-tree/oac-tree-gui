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

#ifndef SEQUENCERGUI_COMPONENTS_ANYVALUE_EXTENDED_EDITOR_H_
#define SEQUENCERGUI_COMPONENTS_ANYVALUE_EXTENDED_EDITOR_H_

#include <sup/gui/anyvalueeditor/abstract_anyvalue_editor.h>

class QLabel;
class QString;

namespace sup::gui
{
class AnyValueItem;
class AnyValueEditor;
}  // namespace sup::gui

namespace sequencergui
{

/**
 * @brief The AnyValueExtendedEditor class it's an envelop around full-fledged AnyValueEditor.
 */
class AnyValueExtendedEditor : public sup::gui::AbstractAnyValueEditor
{
  Q_OBJECT

public:
  explicit AnyValueExtendedEditor(QWidget* parent = nullptr);
  ~AnyValueExtendedEditor() override;

  void SetInitialValue(const sup::gui::AnyValueItem* item) override;

  std::unique_ptr<sup::gui::AnyValueItem> GetResult() override;

private:
  sup::gui::AnyValueEditor* m_editor{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_ANYVALUE_EXTENDED_EDITOR_H_
