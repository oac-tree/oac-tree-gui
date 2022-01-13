/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_XMLEDITOR_H
#define SEQUENCERGUI_MAINWINDOW_XMLEDITOR_H

#include <QWidget>

class QTextEdit;

namespace sequencergui
{
class XmlSyntaxHighlighter;
class ExplorerToolBar;

//! Simple XML viewer with syntax highlight.
class XMLEditor : public QWidget
{
  Q_OBJECT

public:
  XMLEditor(QWidget* parent = nullptr);

  void SetXMLFile(const QString& file_name);

  void ClearText();

private:
  ExplorerToolBar* m_tool_bar{nullptr};
  QTextEdit* m_text_edit{nullptr};
  XmlSyntaxHighlighter* m_syntax_highlighter{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_XMLEDITOR_H
