/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/explorer/xmleditor.h"

#include "sequencergui/explorer/xmlsyntaxhighlighter.h"
#include "sequencergui/mainwindow/explorertoolbar.h"

#include <QFile>
#include <QLabel>
#include <QScrollBar>
#include <QTextEdit>
#include <QTextStream>
#include <QToolBar>
#include <QVBoxLayout>

namespace sequencergui
{
XMLEditor::XMLEditor(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new ExplorerToolBar)
    , m_text_edit(new QTextEdit)
    , m_syntax_highlighter(new XmlSyntaxHighlighter(m_text_edit->document()))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_text_edit);

  QFont textFont("Monospace");
  m_text_edit->setFont(textFont);
  m_text_edit->setLineWrapMode(QTextEdit::NoWrap);
}

void XMLEditor::SetXMLFile(const QString &file_name)
{
  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return;
  }

  QTextStream in(&file);
  QString text;
  while (!in.atEnd())
  {
    text.append(in.readLine() + "\n");
  }

  SetXMLContent(text);
}

void XMLEditor::SetXMLContent(const QString &content)
{
  const int old_scrollbar_value = m_text_edit->verticalScrollBar()->value();
  m_text_edit->clear();

  m_text_edit->setText(content);
  m_text_edit->verticalScrollBar()->setValue(old_scrollbar_value);
}

void XMLEditor::ClearText()
{
  m_text_edit->clear();
}

}  // namespace sequencergui
