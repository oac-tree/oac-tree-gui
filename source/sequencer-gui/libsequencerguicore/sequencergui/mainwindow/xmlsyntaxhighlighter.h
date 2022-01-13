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

#ifndef SEQUENCERGUI_MAINWINDOW_XMLSYNTAXHIGHLIGHTER_H
#define SEQUENCERGUI_MAINWINDOW_XMLSYNTAXHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QVector>

namespace sequencergui
{

//! Borrowed from https://github.com/d1vanov/basic-xml-syntax-highlighter

class XmlSyntaxHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT
public:
  XmlSyntaxHighlighter(QObject* parent);
  XmlSyntaxHighlighter(QTextDocument* parent);

protected:
  virtual void highlightBlock(const QString& text);

private:
  void highlightByRegex(const QTextCharFormat& format, const QRegExp& regex, const QString& text);

  void setRegexes();
  void setFormats();

private:
  QTextCharFormat m_xmlKeywordFormat;
  QTextCharFormat m_xmlElementFormat;
  QTextCharFormat m_xmlAttributeFormat;
  QTextCharFormat m_xmlValueFormat;
  QTextCharFormat m_xmlCommentFormat;

  QList<QRegExp> m_xmlKeywordRegexes;
  QRegExp m_xmlElementRegex;
  QRegExp m_xmlAttributeRegex;
  QRegExp m_xmlValueRegex;
  QRegExp m_xmlCommentRegex;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_XMLSYNTAXHIGHLIGHTER_H
