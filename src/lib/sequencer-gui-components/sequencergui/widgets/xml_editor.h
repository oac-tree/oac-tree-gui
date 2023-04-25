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

#ifndef SEQUENCERGUI_WIDGETS_XML_EDITOR_H_
#define SEQUENCERGUI_WIDGETS_XML_EDITOR_H_

#include <QWidget>

class QTextEdit;

namespace sequencergui
{
class XmlSyntaxHighlighter;

//! Simple XML viewer with syntax highlight.
class XMLEditor : public QWidget
{
  Q_OBJECT

public:
  explicit XMLEditor(QWidget* parent = nullptr);
  ~XMLEditor();

  void SetXMLFile(const QString& file_name);

  void SetXMLContent(const QString& content);

  void ClearText();

signals:
  void ExportXmlRequest();

private:
  void ReadSettings();
  void WriteSettings();
  void SetupActions();

  QTextEdit* m_text_edit{nullptr};
  XmlSyntaxHighlighter* m_syntax_highlighter{nullptr};
  QString m_current_workdir;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_XML_EDITOR_H_
