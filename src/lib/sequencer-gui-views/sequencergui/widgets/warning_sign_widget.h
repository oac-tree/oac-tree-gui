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

#ifndef SEQUENCERGUI_WIDGETS_WARNING_SIGN_WIDGET_H_
#define SEQUENCERGUI_WIDGETS_WARNING_SIGN_WIDGET_H_

#include <QPixmap>
#include <QWidget>

namespace sequencergui
{

//! The overlay warning sign indended to appear in the corner of some widget.
//! Provide dialog window on click.

class WarningSignWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WarningSignWidget(const QString& message, QWidget* parent = nullptr);

  void SetHeader(const QString& header);

  bool IsBusy() const;

protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

private:
  QString m_header;
  QString m_message;
  QPixmap m_pixmap;
  bool m_is_busy{false};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_WARNING_SIGN_WIDGET_H_
