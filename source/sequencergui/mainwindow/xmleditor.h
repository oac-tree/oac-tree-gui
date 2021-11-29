/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_XMLEDITOR_H
#define SEQUENCERGUI_MAINWINDOW_XMLEDITOR_H

#include <QWidget>

class QTextEdit;

namespace sequi
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
