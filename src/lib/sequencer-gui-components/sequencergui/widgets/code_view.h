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

#ifndef SEQUENCERGUI_WIDGETS_CODE_VIEW_H_
#define SEQUENCERGUI_WIDGETS_CODE_VIEW_H_

#include <QWidget>

namespace sup::gui
{
class CodeEditor;
}

namespace sequencergui
{

/**
 * @brief Simple viewer of XML/JSON content.
 *
 * @details It is intended for dynamic updates of text content while preserving the position
 * of the scroll area. It allows to monitor the XML/JSON representation of the model in the
 * course of model update.
 */

class CodeView : public QWidget
{
  Q_OBJECT

public:
  explicit CodeView(QWidget* parent = nullptr);
  ~CodeView() override;

  void SetXMLFile(const QString& file_name);

  void SetXMLContent(const QString& content);

  void ClearText();

private:
  void ReadSettings();
  void WriteSettings();
  void SetupActions();

  sup::gui::CodeEditor* m_text_edit{nullptr};
  QString m_current_workdir;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_CODE_VIEW_H_
