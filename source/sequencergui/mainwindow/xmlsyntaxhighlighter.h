/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_XMLSYNTAXHIGHLIGHTER_H
#define SEQUENCERGUI_MAINWINDOW_XMLSYNTAXHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QVector>

namespace sequi
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

#endif // SEQUENCERGUI_MAINWINDOW_XMLSYNTAXHIGHLIGHTER_H
