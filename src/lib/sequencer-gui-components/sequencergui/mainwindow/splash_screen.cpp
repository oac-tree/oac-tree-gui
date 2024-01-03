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

#include "splash_screen.h"

#include <sequencergui/core/version.h>

#include <mvvm/widgets/widget_utils.h>

#include <QCoreApplication>
#include <QStyle>
#include <QTime>

namespace
{

/**
 * @brief Returns string containing "loading ..." with the number of dots added.
 */
QString GetLoadingText(int dots_count)
{
  QString result("loading ");
  for (int i = 0; i < dots_count; ++i)
  {
    result.append(". ");
  }
  return result;
}

QString GetVersionText()
{
  QString result =
      QString("version ").append(QString::fromStdString(sequencergui::ProjectVersion()));
  return result;
}

}  // namespace

namespace sequencergui
{

SplashScreen::SplashScreen() : QSplashScreen(QPixmap(":/icons/splash-bg1.png"))

{
  QSplashScreen::setCursor(Qt::BusyCursor);
}

void SplashScreen::Start(int show_during)
{
  show();

  // The only purpose of this method is to show splash screen for given amount of time.
  // It is blocking, and the main window will not even begin to initialize before the end of the
  // method.

  auto end_time = QTime::currentTime().addMSecs(show_during);
  while (QTime::currentTime() < end_time)
  {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  }
}

void SplashScreen::drawContents(QPainter* painter)
{
  QSplashScreen::drawContents(painter);

  auto img_rect = frameGeometry();
  const auto char_height = mvvm::utils::WidthOfLetterM();
  const auto bottom_gap = char_height * 3.5;
  const auto text_rect_height = char_height * 3.0;

  QRect text_rect(img_rect.width() * 0.035, img_rect.height() - bottom_gap, img_rect.width() * 0.5,
                  text_rect_height);
  style()->drawItemText(painter, text_rect, 0, this->palette(), true, GetVersionText());

  QRect loading_rect(img_rect.width() * 0.8, img_rect.height() - bottom_gap, img_rect.width(),
                     text_rect_height);
  style()->drawItemText(painter, loading_rect, 0, this->palette(), true, GetLoadingText(5));
}

}  // namespace sequencergui
