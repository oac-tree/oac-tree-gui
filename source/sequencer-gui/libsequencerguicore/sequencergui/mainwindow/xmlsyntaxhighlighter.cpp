/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
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

#include "sequencergui/mainwindow/xmlsyntaxhighlighter.h"

namespace sequi
{
XmlSyntaxHighlighter::XmlSyntaxHighlighter(QObject* parent) : QSyntaxHighlighter(parent)
{
  setRegexes();
  setFormats();
}

XmlSyntaxHighlighter::XmlSyntaxHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
  setRegexes();
  setFormats();
}

void XmlSyntaxHighlighter::highlightBlock(const QString& text)
{
  // Special treatment for xml element regex as we use captured text to emulate lookbehind
  int xmlElementIndex = m_xmlElementRegex.indexIn(text);
  while (xmlElementIndex >= 0)
  {
    int matchedPos = m_xmlElementRegex.pos(1);
    int matchedLength = m_xmlElementRegex.cap(1).length();
    setFormat(matchedPos, matchedLength, m_xmlElementFormat);

    xmlElementIndex = m_xmlElementRegex.indexIn(text, matchedPos + matchedLength);
  }

  // Highlight xml keywords *after* xml elements to fix any occasional / captured into the enclosing
  // element
  typedef QList<QRegExp>::const_iterator Iter;
  Iter xmlKeywordRegexesEnd = m_xmlKeywordRegexes.end();
  for (Iter it = m_xmlKeywordRegexes.begin(); it != xmlKeywordRegexesEnd; ++it)
  {
    const QRegExp& regex = *it;
    highlightByRegex(m_xmlKeywordFormat, regex, text);
  }

  highlightByRegex(m_xmlAttributeFormat, m_xmlAttributeRegex, text);
  highlightByRegex(m_xmlCommentFormat, m_xmlCommentRegex, text);
  highlightByRegex(m_xmlValueFormat, m_xmlValueRegex, text);
}

void XmlSyntaxHighlighter::highlightByRegex(const QTextCharFormat& format, const QRegExp& regex,
                                            const QString& text)
{
  int index = regex.indexIn(text);

  while (index >= 0)
  {
    int matchedLength = regex.matchedLength();
    setFormat(index, matchedLength, format);

    index = regex.indexIn(text, index + matchedLength);
  }
}

void XmlSyntaxHighlighter::setRegexes()
{
  m_xmlElementRegex.setPattern("<[?\\s]*[/]?[\\s]*([^\\n][^>]*)(?=[\\s/>])");
  m_xmlAttributeRegex.setPattern("\\w+(?=\\=)");
  m_xmlValueRegex.setPattern("\"[^\\n\"]+\"(?=[?\\s/>])");
  m_xmlCommentRegex.setPattern("<!--[^\\n]*-->");

  m_xmlKeywordRegexes = QList<QRegExp>() << QRegExp("<\\?") << QRegExp("/>") << QRegExp(">")
                                         << QRegExp("<") << QRegExp("</") << QRegExp("\\?>");
}

void XmlSyntaxHighlighter::setFormats()
{
  m_xmlKeywordFormat.setForeground(Qt::blue);
  m_xmlKeywordFormat.setFontWeight(QFont::Bold);

  m_xmlElementFormat.setForeground(Qt::darkMagenta);
  m_xmlElementFormat.setFontWeight(QFont::Bold);

  m_xmlAttributeFormat.setForeground(Qt::darkGreen);
  m_xmlAttributeFormat.setFontWeight(QFont::Bold);
  m_xmlAttributeFormat.setFontItalic(true);

  m_xmlValueFormat.setForeground(Qt::darkRed);

  m_xmlCommentFormat.setForeground(Qt::gray);
}
}  // namespace sequi
