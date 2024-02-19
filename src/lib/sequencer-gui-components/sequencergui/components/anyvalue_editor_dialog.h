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

#ifndef SEQUENCERGUI_COMPONENTS_ANYVALUE_EDITOR_DIALOG_H_
#define SEQUENCERGUI_COMPONENTS_ANYVALUE_EDITOR_DIALOG_H_

#include <QDialog>
#include <memory>

namespace sup::gui
{
class AnyValueItem;
}  // namespace sup::gui

namespace sequencergui
{

class AbstractAnyValueEditor;

/**
 * @brief The AnyValueEditorDialog class is a modal dialog containing one of AnyValue editors.
 */
class AnyValueEditorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AnyValueEditorDialog(std::unique_ptr<AbstractAnyValueEditor> editor,
                                QWidget* parent = nullptr);
  ~AnyValueEditorDialog() override;

  void SetInitialValue(const sup::gui::AnyValueItem* item);

  std::unique_ptr<sup::gui::AnyValueItem> GetResult();

  void SetDescription(const QString& description);

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  void ReadSettings();
  void WriteSettings();

  AbstractAnyValueEditor* m_anyvalue_editor{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_ANYVALUE_EDITOR_DIALOG_H_
