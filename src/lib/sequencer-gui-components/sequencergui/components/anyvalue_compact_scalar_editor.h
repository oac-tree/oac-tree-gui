/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPONENTS_ANYVALUE_COMPACT_SCALAR_EDITOR_H_
#define SEQUENCERGUI_COMPONENTS_ANYVALUE_COMPACT_SCALAR_EDITOR_H_

#include <QWidget>

class QLabel;
class QString;

namespace sup::gui
{
class AnyValueItem;
}  // namespace sup::gui

namespace sequencergui
{

/**
 * @brief The AnyValueCompactScalarEditor class provides a simplified AnyValueEditor for scalar
 * types.
 */

class AnyValueCompactScalarEditor : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueCompactScalarEditor(QWidget* parent = nullptr);
  ~AnyValueCompactScalarEditor() override;

  void SetDescription(const QString& text);

private:
  QLabel* m_label{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_ANYVALUE_COMPACT_SCALAR_EDITOR_H_
