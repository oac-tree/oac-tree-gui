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

#include "splash_screen.h"

#include <sequencergui/core/version.h>

#include <mvvm/widgets/widget_utils.h>

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QStyle>
#include <QTime>

namespace
{

/**
 * @brief Returns string containing "loading ..." with number of dots changing according to the
 * processed parameter.
 */
QString GetLoadingText(int processed)
{
  QString result("loading . ");
  for (int i = 0; i < processed / 20; ++i)
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

SplashScreen::SplashScreen()
    : QSplashScreen(QPixmap(":/icons/splash-bg1.png")), m_percentage_done(0)

{
  QSplashScreen::setCursor(Qt::BusyCursor);
}

void SplashScreen::Start(int show_during)
{
  show();
  QTime die_time = QTime::currentTime().addMSecs(show_during);
  QElapsedTimer timer;
  timer.start();
  while (QTime::currentTime() < die_time)
  {
    SetProgress(timer.elapsed() / (show_during / 100));
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  }
}

void SplashScreen::SetProgress(int value)
{
  m_percentage_done = std::clamp(value, 0, 100);
  update();
}

void SplashScreen::drawContents(QPainter* painter)
{
  QSplashScreen::drawContents(painter);

  auto img_rect = frameGeometry();
  const auto char_height = mvvm::utils::WidthOfLetterM();
  const auto bottom_gap = char_height * 3.0;
  const auto text_rect_height = char_height * 3.0;

  QRect text_rect(img_rect.width() * 0.035, img_rect.height() - bottom_gap, img_rect.width() * 0.5,
                  text_rect_height);
  style()->drawItemText(painter, text_rect, 0, this->palette(), true, GetVersionText());

  QRect loading_rect(img_rect.width() * 0.8, img_rect.height() - bottom_gap, img_rect.width(),
                     text_rect_height);
  style()->drawItemText(painter, loading_rect, 0, this->palette(), true,
                        GetLoadingText(m_percentage_done));
}

}  // namespace sequencergui
